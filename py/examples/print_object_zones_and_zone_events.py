import asyncio, signal

import horus


def on_detection_event(detection_event: horus.DetectionEvent) -> None:
    for obj in detection_event.objects:
        print(
            f"Object {obj.status.id} ({obj.classification.class_label.name}) "
            f"belongs to zones: {obj.event_zone_ids if obj.event_zone_ids else '(none)'}"
        )


def on_zone_event(zone_event: horus.ZoneEvent) -> None:
    object_id = (
        zone_event.object.status.id
        if zone_event.object is not None
        else zone_event.object_id
    )
    print(
        f"Zone event: {zone_event.type.name} - "
        f"object {object_id} "
        f"in zone '{zone_event.zone_id}'"
    )


async def main() -> None:
    sdk = horus.Sdk()
    end: asyncio.Future[None] = asyncio.Future()

    asyncio.get_running_loop().add_signal_handler(
        signal.SIGINT, lambda: end.set_result(None)
    )

    async with (
        sdk.subscribe_to_detections(on_detection_event=on_detection_event),
        sdk.subscribe_to_zone_events(on_zone_event=on_zone_event),
    ):
        await end


asyncio.run(main())
