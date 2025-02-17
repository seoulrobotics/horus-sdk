import asyncio, horus, signal


async def print_sensor_status() -> None:
    sdk = horus.Sdk()
    end: asyncio.Future[None] = asyncio.Future()

    asyncio.get_running_loop().add_signal_handler(
        signal.SIGINT, lambda: end.set_result(None)
    )

    def print_sensor(sensor_info_event: horus.SensorInfoEvent) -> None:
        for sensor_info in sensor_info_event.sensor_info:
            print(
                f"Sensor {sensor_info.lidar_id}, status: {sensor_info.status}, frequency: {sensor_info.measured_frequency_hz} Hz, corrected pose: {sensor_info.corrected_pose}"
            )

    async with sdk.subscribe_to_sensor_info(on_sensor_info_event=print_sensor):
        await end


asyncio.run(print_sensor_status())
