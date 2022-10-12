package main

import (
	"log"

	"github.com/godotc/socks5"
)

func main() {
	server := socks5.SOCKS5Server{
		IP:   "localhost",
		Port: 9527,
	}

	err := server.Run()
	if err != nil {
		log.Fatal(err)
	}

}
