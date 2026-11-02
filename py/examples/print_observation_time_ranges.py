import asyncio, signal

import horus


def print_observation_time_ranges(detection_event: horus.DetectionEvent) -> None:
    frame_timestamp = detection_event.frame_info.frame_timestamp
    print(f"Frame {frame_timestamp}, {len(detection_event.objects)} objects")

    for obj in detection_event.objects:
        time_range = obj.status.observation_time_range
        span_ms = (time_range.end - time_range.start).total_seconds() * 1000
        end_age_ms = (frame_timestamp - time_range.end).total_seconds() * 1000
        print(
            f"  id={obj.status.id:<5} {obj.classification.class_label.name:<10} "
            f"{obj.status.tracking_status.name:<12} "
            f"start={time_range.start.time()} end={time_range.end.time()} "
            f"span={span_ms:7.2f}ms frame-end={end_age_ms:7.2f}ms"
        )


async def subscribe_to_detection() -> None:
    sdk = horus.Sdk()
    end: asyncio.Future[None] = asyncio.Future()

    asyncio.get_running_loop().add_signal_handler(
        signal.SIGINT, lambda: end.set_result(None)
    )

    async with sdk.subscribe_to_detections(
        on_detection_event=print_observation_time_ranges
    ):
        await end


asyncio.run(subscribe_to_detection())
