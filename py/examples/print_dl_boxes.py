import asyncio, signal

import horus


def print_object_boxes(detection_event: horus.DetectionEvent) -> None:
    for dl_object in detection_event.raw_deep_learning_objects:
        print(
            f"Class: {dl_object.classification.class_label} Confidence: {dl_object.classification.class_confidence}"
        )
        print(f"Bounding box: {dl_object.bounding_box}")
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
