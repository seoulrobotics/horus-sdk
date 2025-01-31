# Horus Go SDK

> [!WARNING]\
> The Go SDK is intended for internal use, and may be modified or removed
> without warning.

## Usage

```sh
go get github.com/seoulrobotics/horus-sdk/go
```

```go
import (
    "context"
    "fmt"

    "github.com/seoulrobotics/horus-sdk/go/horus"
    "github.com/seoulrobotics/horus-sdk/go/proto/detection_service/detection_pb"
)

func printObjects(until <-chan bool) error {
    ctx := context.Background()

    sdk, err := horus.NewSdk(ctx, horus.SdkOptions{})
    if err != nil {
        return err
    }
    defer sdk.Close()

    subscription := sdk.SubscribeToObjects(func(e *detection_pb.DetectionEvent) {
        fmt.Printf("detected objects: %s\n", e.GetObjects())
    })
    defer subscription.Close()

    <-until

    return nil
}
```

## Examples

```sh
go run examples/print_objects.go
```
