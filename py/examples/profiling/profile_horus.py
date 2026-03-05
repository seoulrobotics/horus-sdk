# mypy: ignore-errors

import argparse
import asyncio
import datetime
import json
import os
import signal
import statistics
import subprocess
from collections import defaultdict
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple, Union

import horus
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from horus.sdk import profiling

# Constants
OVERALL_FRAME_LATENCY = "OverallFrameLatency"
FRAME_BUNDLING_WAITING = "FrameBundlingWaiting"
PREPROCESSING_OVERHEAD = "PreprocessingOverhead"
DETECTION_LATENCY = "DetectionLatency"
DETECTION_PROCESSING = "DetectionProcessing"
DETECTION_IDLE = "DetectionIdle"


class ServiceProfileData:
    """Holds profiling and resource-usage data for a single Horus service."""

    def __init__(self):
        self.profiling: Dict[str, List[float]] = defaultdict(list)
        self.cpu_usage: List[float] = []
        self.memory_usage: List[float] = []
        # Per-GPU metrics keyed by gpu_id
        self.gpu_utilization: Dict[int, List[float]] = defaultdict(list)
        self.gpu_memory: Dict[int, List[float]] = defaultdict(list)


class NumpyEncoder(json.JSONEncoder):
    """Custom JSON encoder to handle numpy types."""

    def default(self, o):
        if isinstance(o, (np.integer, np.floating, np.bool_)):
            return o.item()
        elif isinstance(o, np.ndarray):
            return o.tolist()
        return super().default(o)


