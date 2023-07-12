package discover

import "log"

type DiscoverryClient interface {
	Register(
		servicename, instanceId, healthCheckUrl string,
		instanceHost string,
		instacePort int,
		meta map[string]string,
		logger *log.Logger,
	) bool

	UnRegister(instanceId string, logger *log.Logger) bool

	DiscoverryService(serviceName string, logger *log.Logger) []interface{}
}
