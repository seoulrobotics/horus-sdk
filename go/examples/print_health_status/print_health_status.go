package main

import (
	"context"
	"fmt"
	"os"

	"github.com/seoulrobotics/horus-sdk/go/horus"
)

// Displays cleanly a health status.
func printHealthStatus(healthStatus *horus.HealthStatus) {
	fmt.Println("---- License status ----")
	fmt.Printf("Level: %d\n", healthStatus.LicenseStatus.LicenseLevel)
	fmt.Printf("Reason: %s\n", healthStatus.LicenseStatus.Reason)

	fmt.Println("Privileges:")
	for privilege, hasPrivilege := range healthStatus.LicenseStatus.Privileges {
		if hasPrivilege {
			fmt.Printf("- %s\n", privilege.String())
		}
	}

	if healthStatus.LicenseStatus.LicenseInfo != nil {
		fmt.Printf("Lidar count: %d\n", healthStatus.LicenseStatus.LicenseInfo.LidarCount)
		fmt.Printf("Expiration timestamp: %s\n", healthStatus.LicenseStatus.LicenseInfo.Expiration.String())
	}
	fmt.Println()

	if !healthStatus.LicenseStatus.HasPrivilege(horus.LicensePrivilege_READ) {
		fmt.Printf("License level does not grant Read privilege. Could not receive sensor and service statuses")
		return
	}

	fmt.Println("---- Sensor statuses ----")
	for _, sensorStatus := range healthStatus.SensorStatuses {
		fmt.Printf("%s:", sensorStatus.LidarId)

		if sensorStatus.IsUnreachable() {
			fmt.Printf(" Unreachable!: %s\n\n", sensorStatus.UnreachableReason)
			continue
		}

		fmt.Println()
		fmt.Println("- Statuses:")
		for status, hasStatus := range sensorStatus.Statuses {
			if hasStatus {
				fmt.Printf("  - %s\n", status.String())
			}
		}
		fmt.Printf("- Frequency: %f Hz\n", sensorStatus.MeasuredFrequencyHz)
		fmt.Println()
	}

	fmt.Println("---- Service statuses ----")

	printService := func(service horus.NodeHealthService) {
		var serviceNodes []horus.NodeHealth
		for _, serviceStatus := range healthStatus.ServiceStatuses {
			if serviceStatus.Service == service {
				serviceNodes = append(serviceNodes, serviceStatus)
			}
		}

		fmt.Printf("%s:", service.String())
		if len(serviceNodes) == 0 {
			fmt.Println(" No node found!")
			fmt.Println()
			return
		}
		fmt.Println()
		for _, serviceNode := range serviceNodes {
			fmt.Printf("- %s: %s\n", serviceNode.NodeId, serviceNode.Status.String())
		}
		fmt.Println()
	}

	printService(horus.NodeHealthService_SERVICE_DETECTION)
	printService(horus.NodeHealthService_SERVICE_LIDAR_RUNNER)
	printService(horus.NodeHealthService_SERVICE_NOTIFICATION)
	printService(horus.NodeHealthService_SERVICE_POINT_AGGREGATOR)
	printService(horus.NodeHealthService_SERVICE_PREPROCESSING)
}

func main() {
	ctx := context.Background()

	sdk, err := horus.NewSdk(ctx, horus.SdkOptions{})
	if err != nil {
		fmt.Printf("Error while creating the SDK: %s", err)
		os.Exit(1)
	}

	healthStatus, err := sdk.GetHealthStatus(horus.GetHealthStatusRequest{})
	if err != nil {
		fmt.Printf("Error while getting health status: %s", err)
		os.Exit(1)
	}

	printHealthStatus(healthStatus)
}
