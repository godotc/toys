package store

import (
	"testing"

	"github.com/ethereum/go-ethereum/accounts/abi/bind"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/ethclient"
)

const (
	constract_addr string = ""
)

func TestUseGen(t *testing.T) {

	conn, err := ethclient.Dial("http://localhost:8545")
	if err != nil {
		panic(err)
	}

	store, err := NewStore(common.HexToAddress(constract_addr), conn)
	if err != nil {
		panic(err)
	}

	store.Version(&bind.CallOpts{
		Pending: false,
	})

	thx, err := store.SetItem(
		&bind.TransactOpts{
			From: common.HexToAddress("0x01"),
		},
		[32]byte(common.Hex2Bytes("0x01")),
		[32]byte(common.Hex2Bytes("0x01")),
	)
	if err != nil {
		panic(err)
	}

	println(thx.Hash().String())

}
