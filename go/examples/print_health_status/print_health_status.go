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
		fmt.Printf("Expiration timestamp: %s\n", healthStatus.LicenseStatus.LicenseInfo.Expiration.String())
		fmt.Printf("Lidar count: %d\n", healthStatus.LicenseStatus.LicenseInfo.LidarCount)

		if len(healthStatus.LicenseStatus.LicenseInfo.AllowedFeatures) > 0 {
			fmt.Println("Allowed features:")
			for _, feature := range healthStatus.LicenseStatus.LicenseInfo.AllowedFeatures {
				fmt.Printf("  - %s\n", feature.String())
			}
		} else {
			fmt.Println("Allowed features: NONE!")
		}
	}
	fmt.Println()

	if !healthStatus.LicenseStatus.HasPrivilege(horus.LicensePrivilegeRead) {
		fmt.Printf("License level does not grant Read privilege. Could not receive sensor and service statuses")
		return
	}

	fmt.Println("---- Sensor statuses ----")
	for _, sensorStatus := range healthStatus.SensorStatuses {
		fmt.Printf("%s:", sensorStatus.LidarId)

		if sensorStatus.IsUnreachable() {
			fmt.Printf(" Unreachable: %s\n\n", sensorStatus.UnreachableReason)
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

	printService(horus.NodeHealthServiceServiceDetection)
	printService(horus.NodeHealthServiceServiceLidarRunner)
	printService(horus.NodeHealthServiceServiceNotification)
	printService(horus.NodeHealthServiceServicePointAggregator)
	printService(horus.NodeHealthServiceServicePreprocessing)
}

func main() {
	ctx := context.Background()

	// Only create connection to the project manager service
	onlyProjectManagerServices := horus.Services{}.WithProjectManager(*horus.DefaultServices().ProjectManager)

	sdk, err := horus.NewSdk(ctx, horus.SdkOptions{Services: &onlyProjectManagerServices})
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
