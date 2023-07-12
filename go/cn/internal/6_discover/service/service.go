package service

import (
	"context"
)

type Service interface {
	HealthCheck() bool

	SayHello() string

	DiscoverryServices(ctx context.Context, serviceName string) ([]interface{}, error)
}
