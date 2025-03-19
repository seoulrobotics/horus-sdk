import asyncio, signal

import horus


def print_object_boxes(detection_event: horus.DetectionEvent) -> None:
    for bbox in detection_event.raw_deep_learning_bounding_boxes:
        print(f"Object detected at {bbox.base} with size {bbox.size}")
        print(f"Yaw: {bbox.yaw}")
        print()


async def subscribe_to_detection() -> None:
    sdk = horus.Sdk()
    end: asyncio.Future[None] = asyncio.Future()

    asyncio.get_running_loop().add_signal_handler(
        signal.SIGINT, lambda: end.set_result(None)
    )

    async with sdk.subscribe_to_detections(on_detection_event=print_object_boxes):
        await end


asyncio.run(subscribe_to_detection())
