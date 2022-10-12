package socks5

import (
	"errors"
	"io"
)

type Method = byte

type ClientAuthMessage struct {
	Version   byte
	N_Methods byte
	Methods   []Method
}

func NewClientAuthMessage(conn io.Reader) (*ClientAuthMessage, error) {
	// Read version and nMethods
	buf := make([]byte, 2)
	_, err := io.ReadFull(conn, buf)
	if err != nil {
		return nil, err
	}

	// Validation version
	if buf[0] != SOCKS_VERISON_5 {
		return nil, errors.New("protol version not supported")
	}

	// Get methods
	n_methods := buf[1]
	buf = make([]byte, n_methods)
	_, err = io.ReadFull(conn, buf)
	if err != nil {
		return nil, err
	}

	return &ClientAuthMessage{
		Version:   SOCKS_VERISON_5,
		N_Methods: n_methods,
		Methods:   buf,
	}, nil

}
