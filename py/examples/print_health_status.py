import asyncio, horus
from horus.sdk.health import (
    HealthStatus,
    LicensePrivilege,
    NodeHealth,
)
from horus.sdk.version import Version


async def print_health_status() -> None:
    health_status: HealthStatus
    version: Version
    async with horus.Sdk() as sdk:
        health_status = await sdk.get_health_status()
        version = await sdk.get_version()

    print(f"Horus version: {version}\n")

    print("---- License status ----")
    print(f"Licence level: {health_status.license_status.level}")
    print(f"Reason: {health_status.license_status.reason}")

    print("Privileges:")
    for privilege in health_status.license_status.privileges:
        print(f"- {privilege.name}")

    if health_status.license_status.license_info is not None:
        date_str = (
            health_status.license_status.license_info.expiration_datetime.strftime(
                "%Y-%m-%d %H:%M:%S"
            )
        )
        print(f"Expiration date: {date_str}")
        print(
            f"Lidar count: {health_status.license_status.license_info.number_of_lidars}"
        )
        if len(health_status.license_status.license_info.allowed_features) > 0:
            print(f"Allowed features:")
            for feature in health_status.license_status.license_info.allowed_features:
                print(f"  - {feature.name}")
        else:
            print(f"Allowed features: NONE!")

        print()

    if LicensePrivilege.READ not in health_status.license_status.privileges:
        print(
            "Sensor and service statuses not received because license level does not grant the READ privilege."
        )
        return

    print("---- Sensor statuses ----")
    for sensor_status in health_status.sensor_statuses:
        if sensor_status.is_unreachable():
            print(
                f"{sensor_status.lidar_id}: Unreachable! {sensor_status.node_unreachable_error}"
            )
            print()
            continue

        print(f"{sensor_status.lidar_id}:")
        print(f"- Status:")
        for status in sensor_status.status:
            print(f"  - {status.name}")

        print(
            f"- Measured frequency: {sensor_status.measured_frequency_hz}Hz",
        )
        print()

    print("---- Service statuses ----")
    for service in NodeHealth.Service:
        nodes = [
            node for node in health_status.service_statuses if node.service == service
        ]

        if len(nodes) == 0:
            print(f"{service.name}: No node found!\n")
            continue

        print(f"{service.name}")

        for node in nodes:
            print(f"- {node.node_id}: {node.node_status.name}")
        print()


asyncio.run(print_health_status())
