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
from typing import Any, Dict, List, Optional, Tuple

import horus
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from horus.sdk import profiling

# Per-frame timings sourced from the detection merger.
OVERALL_FRAME_LATENCY = "OverallFrameLatency"
DETECTION_MERGER_OVERHEAD = "DetectionMergerOverhead"

# Per-frame timings sourced from each detection service (bundled frame profiling).
FRAME_BUNDLING_WAITING = "FrameBundlingWaiting"
PREPROCESSING_OVERHEAD = "PreprocessingOverhead"

# Service-level aggregates. `TotalIdleTime` is reported for both detection and preprocessing;
# the other two are only meaningful for detection (matching the frontend latency widget).
TOTAL_SERVICE_LATENCY = "TotalServiceLatency"
TOTAL_SERVICE_PROCESSING = "TotalServiceProcessing"
TOTAL_IDLE_TIME = "TotalIdleTime"


class ProfileBucket:
    """Per-component latency samples (ms) from a single source (e.g. one detection node)."""

    def __init__(self) -> None:
        self.timings: Dict[str, List[float]] = defaultdict(list)


class ResourceBucket:
    """CPU / memory / GPU samples for a single (service, node) pair."""

    def __init__(self) -> None:
        self.cpu_usage: List[float] = []
        self.memory_usage: List[float] = []
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
    def __init__(self) -> None:
        # Per-frame timings, grouped by source.
        self.detection_by_node: Dict[str, ProfileBucket] = defaultdict(ProfileBucket)
        self.preprocessing_by_lidar: Dict[Tuple[str, str], ProfileBucket] = defaultdict(
            ProfileBucket
        )
        self.merger: ProfileBucket = ProfileBucket()

        # Resource usage from general profiling sets, grouped by node.
        self.detection_resources_by_node: Dict[str, ResourceBucket] = defaultdict(
            ResourceBucket
        )
        self.preprocessing_resources_by_node: Dict[str, ResourceBucket] = defaultdict(
            ResourceBucket
        )
        self.merger_resources_by_node: Dict[str, ResourceBucket] = defaultdict(
            ResourceBucket
        )

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

    @staticmethod
    def _record_processing_times(
        bucket: ProfileBucket, service_profiling: profiling.ServiceProfiling
    ) -> None:
        """Records per-component processing times and the total idle time.

        Shared between detection and preprocessing; the only thing both services have in
        common at this level is per-component processing times and idle time.
        """
        details = service_profiling.details_profiling_set
        for component_name, elapsed in details.processing_times.items():
            bucket.timings[component_name].append(elapsed.total_seconds() * 1000)

        idle_before_s = service_profiling.idle_time_before_processing.total_seconds()
        idle_intra_s = service_profiling.intra_component_idle_time.total_seconds()
        bucket.timings[TOTAL_IDLE_TIME].append((idle_before_s + idle_intra_s) * 1000)

    @staticmethod
    def _record_detection_totals(
        bucket: ProfileBucket, service_profiling: profiling.ServiceProfiling
    ) -> None:
        """Records the detection-only service-level aggregates."""
        total_latency_s = service_profiling.total_service_latency.total_seconds()
        idle_before_s = service_profiling.idle_time_before_processing.total_seconds()

        bucket.timings[TOTAL_SERVICE_LATENCY].append(total_latency_s * 1000)
        bucket.timings[TOTAL_SERVICE_PROCESSING].append(
            (total_latency_s - idle_before_s) * 1000
        )

    @staticmethod
    def _record_resource_usage(
        resources: ResourceBucket, profiling_set: profiling.ProfilingSet
    ) -> None:
        usage = profiling_set.resource_usage
        resources.cpu_usage.append(usage.process.cpu_usage_percentage)
        resources.memory_usage.append(usage.process.memory_usage_bytes)
        for gpu in usage.gpus:
            resources.gpu_utilization[gpu.gpu_id].append(gpu.utilization_percentage)
            resources.gpu_memory[gpu.gpu_id].append(gpu.memory_used_bytes)

    def _on_profiling_info(self, profiling_info: horus.ProfilingInfo) -> None:
        if profiling_info.profile_type == profiling.ProfileType.BUNDLED:
            bundled = profiling_info.bundled_frame_profiling_set
            if bundled is None:
                return
            node_id = bundled.detection_service_profiling.node_id
            bucket = self.detection_by_node[node_id]
            self._record_processing_times(bucket, bundled.detection_service_profiling)
            self._record_detection_totals(bucket, bundled.detection_service_profiling)
            # Frame-level timings owned by the detection service.
            bucket.timings[FRAME_BUNDLING_WAITING].append(
                bundled.frame_profiling.frame_bundling_latency.total_seconds() * 1000
            )
            bucket.timings[PREPROCESSING_OVERHEAD].append(
                bundled.frame_profiling.preprocessing_overhead.total_seconds() * 1000
            )

        elif profiling_info.profile_type == profiling.ProfileType.PREPROCESSING_FRAME:
            preprocessing = profiling_info.preprocessing_frame_profiling
            if preprocessing is None:
                return
            key = (preprocessing.service_profiling.node_id, preprocessing.lidar_id)
            bucket = self.preprocessing_by_lidar[key]
            self._record_processing_times(bucket, preprocessing.service_profiling)

        elif (
            profiling_info.profile_type == profiling.ProfileType.DETECTION_MERGER_FRAME
        ):
            merger = profiling_info.detection_merger_frame_profiling
            if merger is None:
                return
            self.merger.timings[OVERALL_FRAME_LATENCY].append(
                merger.total_overall_frame_latency.total_seconds() * 1000
            )
            self.merger.timings[DETECTION_MERGER_OVERHEAD].append(
                merger.detection_merger_overhead.total_seconds() * 1000
            )

        elif profiling_info.profile_type == profiling.ProfileType.GENERAL:
            ps = profiling_info.general_profiling_set
            if ps is None:
                return
            if ps.profiled_service == profiling.ProfiledService.DETECTION_SERVICE:
                self._record_resource_usage(
                    self.detection_resources_by_node[ps.node_id], ps
                )
            elif ps.profiled_service == profiling.ProfiledService.PREPROCESSING_SERVICE:
                self._record_resource_usage(
                    self.preprocessing_resources_by_node[ps.node_id], ps
                )
            elif (
                ps.profiled_service
                == profiling.ProfiledService.DETECTION_MERGER_SERVICE
            ):
                self._record_resource_usage(
                    self.merger_resources_by_node[ps.node_id], ps
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
            "merger": dict(self.merger.timings),
            "detection_by_node": {
                node_id: dict(bucket.timings)
                for node_id, bucket in self.detection_by_node.items()
            },
            "preprocessing_by_lidar": {
                f"{node_id}/{lidar_id}": dict(bucket.timings)
                for (node_id, lidar_id), bucket in self.preprocessing_by_lidar.items()
            },
            "resources_by_node": {
                service: {
                    node_id: {
                        "cpu_usage": resources.cpu_usage,
                        "memory_usage": resources.memory_usage,
                        "gpu_utilization": dict(resources.gpu_utilization),
                        "gpu_memory": dict(resources.gpu_memory),
                    }
                    for node_id, resources in by_node.items()
                }
                for service, by_node in (
                    ("detection", self.detection_resources_by_node),
                    ("preprocessing", self.preprocessing_resources_by_node),
                    ("merger", self.merger_resources_by_node),
                )
            },
        }

    def _preprocessing_buckets(
        self, per_lidar: bool
    ) -> Dict[Tuple[str, ...], ProfileBucket]:
        """Returns preprocessing buckets keyed for display.

        - `per_lidar=True`: keys are `(node_id, lidar_id)` (the natural collection granularity).
        - `per_lidar=False`: samples are pooled across all lidars under each node_id, with keys
          `(node_id,)`.
        """
        if per_lidar:
            return dict(self.preprocessing_by_lidar)

        merged: Dict[Tuple[str, ...], ProfileBucket] = defaultdict(ProfileBucket)
        for (node_id, _lidar_id), bucket in self.preprocessing_by_lidar.items():
            target = merged[(node_id,)]
            for key, values in bucket.timings.items():
                target.timings[key].extend(values)
        return merged

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

    def _stats_for_bucket(self, bucket: ProfileBucket) -> Dict[str, Dict[str, float]]:
        return {key: self._calc_stats(values) for key, values in bucket.timings.items()}

    def _resource_metrics(
        self, resources: ResourceBucket
    ) -> Dict[str, Dict[str, float]]:
        metrics: Dict[str, Dict[str, float]] = {
            "cpu_percent": self._calc_stats(resources.cpu_usage),
            "memory_mb": self._calc_stats([m / 1e6 for m in resources.memory_usage]),
        }
        for gpu_id, values in sorted(resources.gpu_utilization.items()):
            metrics[f"gpu_{gpu_id}_utilization_percent"] = self._calc_stats(values)
        for gpu_id, values in sorted(resources.gpu_memory.items()):
            metrics[f"gpu_{gpu_id}_memory_mb"] = self._calc_stats(
                [m / 1e6 for m in values]
            )
        return metrics

    def _total_frames(self) -> int:
        return len(self.merger.timings.get(OVERALL_FRAME_LATENCY, []))

    def save_json_report(
        self,
        output_path: Path,
        preset_name: str = "PERFORMANCE_TEST",
        per_lidar: bool = False,
    ) -> None:
        """Generates a JSON report grouped by node / service.

        When `per_lidar` is True, preprocessing samples are kept per `(node_id, lidar_id)`;
        otherwise they're aggregated across all lidars under each preprocessing node.
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

        preprocessing_buckets = self._preprocessing_buckets(per_lidar)
        preprocessing_metrics = {
            "/".join(key): self._stats_for_bucket(bucket)
            for key, bucket in preprocessing_buckets.items()
        }
        preprocessing_label = (
            "preprocessing_by_lidar" if per_lidar else "preprocessing_by_node"
        )

        latency_metrics = {
            "merger": self._stats_for_bucket(self.merger),
            "detection_by_node": {
                node_id: self._stats_for_bucket(bucket)
                for node_id, bucket in self.detection_by_node.items()
            },
            preprocessing_label: preprocessing_metrics,
        }

        resource_metrics = {
            service: {
                node_id: self._resource_metrics(resources)
                for node_id, resources in by_node.items()
            }
            for service, by_node in (
                ("detection", self.detection_resources_by_node),
                ("preprocessing", self.preprocessing_resources_by_node),
                ("merger", self.merger_resources_by_node),
            )
        }

        total_frames = self._total_frames()
        json_report = {
            "horus_version": self.sdk_version,
            "git_commit": commit_hash,
            "branch": branch,
            "run_id": run_id,
            "evaluation_duration_sec": int(duration),
            "total_frames_evaluated": total_frames,
            "timestamp": (
                self.end_time.strftime("%Y-%m-%dT%H:%M:%SZ") if self.end_time else ""
            ),
            "presets": {
                preset_name: {
                    "summary_metrics": {
                        "latency_ms": latency_metrics,
                        "resources": resource_metrics,
                    },
                    "total_frames": total_frames,
                }
            },
        }

        with open(output_path, "w", encoding="utf-8") as f:
            json.dump(json_report, f, indent=2, cls=NumpyEncoder)

        print(f"JSON performance report saved to {output_path}")

    def _format_timings_table(
        self, header: str, timings: Dict[str, List[float]], ordered_keys: List[str]
    ) -> str:
        s = f"\n## {header}\n"
        s += "| Component                       | Avg (ms) | Median (ms) | StdDev (ms) | Min (ms) | Max (ms) |\n"
        s += "|---------------------------------|----------|-------------|-------------|----------|----------|\n"
        # Emit ordered_keys first, then anything else encountered (so unknown components still show).
        seen = set()
        for k in ordered_keys:
            if k in timings and timings[k]:
                seen.add(k)
                stats = self._calc_stats(timings[k])
                s += f"| {k:<32} | {stats['mean']:>8} | {stats['median']:>11} | {stats['std']:>11} | {stats['min']:>8} | {stats['max']:>8} |\n"
        for k in sorted(timings):
            if k in seen or not timings[k]:
                continue
            stats = self._calc_stats(timings[k])
            s += f"| {k:<32} | {stats['mean']:>8} | {stats['median']:>11} | {stats['std']:>11} | {stats['min']:>8} | {stats['max']:>8} |\n"
        return s

    def _format_resource_table(self, header: str, resources: ResourceBucket) -> str:
        def row(label: str, values: List[float], unit: str) -> str:
            if not values:
                return ""
            stats = self._calc_stats(values)
            tag = f"{label} ({unit})"
            return (
                f"| {tag:<32} | {stats['mean']:>8} | {stats['median']:>11} |"
                f" {stats['std']:>11} | {stats['min']:>8} | {stats['max']:>8} |\n"
            )

        rows = row("CPU", resources.cpu_usage, "%")
        rows += row("Memory", [v / 1e6 for v in resources.memory_usage], "MB")
        for gpu_id in sorted(resources.gpu_utilization):
            rows += row(
                f"GPU {gpu_id} Utilization", resources.gpu_utilization[gpu_id], "%"
            )
            rows += row(
                f"GPU {gpu_id} Memory",
                [v / 1e6 for v in resources.gpu_memory.get(gpu_id, [])],
                "MB",
            )

        if not rows:
            return ""

        s = f"\n### Resource Usage — {header}\n"
        s += "| Metric                          | Avg      | Median      | StdDev      | Min      | Max      |\n"
        s += "|---------------------------------|----------|-------------|-------------|----------|----------|\n"
        s += rows
        return s

    def generate_report(
        self, output_prefix: str = "", per_lidar: bool = False
    ) -> Tuple[str, pd.DataFrame]:
        """Generates Markdown / JSON / plot reports.

        `per_lidar` controls preprocessing granularity: by default (False) preprocessing samples
        are aggregated across all lidars within each preprocessing node; if True, each
        `(node_id, lidar_id)` pair is reported separately.
        """
        output_folder = "measurements"

        if not output_prefix:
            output_prefix = "measurement-" + datetime.datetime.now().strftime(
                "%Y-%m-%d-%H-%M-%S"
            )

        file_destination = Path(output_folder) / output_prefix
        file_destination.parent.mkdir(parents=True, exist_ok=True)

        merger_keys = [OVERALL_FRAME_LATENCY, DETECTION_MERGER_OVERHEAD]
        detection_keys = [
            "Clusterer",
            "ObjectDetector",
            "Merger",
            "ClusterBoxesFitting",
            "Tracker",
            "Classifier",
            "OutputManager",
            FRAME_BUNDLING_WAITING,
            PREPROCESSING_OVERHEAD,
            TOTAL_SERVICE_PROCESSING,
            TOTAL_IDLE_TIME,
            TOTAL_SERVICE_LATENCY,
        ]
        preprocessing_keys = [
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
            TOTAL_IDLE_TIME,
        ]

        sections: List[str] = []
        sections.append(
            self._format_timings_table(
                "Detection Merger", dict(self.merger.timings), merger_keys
            )
        )
        for node_id in sorted(self.merger_resources_by_node):
            sections.append(
                self._format_resource_table(
                    f"Detection Merger (node: {node_id})",
                    self.merger_resources_by_node[node_id],
                )
            )

        for node_id in sorted(self.detection_by_node):
            sections.append(
                self._format_timings_table(
                    f"Detection (node: {node_id})",
                    dict(self.detection_by_node[node_id].timings),
                    detection_keys,
                )
            )
            if node_id in self.detection_resources_by_node:
                sections.append(
                    self._format_resource_table(
                        f"Detection (node: {node_id})",
                        self.detection_resources_by_node[node_id],
                    )
                )

        preprocessing_buckets = self._preprocessing_buckets(per_lidar)
        seen_preprocessing_nodes: set = set()
        for key, bucket in sorted(preprocessing_buckets.items()):
            if per_lidar:
                node_id, lidar_id = key
                header = f"Preprocessing (node: {node_id}, lidar: {lidar_id})"
            else:
                (node_id,) = key
                header = f"Preprocessing (node: {node_id}, all lidars)"
            sections.append(
                self._format_timings_table(
                    header, dict(bucket.timings), preprocessing_keys
                )
            )
            # Emit each node's resource table once, after its first timings table.
            if (
                node_id in self.preprocessing_resources_by_node
                and node_id not in seen_preprocessing_nodes
            ):
                seen_preprocessing_nodes.add(node_id)
                sections.append(
                    self._format_resource_table(
                        f"Preprocessing (node: {node_id})",
                        self.preprocessing_resources_by_node[node_id],
                    )
                )

        full_report = "".join(sections)

        with open(file_destination.with_suffix(".md"), "w") as f:
            f.write(full_report)

        self.save_json_report(
            file_destination.with_suffix(".json"), per_lidar=per_lidar
        )

        df = self._build_dataframe(
            detection_keys, preprocessing_keys, merger_keys, per_lidar=per_lidar
        )
        if not df.empty:
            self._plot_timings(df, file_destination.with_suffix(".png"))

        self._plot_resources(
            file_destination.with_name(file_destination.name + "_resources.png")
        )
        return full_report, df

    def _build_dataframe(
        self,
        detection_keys: List[str],
        preprocessing_keys: List[str],
        merger_keys: List[str],
        per_lidar: bool = False,
    ) -> pd.DataFrame:
        data_rows = []
        for key in merger_keys:
            for t in self.merger.timings.get(key, []):
                data_rows.append({"Group": "Merger", "Key": key, "Time": t})
        for node_id, bucket in self.detection_by_node.items():
            group = f"Detection ({node_id})"
            for key in detection_keys:
                for t in bucket.timings.get(key, []):
                    data_rows.append({"Group": group, "Key": key, "Time": t})
        for bucket_key, bucket in self._preprocessing_buckets(per_lidar).items():
            if per_lidar:
                node_id, lidar_id = bucket_key
                group = f"Preprocessing ({node_id}/{lidar_id})"
            else:
                (node_id,) = bucket_key
                group = f"Preprocessing ({node_id})"
            for key in preprocessing_keys:
                for t in bucket.timings.get(key, []):
                    data_rows.append({"Group": group, "Key": key, "Time": t})
        return pd.DataFrame(data_rows)

    def _plot_timings(self, df: pd.DataFrame, filename: Path) -> None:
        sns.set_theme(style="whitegrid")
        groups = sorted(df["Group"].unique())
        n = len(groups)
        if n == 0:
            return
        fig, axes = plt.subplots(n, 1, figsize=(14, max(4, 4 * n)), squeeze=False)
        fig.suptitle("Horus Profiling", fontsize=20)

        for ax, group in zip(axes[:, 0], groups):
            sub = df[df["Group"] == group]
            if sub.empty:
                continue
            sns.boxplot(
                ax=ax,
                x="Key",
                y="Time",
                data=sub,
                hue="Key",
                palette="Set2",
                legend=False,
            )
            ax.set_title(group)
            ax.set_ylabel("Time (ms)")
            ax.set_xlabel("")
            plt.setp(ax.xaxis.get_majorticklabels(), rotation=45, ha="right")

        plt.tight_layout(rect=[0, 0.03, 1, 0.97])
        plt.savefig(filename)
        plt.close()

    def _plot_resources(self, filename: Path) -> None:
        # One column per (service, node) pair.
        columns: List[Tuple[str, ResourceBucket]] = []
        for node_id in sorted(self.preprocessing_resources_by_node):
            columns.append(
                (
                    f"Preprocessing\n{node_id}",
                    self.preprocessing_resources_by_node[node_id],
                )
            )
        for node_id in sorted(self.detection_resources_by_node):
            columns.append(
                (f"Detection\n{node_id}", self.detection_resources_by_node[node_id])
            )
        for node_id in sorted(self.merger_resources_by_node):
            columns.append(
                (f"Merger\n{node_id}", self.merger_resources_by_node[node_id])
            )

        if not columns or not any(r.cpu_usage or r.memory_usage for _, r in columns):
            return

        all_gpu_ids = sorted(
            {gpu_id for _, r in columns for gpu_id in r.gpu_utilization}
            | {gpu_id for _, r in columns for gpu_id in r.gpu_memory}
        )

        n_rows = 2 + 2 * len(all_gpu_ids)  # CPU, Mem, then per-GPU (util, mem)
        n_cols = len(columns)
        fig, axes = plt.subplots(
            n_rows, n_cols, figsize=(6 * n_cols, 5 * n_rows), squeeze=False
        )
        fig.suptitle("Resource Usage")

        for col, (title, resources) in enumerate(columns):
            self._plot_or_hide(axes[0, col], resources.cpu_usage, f"{title}\nCPU %")
            self._plot_or_hide(
                axes[1, col],
                [m / 1e6 for m in resources.memory_usage],
                f"{title}\nMemory (MB)",
            )
            for row_offset, gpu_id in enumerate(all_gpu_ids):
                util_row = 2 + 2 * row_offset
                mem_row = util_row + 1
                # Only show GPU rows for columns that actually have data for this GPU.
                self._plot_or_hide(
                    axes[util_row, col],
                    resources.gpu_utilization.get(gpu_id, []),
                    f"{title}\nGPU {gpu_id} Utilization %",
                )
                self._plot_or_hide(
                    axes[mem_row, col],
                    [m / 1e6 for m in resources.gpu_memory.get(gpu_id, [])],
                    f"{title}\nGPU {gpu_id} Memory (MB)",
                )

        plt.tight_layout()
        plt.savefig(filename)
        plt.close()

    @staticmethod
    def _plot_or_hide(ax, values: List[float], title: str) -> None:
        if not values:
            ax.set_visible(False)
            return
        ax.plot(values)
        ax.set_title(title)
        ax.grid(True)


async def main_cli():
    parser = argparse.ArgumentParser(description="Profile Horus components.")
    parser.add_argument("host_address", nargs="?", default="localhost")
    parser.add_argument(
        "--per-lidar",
        action="store_true",
        help=(
            "Report preprocessing metrics for each (node, lidar) pair separately. "
            "By default, preprocessing samples are averaged across all lidars under each "
            "preprocessing node."
        ),
    )
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
    report_str, _ = profiler.generate_report(per_lidar=args.per_lidar)
    print(report_str)
    print("Done.")


if __name__ == "__main__":
    asyncio.run(main_cli())
