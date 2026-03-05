# Horus Profiling Tool

Collects latency, CPU, memory, and GPU metrics from a running Horus instance and
generates Markdown tables, a JSON report, and PNG charts.

## Install

Install the profiling dependencies from the repository root:

```sh
python -m pip install -r requirements-profiling.txt
```

## Run

Use [Horus SDK](https://github.com/seoulrobotics/horus-sdk) for installation and
instructions.

```sh
python profile_horus.py [host_address]
```

The tool reconnects automatically if Horus is not reachable yet. Press
**Ctrl+C** to stop and generate the report.

## Output

Reports are saved to `measurements/`:

| File             | Contents                                                                |
| ---------------- | ----------------------------------------------------------------------- |
| `.md`            | Latency stats per component (mean, median, std, min, max)               |
| `.json`          | Latency and resource metrics, git metadata, SDK version                 |
| `.png`           | Box plots of component latencies                                        |
| `_resources.png` | CPU %, memory (MB), and per-GPU utilization % and memory (MB) over time |

GPU entries only appear when an NVIDIA GPU is detected on the host running
Horus.
