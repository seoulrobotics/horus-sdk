import asyncio, horus, signal

# mypy: disable-error-code="import-not-found"
import numpy as np  # type: ignore[unused-ignore, import-untyped]
import matplotlib.pyplot as plt  # type: ignore[unused-ignore, import-untyped]
from matplotlib import colors, patches  # type: ignore[unused-ignore, import-untyped]


async def print_occupancy_grid() -> None:
    sdk = horus.Sdk()
    end: asyncio.Future[None] = asyncio.Future()

    asyncio.get_running_loop().add_signal_handler(
        signal.SIGINT, lambda: end.set_result(None)
    )

    plt.ion()

    UNSPECIFIED = "white"
    FREE_COLOUR = "lightgreen"
    OCCLUDED_COLOUR = "gray"
    STATIC_OCCUPIED_COLOUR = "red"
    EXCLUDED_COLOUR = "black"
    cmap = colors.ListedColormap(
        [
            UNSPECIFIED,
            FREE_COLOUR,
            OCCLUDED_COLOUR,
            STATIC_OCCUPIED_COLOUR,
            EXCLUDED_COLOUR,
        ]
    )

    legend_labels = ["Free", "Occluded", "Stationary Occupied", "Excluded"]
    legend_colors = [
        FREE_COLOUR,
        OCCLUDED_COLOUR,
        STATIC_OCCUPIED_COLOUR,
        EXCLUDED_COLOUR,
    ]
    legend_patches = [
        patches.Patch(color=color, label=label)
        for color, label in zip(legend_colors, legend_labels)
    ]

    fig = plt.figure()

    def print_grids(event: horus.OccupancyGridListEvent) -> None:
        nodes = event.occupancy_grid_events
        n = len(nodes)
        if n == 0:
            return

        fig.clf()
        for i, node in enumerate(nodes):
            print(
                f"[{node.timestamp}] [{node.node_id}] {node.detection_range_name}: "
                f"{node.grid.rows}x{node.grid.cols} "
                f"x=[{node.x_min}, {node.x_max}] y=[{node.y_min}, {node.y_max}] "
                f"z=[{node.z_min}, {node.z_max}] res={node.resolution}"
            )

            ax = fig.add_subplot(1, n, i + 1)
            grid_map = np.asarray(node.grid.data).reshape(
                node.grid.rows, node.grid.cols
            )
            ax.set_title(f"{node.node_id} / {node.detection_range_name}")
            ax.imshow(grid_map, cmap=cmap, vmin=0, vmax=4)

        fig.axes[0].legend(
            handles=legend_patches, loc="upper right", title="Classification"
        )
        fig.tight_layout()
        plt.pause(0.01)

    async with sdk.subscribe_to_occupancy_grid(on_occupancy_grid_event=print_grids):
        await end


asyncio.run(print_occupancy_grid())
