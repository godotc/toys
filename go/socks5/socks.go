package socks5

import (
	"fmt"
	"log"
	"net"
)

type Server interface {
	Run() error
}

type SOCKS5Server struct {
	IP   string
	Port int
}

func (s *SOCKS5Server) Run() error {
	address := fmt.Sprintf("%s:%d", s.IP, s.Port)

	listener, err := net.Listen("tcp", address)
	if err != nil {
		return err
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("Connection failure from %s: %s", conn.RemoteAddr(), err)
			continue
		}

		go func() {
			defer conn.Close()
			err := handleConnection(conn)
			if err != nil {
				log.Printf("Handle connection failure from %s: %s", conn.RemoteAddr(), err)
			}
		}()

	}
}

func handleConnection(conn net.Conn) error {
	// 协商过程
	if err := auth(conn); err != nil {
		return err
	}
	// 请求过程
	// 转发过程
	return nil
}

// func auth(conn net.Conn) error {
// 	clientAuthMessage, err := NewClientAuthMessage(conn)
// 	if err != nil {
// 		return err
// 	}
// 	//log.Println(clientAuthMessage.Version, clientAuthMessage.N_Methods, clientAuthMessage.Methods)

// 	// Only support "no auth" and

// 	return nil
// }
