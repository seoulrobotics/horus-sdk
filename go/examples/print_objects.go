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

	// Create SDK.
	sdk, err := horus.NewSdk(ctx, horus.SdkOptions{
		OnConnected: func(si horus.ServiceInfo) {
			fmt.Printf("%s connected\n", si.FullName)
		},
		OnDisconnected: func(si horus.ServiceInfo, err error) {
			if err != nil {
				fmt.Printf("%s disconnected: %s\n", si.FullName, err.Error())
			} else {
				fmt.Printf("%s disconnected\n", si.FullName)
			}
		},
		OnError: func(si horus.ServiceInfo, err error) {
			fmt.Printf("%s error: %s\n", si.FullName, err.Error())
		},
	})
	if err != nil {
		panic(err)
	}
	defer sdk.Close()

	// Subscribe to detection events.
	subscription := sdk.SubscribeToObjects(func(e *detection_pb.DetectionEvent) {
		fmt.Printf("%d detected object(s):\n", len(e.GetObjects()))

		for _, obj := range e.GetObjects() {
			fmt.Println(prototext.MarshalOptions{Multiline: true}.Format(obj))
		}
	})
	defer subscription.Close()

	// Wait until end of program.
	end := make(chan os.Signal, 1)
	signal.Notify(end, os.Interrupt)
	<-end
}
