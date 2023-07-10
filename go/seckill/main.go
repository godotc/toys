package main

import (
	"fmt"
	"log"
	"strconv"
	"sync"

	"github.com/hashicorp/consul/agent/consul"
	"github.com/hashicorp/consul/api"
)

type ServiceInstance struct {
	Host      string
	Port      int
	Weight    int
	CurWeight int
	GrpcPort  int
}

type DiscoverryClient interface {
	Register(
		instanceId, svcHost, healthCheckUrl, svcPort, svcName string,
		weight int,
		meta map[string]string,
		tags []string,
		logger *log.Logger,
	) bool

	UnRegister(instanceId string, logger *log.Logger) bool

	DiscoverryService(serviceName string, logger *log.Logger) []interface{}
}

type DiscoveryClientInstance struct {
	Host         string
	Port         int
	config       *api.Config // for consul
	client       *consul.Client
	mutex        sync.Mutex
	instancesMap sync.Map
}

func (c *DiscoveryClientInstance) Register(
	instanceId, svcHost, healthCheckUrl, svcPort, svcName string,
	weight int,
	meta map[string]string,
	tags []string,
	logger *log.Logger,
) bool {

	port, _ := strconv.Atoi(svcPort)

	// 1. construct the metadata of service_instance
	fmt.Println(weight)
	serviceRegistration := api.AgentServiceRegistration{
		ID:      instanceId,
		Name:    svcName,
		Address: svcHost,
		Port:    port,
		Meta:    meta,
		Tags:    tags,
		Weights: &api.AgentWeights{
			Passing: weight,
		},
		Check: &api.AgentServiceCheck{
			DeregisterCriticalServiceAfter: "30s",
			HTTP:                           "Http://" + svcHost + ":" + strconv.Itoa(port) + healthCheckUrl,
			Interval:                       "15s",
		},
	}

	// 2. send the service-registration to consul
	err := c.client.Register(serviceRegistration)

	if err != nil {
		logger.Println("Register service error")
		return false
	}

	logger.Println("Register service success")

	return false
}
