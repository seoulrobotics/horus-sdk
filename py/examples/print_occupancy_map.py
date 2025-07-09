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

    def print_grid(event: horus.OccupancyGridEvent) -> None:
        print(
            f"[{event.timestamp}] Received occupancy grid: {event.grid.rows}x{event.grid.cols}"
        )
        plt.clf()

        grid_map = np.asarray(event.grid.data).reshape(event.grid.rows, event.grid.cols)

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
        plt.legend(handles=legend_patches, loc="upper right", title="Classification")
        plt.imshow(grid_map, cmap=cmap, vmin=0, vmax=4)
        plt.pause(0.01)

    async with sdk.subscribe_to_occupancy_grid(on_occupancy_grid_event=print_grid):
        await end


asyncio.run(print_occupancy_grid())
