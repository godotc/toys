package main

import (
	"fmt"
	"grpc/internal/hello"
	"grpc/pb"
	"grpc/pkg/etc"
	"net"

	"google.golang.org/grpc"
	"google.golang.org/grpc/grpclog"
)

func main() {
	listen, err := net.Listen("tcp", etc.Host)
	if err != nil {
		grpclog.Fatal("Failed to listen: %v", err)
	}

	s := grpc.NewServer()

	pb.RegisterHelloServer(s, hello.HelloService)
	fmt.Println("Listen on ", etc.Host)
	grpclog.Println("Listen on ", etc.Host)

	s.Serve(listen)

}