class HorusProfiler:
    def __init__(self):
        self.detection: ServiceProfileData = ServiceProfileData()
        self.preprocessing: ServiceProfileData = ServiceProfileData()

        self.sdk_version: str = "unknown"
        self.start_time: Optional[datetime.datetime] = None
        self.end_time: Optional[datetime.datetime] = None

    def _get_git_commit_hash(self) -> str:
        """Get the current git commit hash."""
        github_sha = os.getenv("GITHUB_SHA")
        if github_sha:
            return github_sha[:7]

        try:
            result = subprocess.run(
                ["git", "rev-parse", "--short", "HEAD"],
                capture_output=True,
                text=True,
                check=False,
            )
            if result.returncode == 0:
                return result.stdout.strip()
        except Exception:
            pass
        return "unknown"

    def _get_current_branch(self) -> str:
        """Get the current git branch name."""
        github_ref_name = os.getenv("GITHUB_REF_NAME")
        if github_ref_name:
            return github_ref_name

        try:
            result = subprocess.run(
                ["git", "branch", "--show-current"],
                capture_output=True,
                text=True,
                check=False,
            )
            if result.returncode == 0:
                return result.stdout.strip()
        except Exception:
            pass
        return "unknown"

    def _parse_profiling_set(
        self, profiling_set: profiling.ProfilingSet | None, update_resource_usage=False
    ) -> None:
        if not profiling_set:
            return

        target_dict = None
        if (
            profiling_set.profiled_service
            == profiling.ProfiledService.DETECTION_SERVICE
        ):
            target_dict = self.detection.profiling
        elif (
            profiling_set.profiled_service
            == profiling.ProfiledService.PREPROCESSING_SERVICE
        ):
            target_dict = self.preprocessing.profiling

        if target_dict is not None:
            for component_name, elapsed_time in profiling_set.processing_times.items():
                target_dict[component_name].append(elapsed_time.total_seconds() * 1000)

        if update_resource_usage:
            match profiling_set.profiled_service:
                case profiling.ProfiledService.DETECTION_SERVICE:
                    self.detection.cpu_usage.append(
                        profiling_set.resource_usage.process.cpu_usage_percentage
                    )
                    self.detection.memory_usage.append(
                        profiling_set.resource_usage.process.memory_usage_bytes
                    )
                    for gpu in profiling_set.resource_usage.gpus:
                        self.detection.gpu_utilization[gpu.gpu_id].append(
                            gpu.utilization_percentage
                        )
                        self.detection.gpu_memory[gpu.gpu_id].append(
                            gpu.memory_used_bytes
                        )
                case profiling.ProfiledService.PREPROCESSING_SERVICE:
                    self.preprocessing.cpu_usage.append(
                        profiling_set.resource_usage.process.cpu_usage_percentage
                    )
                    self.preprocessing.memory_usage.append(
                        profiling_set.resource_usage.process.memory_usage_bytes
                    )
                    for gpu in profiling_set.resource_usage.gpus:
                        self.preprocessing.gpu_utilization[gpu.gpu_id].append(
                            gpu.utilization_percentage
                        )
                        self.preprocessing.gpu_memory[gpu.gpu_id].append(
                            gpu.memory_used_bytes
                        )

    def _parse_frame_profiling(
        self, frame_profiling: profiling.FrameProfiling | None
    ) -> None:
        if not frame_profiling:
            return
        self.detection.profiling[OVERALL_FRAME_LATENCY].append(
            frame_profiling.overall_frame_latency.total_seconds() * 1000
        )
        self.detection.profiling[FRAME_BUNDLING_WAITING].append(
            frame_profiling.frame_bundling_latency.total_seconds() * 1000
        )

    def _on_profiling_info(self, profiling_info: horus.ProfilingInfo) -> None:
        if profiling_info.profile_type == profiling.ProfileType.BUNDLED:
            bundled_frame = profiling_info.bundled_frame_profiling_set
            if bundled_frame:
                self._parse_frame_profiling(bundled_frame.frame_profiling)
                self._parse_profiling_set(
                    bundled_frame.detection_service_profiling.details_profiling_set
                )

                self.detection.profiling[DETECTION_LATENCY].append(
                    bundled_frame.detection_service_profiling.total_service_latency.total_seconds()
                    * 1000
                )

                proc_time = (
                    bundled_frame.detection_service_profiling.total_service_latency.total_seconds()
                    - bundled_frame.detection_service_profiling.idle_time_before_processing.total_seconds()
                )
                self.detection.profiling[DETECTION_PROCESSING].append(proc_time * 1000)

                total_idle = (
                    bundled_frame.detection_service_profiling.idle_time_before_processing.total_seconds()
                    + bundled_frame.detection_service_profiling.intra_component_idle_time.total_seconds()
                )
                self.detection.profiling[DETECTION_IDLE].append(total_idle * 1000)

                profiled_services = (
                    bundled_frame.preprocessing_service_point_cloud_profiling
                )
                for _, profiled_service in profiled_services.items():
                    self._parse_profiling_set(
                        profiled_service.service_profiling.details_profiling_set
                    )

                if (
                    self.detection.profiling[OVERALL_FRAME_LATENCY]
                    and self.detection.profiling[FRAME_BUNDLING_WAITING]
                ):
                    preprocessing_overhead = (
                        self.detection.profiling[OVERALL_FRAME_LATENCY][-1]
                        - self.detection.profiling[FRAME_BUNDLING_WAITING][-1]
                        - bundled_frame.detection_service_profiling.total_service_latency.total_seconds()
                        * 1000
                    )
                    self.preprocessing.profiling[PREPROCESSING_OVERHEAD].append(
                        preprocessing_overhead
                    )

        if profiling_info.profile_type == profiling.ProfileType.GENERAL:
            self._parse_profiling_set(
                profiling_info.general_profiling_set, update_resource_usage=True
            )

    async def collect_profiling(
        self, host_address: str, duration: Optional[float] = None
    ) -> Dict[str, Any]:
        """Connects to Horus and collects profiling data."""
        self.__init__()  # Reset data

        self.start_time = datetime.datetime.now()

        services = horus.RpcServices()
        if host_address not in ["localhost", "127.0.0.1"]:
            services = horus.RpcServices(
                **{
                    name: getattr(services, name).with_(host=host_address)
                    for name in services.__dataclass_fields__
                }
            )

        stop_event = asyncio.Event()

        if duration is not None:

            async def timer():
                await asyncio.sleep(duration)
                stop_event.set()

            asyncio.create_task(timer())
        else:
            loop = asyncio.get_running_loop()
            try:
                loop.add_signal_handler(signal.SIGINT, stop_event.set)
            except NotImplementedError:
                pass

        # Retry connecting until Horus is reachable or the user presses Ctrl+C.
        _RETRY_INTERVAL = 2.0
        sdk = None
        print(f"Connecting to Horus at {host_address} (press Ctrl+C to abort)...")
        while not stop_event.is_set():
            sdk = horus.Sdk(services)
            try:
                version = await sdk.get_version()
                self.sdk_version = str(version)
                print(f"Connected. SDK Version: {self.sdk_version}")
                break
            except Exception as e:
                print(
                    f"Failed to connect to Horus at {host_address}: {e}. Retrying in {_RETRY_INTERVAL:.0f}s..."
                )
                await sdk.close()
                sdk = None
                try:
                    await asyncio.wait_for(stop_event.wait(), timeout=_RETRY_INTERVAL)
                except asyncio.TimeoutError:
                    pass

        if stop_event.is_set() or sdk is None:
            print("Aborted before connecting.")
            return {}

        print("Subscribing to profiling data...")
        try:
            async with sdk.subscribe_to_profiling(
                on_profiling_info=self._on_profiling_info
            ):
                print("Collection started. Press Ctrl+C to stop.")
                await stop_event.wait()
                print("Collection stopped.")
        finally:
            await sdk.close()

        self.end_time = datetime.datetime.now()

        return {
            "detection": dict(self.detection.profiling),
            "preprocessing": dict(self.preprocessing.profiling),
            "cpu_detection": self.detection.cpu_usage,
            "cpu_preprocessing": self.preprocessing.cpu_usage,
            "mem_detection": self.detection.memory_usage,
            "mem_preprocessing": self.preprocessing.memory_usage,
            "gpu_utilization_detection": dict(self.detection.gpu_utilization),
            "gpu_utilization_preprocessing": dict(self.preprocessing.gpu_utilization),
            "gpu_memory_detection": dict(self.detection.gpu_memory),
            "gpu_memory_preprocessing": dict(self.preprocessing.gpu_memory),
        }

    def _calc_stats(self, values: List[float]) -> Dict[str, float]:
        if not values:
            return {"mean": 0.0, "median": 0.0, "std": 0.0, "min": 0.0, "max": 0.0}
        return {
            "mean": round(statistics.mean(values), 3),
            "median": round(statistics.median(values), 3),
            "std": round(statistics.stdev(values), 3) if len(values) > 1 else 0.0,
            "min": round(min(values), 3),
            "max": round(max(values), 3),
        }

    def save_json_report(
        self, output_path: Path, preset_name: str = "PERFORMANCE_TEST"
    ) -> None:
        """
        Generates a JSON report matching the EvaluationDetectionReport schema structure.
        """
        branch = self._get_current_branch()
        commit_hash = self._get_git_commit_hash()

        duration = 0
        if self.start_time and self.end_time:
            duration = (self.end_time - self.start_time).total_seconds()

        run_id = (
            f"{branch}_{self.end_time.strftime('%Y%m%d_%H%M%S')}"
            if self.end_time
            else "unknown"
        )

        latency_metrics = {}
        all_profiling = {
            **self.preprocessing.profiling,
            **self.detection.profiling,
        }

        for key, values in all_profiling.items():
            latency_metrics[key] = self._calc_stats(values)

        resource_metrics = {
            "cpu_detection_percent": self._calc_stats(self.detection.cpu_usage),
            "cpu_preprocessing_percent": self._calc_stats(self.preprocessing.cpu_usage),
            # Convert memory bytes to MB
            "mem_detection_mb": self._calc_stats(
                [m / 1e6 for m in self.detection.memory_usage]
            ),
            "mem_preprocessing_mb": self._calc_stats(
                [m / 1e6 for m in self.preprocessing.memory_usage]
            ),
        }
        for gpu_id, values in sorted(self.detection.gpu_utilization.items()):
            resource_metrics[f"gpu_{gpu_id}_detection_percent"] = self._calc_stats(
                values
            )
        for gpu_id, values in sorted(self.preprocessing.gpu_utilization.items()):
            resource_metrics[f"gpu_{gpu_id}_preprocessing_percent"] = self._calc_stats(
                values
            )
        for gpu_id, values in sorted(self.detection.gpu_memory.items()):
            resource_metrics[f"gpu_{gpu_id}_detection_mem_mb"] = self._calc_stats(
                [m / 1e6 for m in values]
            )
        for gpu_id, values in sorted(self.preprocessing.gpu_memory.items()):
            resource_metrics[f"gpu_{gpu_id}_preprocessing_mem_mb"] = self._calc_stats(
                [m / 1e6 for m in values]
            )

        json_report = {
            "horus_version": self.sdk_version,
            "git_commit": commit_hash,
            "branch": branch,
            "run_id": run_id,
            "evaluation_duration_sec": int(duration),
            "total_frames_evaluated": len(
                self.detection.profiling.get(OVERALL_FRAME_LATENCY, [])
            ),
            "timestamp": (
                self.end_time.strftime("%Y-%m-%dT%H:%M:%SZ") if self.end_time else ""
            ),
            "presets": {
                preset_name: {
                    "summary_metrics": {
                        "latency_ms": latency_metrics,
                        "resources": resource_metrics,
                    },
                    "total_frames": len(
                        self.detection.profiling.get(OVERALL_FRAME_LATENCY, [])
                    ),
                }
            },
        }

        with open(output_path, "w", encoding="utf-8") as f:
            json.dump(json_report, f, indent=2, cls=NumpyEncoder)

        print(f"JSON performance report saved to {output_path}")

    def _get_profiling_string(
        self, data: Dict[str, List], header: str, keys: List[str]
    ) -> str:
        s = f"\n## {header}\n"
        s += "| Component                       | Avg (ms) | Median (ms) | StdDev (ms) | Min (ms) | Max (ms) |\n"
        s += "|---------------------------------|----------|-------------|-------------|----------|----------|\n"
        for k in keys:
            if k not in data or not data[k]:
                continue
            vals = data[k]
            stats = self._calc_stats(vals)
            s += f"| {k:<32} | {stats['mean']:>8} | {stats['median']:>11} | {stats['std']:>11} | {stats['min']:>8} | {stats['max']:>8} |\n"
        return s

    def _get_service_resource_string(self, service: str) -> str:
        """Returns a resource usage sub-section for 'detection' or 'preprocessing'."""
        svc = self.detection if service == "detection" else self.preprocessing
        cpu = svc.cpu_usage
        mem = svc.memory_usage
        gpu_util = svc.gpu_utilization
        gpu_mem = svc.gpu_memory

        def row(label: str, values: List[float], unit: str) -> str:
            if not values:
                return ""
            stats = self._calc_stats(values)
            tag = f"{label} ({unit})"
            return (
                f"| {tag:<32} | {stats['mean']:>8} | {stats['median']:>11} |"
                f" {stats['std']:>11} | {stats['min']:>8} | {stats['max']:>8} |\n"
            )

        rows = ""
        rows += row("CPU", cpu, "%")
        rows += row("Memory", [v / 1e6 for v in mem], "MB")
        for gpu_id in sorted(gpu_util):
            rows += row(f"GPU {gpu_id} Utilization", gpu_util.get(gpu_id, []), "%")
            rows += row(
                f"GPU {gpu_id} Memory", [v / 1e6 for v in gpu_mem.get(gpu_id, [])], "MB"
            )

        if not rows:
            return ""

        s = "\n### Resource Usage\n"
        s += "| Metric                          | Avg      | Median      | StdDev      | Min      | Max      |\n"
        s += "|---------------------------------|----------|-------------|-------------|----------|----------|\n"
        s += rows
        return s

    def generate_report(self, output_prefix: str = "") -> Tuple[str, pd.DataFrame]:
        output_folder = "measurements"

        if not output_prefix:
            output_prefix = "measurement-" + datetime.datetime.now().strftime(
                "%Y-%m-%d-%H-%M-%S"
            )

        file_destination = Path(output_folder) / output_prefix
        file_destination.parent.mkdir(parents=True, exist_ok=True)

        det_keys = [
            "Clusterer",
            "ObjectDetector",
            "Merger",
            "ClusterBoxesFitting",
            "Tracker",
            "Classifier",
            "OutputManager",
            FRAME_BUNDLING_WAITING,
            DETECTION_PROCESSING,
            DETECTION_IDLE,
            DETECTION_LATENCY,
            OVERALL_FRAME_LATENCY,
        ]
        pre_keys = [
            "PointCloudParser",
            "DetectionRangeFilter",
            "IntensityFilter",
            "GridZoneFilter",
            "GroundFilter",
            "PlaneFilter",
            "StaticEnvFilter",
            "AttributeMerger",
            "OutputManager",
            "StaticEnvLearner",
            "TiltDetector",
            "RangeObstructionDetector",
            "SparseNoiseFilter",
            "OccupancyGridManager",
            PREPROCESSING_OVERHEAD,
        ]

        pre_str = self._get_profiling_string(
            self.preprocessing.profiling, "Preprocessing", pre_keys
        )
        pre_res_str = self._get_service_resource_string("preprocessing")
        det_str = self._get_profiling_string(
            self.detection.profiling, "Detection", det_keys
        )
        det_res_str = self._get_service_resource_string("detection")
        full_report = pre_str + pre_res_str + "\n" + det_str + det_res_str

        # Save Markdown
        with open(file_destination.with_suffix(".md"), "w") as f:
            f.write(full_report)

        # Save JSON for evaluation report
        self.save_json_report(file_destination.with_suffix(".json"))

        # Create DataFrame
        data_rows = []
        for key in det_keys:
            if key in self.detection.profiling:
                data_rows.extend(
                    [
                        {"Group": "Detection", "Key": key, "Time": t}
                        for t in self.detection.profiling[key]
                    ]
                )
        for key in pre_keys:
            if key in self.preprocessing.profiling:
                data_rows.extend(
                    [
                        {"Group": "Preprocessing", "Key": key, "Time": t}
                        for t in self.preprocessing.profiling[key]
                    ]
                )

        df = pd.DataFrame(data_rows)

        # Plot
        if not df.empty:
            sns.set_theme(style="whitegrid")
            fig1, axes1 = plt.subplots(1, 2, figsize=(18, 8))
            fig1.suptitle("Horus Profiling", fontsize=20)

            def _plot(ax, d, title):
                if not d.empty:
                    sns.boxplot(
                        ax=ax,
                        x="Key",
                        y="Time",
                        data=d,
                        hue="Key",
                        palette="Set2",
                        legend=False,
                    )
                    ax.set_title(title)
                    ax.set_ylabel("Time (ms)")
                    ax.set_xlabel("")
                    plt.setp(ax.xaxis.get_majorticklabels(), rotation=45, ha="right")

            _plot(axes1[0], df[df["Group"] == "Preprocessing"], "Preprocessing")
            _plot(axes1[1], df[df["Group"] == "Detection"], "Detection")

            plt.tight_layout(rect=[0, 0.03, 1, 0.95])
            plt.savefig(file_destination.with_suffix(".png"))
            plt.close()

        self._plot_resources(
            file_destination.with_name(file_destination.name + "_resources.png")
        )
        return full_report, df

    def _plot_resources(self, filename: str):
        if not self.preprocessing.cpu_usage and not self.detection.cpu_usage:
            return

        all_gpu_ids = sorted(
            set(self.detection.gpu_utilization)
            | set(self.preprocessing.gpu_utilization)
        )
        # 2 fixed rows (CPU, memory) + 2 rows per GPU (utilization, memory)
        n_rows = 2 + 2 * len(all_gpu_ids)
        fig, axes = plt.subplots(n_rows, 2, figsize=(15, 5 * n_rows))
        fig.suptitle("Resource Usage")

        axes[0, 0].plot(self.preprocessing.cpu_usage, color="blue")
        axes[0, 0].set_title("Preprocessing CPU %")
        axes[0, 1].plot(self.detection.cpu_usage, color="orange")
        axes[0, 1].set_title("Detection CPU %")
        axes[1, 0].plot(
            [m / 1e6 for m in self.preprocessing.memory_usage], color="green"
        )
        axes[1, 0].set_title("Preprocessing Mem (MB)")
        axes[1, 1].plot([m / 1e6 for m in self.detection.memory_usage], color="red")
        axes[1, 1].set_title("Detection Mem (MB)")

        for row_offset, gpu_id in enumerate(all_gpu_ids):
            util_row = 2 + 2 * row_offset
            mem_row = util_row + 1
            axes[util_row, 0].plot(
                self.preprocessing.gpu_utilization.get(gpu_id, []), color="blue"
            )
            axes[util_row, 0].set_title(f"Preprocessing GPU {gpu_id} Utilization %")
            axes[util_row, 1].plot(
                self.detection.gpu_utilization.get(gpu_id, []), color="orange"
            )
            axes[util_row, 1].set_title(f"Detection GPU {gpu_id} Utilization %")
            axes[mem_row, 0].plot(
                [m / 1e6 for m in self.preprocessing.gpu_memory.get(gpu_id, [])],
                color="green",
            )
            axes[mem_row, 0].set_title(f"Preprocessing GPU {gpu_id} Mem (MB)")
            axes[mem_row, 1].plot(
                [m / 1e6 for m in self.detection.gpu_memory.get(gpu_id, [])],
                color="red",
            )
            axes[mem_row, 1].set_title(f"Detection GPU {gpu_id} Mem (MB)")

        for ax in axes.flat:
            ax.grid(True)
        plt.tight_layout()
        plt.savefig(filename)
        plt.close()


async def main_cli():
    parser = argparse.ArgumentParser(description="Profile Horus components.")
    parser.add_argument("host_address", nargs="?", default="localhost")
    args = parser.parse_args()
    profiler = HorusProfiler()
    print("Starting profiling... Press Ctrl+C to stop.")
    try:
        result = await profiler.collect_profiling(args.host_address, duration=None)
    except asyncio.CancelledError:
        result = {}

    if not result:
        print("No data collected — exiting without generating a report.")
        return

    print("\nGenerating Report...")
    report_str, _ = profiler.generate_report()
    print(report_str)
    print("Done.")


if __name__ == "__main__":
    asyncio.run(main_cli())
