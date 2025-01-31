import asyncio, horus, signal


def print_detection_profile(profiling_info: horus.ProfilingInfo) -> None:
    if profiling_set := profiling_info.bundled_frame_profiling_set:
        print(
            profiling_set.detection_service_profiling.details_profiling_set.profiled_service,
            profiling_set.detection_service_profiling.details_profiling_set.processing_times,
        )


async def print_detection() -> None:
    sdk = horus.Sdk()
    end = asyncio.Future()

    asyncio.get_running_loop().add_signal_handler(
        signal.SIGINT, lambda: end.set_result(None)
    )

    async with sdk.subscribe_to_profiling(on_profiling_info=print_detection_profile):
        await end


asyncio.run(print_detection())
