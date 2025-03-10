package main

import (
	"context"
	"fmt"
	"os"
	"os/signal"

	"github.com/seoulrobotics/horus-sdk/go/horus"
	"github.com/seoulrobotics/horus-sdk/go/proto/detection_service/detection_pb"
	"google.golang.org/protobuf/encoding/prototext"
)

func main() {
	ctx := context.Background()

	// Only create connection to the detection service
	onlyDetectionService := horus.Services{}.WithDetection(*horus.DefaultServices().Detection)

	// Create SDK.
	sdk, err := horus.NewSdk(ctx, horus.SdkOptions{
		Services: &onlyDetectionService,
	})
	if err != nil {
		panic(err)
	}

	// Subscribe to detection events.
	subscription, err := sdk.SubscribeToObjects(func(e *detection_pb.DetectionEvent) {
		fmt.Printf("%d detected object(s):\n", len(e.GetObjects()))

		for _, obj := range e.GetObjects() {
			fmt.Println(prototext.MarshalOptions{Multiline: true}.Format(obj))
		}
	})

	if err != nil {
		panic(err)
	}

	defer subscription.Close()

	// Wait until end of program.
	end := make(chan os.Signal, 1)
	signal.Notify(end, os.Interrupt)
	<-end
}
