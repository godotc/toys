package socks5

import (
	"bytes"
	"reflect"
	"testing"
)

func TestNewClientAuthMessage(t *testing.T) {
	t.Run("should generate a message", func(t *testing.T) {
		b := []byte{SOCKS_VERISON_5, 2, 0x00, 0x01}
		r := bytes.NewReader(b)

		message, err := NewClientAuthMessage(r)

		if err != nil {
			t.Fatalf("want error == nil, but got %s", err)
		}
		if message.Version != SOCKS_VERISON_5 {
			t.Fatalf("want socksVersion5, but got %d", message.Version)
		}
		if message.N_Methods != 2 {
			t.Fatalf("want 2 methods, but got %d", message.N_Methods)
		}
		if !reflect.DeepEqual(message.Methods, []byte{0x00, 0x01}) {
			t.Fatalf("want methods: %v, but got %s", []byte{0x00, 0x01}, message.Methods)
		}
	})

	t.Run("methods length is shorter than methods", func(t *testing.T) {
		b := []byte{SOCKS_VERISON_5, 2, 0x00, 0x01}
		r := bytes.NewReader(b)

		_, err := NewClientAuthMessage(r)
		if err != nil {
			t.Fatalf("want error == nil, but got %s", err)
		}
	})
}
