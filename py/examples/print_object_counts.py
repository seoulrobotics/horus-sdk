import asyncio, signal
from collections import Counter

import horus


def print_object_counts(detection_event: horus.DetectionEvent) -> None:
    object_count_dict = Counter()

    for obj in detection_event.objects:
        object_count_dict[obj.classification.class_label.name] += 1

    print(
        f"{detection_event.frame_info.frame_timestamp}, Total: {len(detection_event.objects)}, Object counts per type: {dict(object_count_dict)}"
    )


async def subscribe_to_detection() -> None:
    sdk = horus.Sdk()
    end = asyncio.Future()

    asyncio.get_running_loop().add_signal_handler(
        signal.SIGINT, lambda: end.set_result(None)
    )

    async with sdk.subscribe_to_detections(on_detection_event=print_object_counts):
        await end


asyncio.run(subscribe_to_detection())
