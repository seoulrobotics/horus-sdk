package horus

// ServiceInfo provides information about a Horus RPC service.
type ServiceInfo struct {
	// Unique full name of the service, e.g. `horus.pb.NotificationService`.
	FullName string
	// Unique identifier of the service, e.g. 1.
	Id uint16
	// Host to connect to the service, e.g. `localhost`.
	Host string
	// Port to connect to the service, e.g. `40001`.
	Port uint16
}

// WithHost returns a copy of si with its [ServiceInfo.Host] set to host.
func (si ServiceInfo) WithHost(host string) ServiceInfo {
	si.Host = host
	return si
}

// WithPort returns a copy of si with its [ServiceInfo.Port] set to port.
func (si ServiceInfo) WithPort(port uint16) ServiceInfo {
	si.Port = port
	return si
}

// Services provides the [ServiceInfo] of the Horus RPC services.
type Services struct {
	Detection ServiceInfo
	Notification ServiceInfo
	PointAggregator ServiceInfo
	ProjectManager ServiceInfo
}

// WithDetection returns a copy of s with its [Services.Detection] set to si.
func (s Services) WithDetection(si ServiceInfo) Services {
	s.Detection = si
	return s
}

// WithNotification returns a copy of s with its [Services.Notification] set to si.
func (s Services) WithNotification(si ServiceInfo) Services {
	s.Notification = si
	return s
}

// WithPointAggregator returns a copy of s with its [Services.PointAggregator] set to si.
func (s Services) WithPointAggregator(si ServiceInfo) Services {
	s.PointAggregator = si
	return s
}

// WithProjectManager returns a copy of s with its [Services.ProjectManager] set to si.
func (s Services) WithProjectManager(si ServiceInfo) Services {
	s.ProjectManager = si
	return s
}

// DefaultServices returns the default configuration of [Services].
func DefaultServices() Services {
	return Services{
		Detection: ServiceInfo{
			FullName: "horus.pb.DetectionService",
			Id: 7,
			Host: "127.0.0.1",
			Port: 40007,
		},
		Notification: ServiceInfo{
			FullName: "horus.pb.NotificationService",
			Id: 2,
			Host: "127.0.0.1",
			Port: 40002,
		},
		PointAggregator: ServiceInfo{
			FullName: "horus.pb.PointAggregatorService",
			Id: 6,
			Host: "127.0.0.1",
			Port: 40006,
		},
		ProjectManager: ServiceInfo{
			FullName: "horus.pb.ProjectManagerService",
			Id: 3,
			Host: "127.0.0.1",
			Port: 40003,
		},
	}
}
