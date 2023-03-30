package main

import (
	"context"
	"fmt"
	"grpc/pb"
	"grpc/pkg/etc"

	"google.golang.org/grpc"
	"google.golang.org/grpc/grpclog"
)

func main() {
	conn, err := grpc.Dial(etc.Host, grpc.WithInsecure())
	if err != nil {
		grpclog.Fatalln(err)
	}

	defer conn.Close()

	c := pb.NewHelloClient(conn)

	req := &pb.HelloRequest{Name: "grpc"}
	resp, err := c.SayHello(context.Background(), req)
	if err != nil {
		grpclog.Fatalln(err)
	}

	fmt.Println(resp.Message)

	req2 := &pb.HiRequest{
		Name:   "grpc",
		Grade:  3,
		Age:    18,
		Status: 2,
		School: "chuanshang",
	}
	resp2, err := c.SayHi(context.Background(), req2)

	if err != nil {
		grpclog.Fatalln(err)
	}

	fmt.Println(resp2.Message)
	fmt.Println(resp2.Status)

}
