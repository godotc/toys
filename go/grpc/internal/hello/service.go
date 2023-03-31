package hello

import (
	"context"
	"fmt"
	"grpc/pb"
)

type helloService struct{}

var HelloService helloService

func (h helloService) SayHello(ctx context.Context, in *pb.HelloRequest) (*pb.HelloResponse, error) {
	resp := &pb.HelloResponse{}
	resp.Message = fmt.Sprintf("Hello %s.", in.Name)

	return resp, nil
}

func (h helloService) SayHi(ctx context.Context, in *pb.HiRequest) (*pb.HiResponse, error) {
	resp := &pb.HiResponse{}
	resp.Message = fmt.Sprintf("Hi %s, grade = %d, age=%d, school=%s,  status=%d",
		in.Name, in.Grade, in.Age, in.School, in.Status)
	resp.Status = 200

	return resp, nil
}
