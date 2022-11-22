package BLC

import (
	"bytes"
	"crypto/sha256"
	"fmt"
	"strconv"
	"time"
)

type Block struct {
	//! chunk height 区块高度
	Height int64
	//! 上一个区块hash
	PreBlockHash []byte
	//! 交易数据
	Data []byte
	//！时间戳
	Timestamp int64
	//! HASH
	Hash []byte
}

// ! Create the ORIGINAL block
func CreateGenesisBlock(data string) *Block {
	return NewBlock(
		data, []byte{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0},
		1, nil)
}

// ! Create New Block
func NewBlock(data string, preBlockHash []byte, height int64, hash []byte) *Block {
	block := &Block{
		Data:         []byte(data),
		PreBlockHash: preBlockHash,
		Height:       height,
		Timestamp:    time.Now().Unix(),
		Hash:         hash,
	}
	block.SetHash()

	return block
}

func (b *Block) SetHash() {
	// 1. convet b.Height to byte array
	heightBytes := IntToHex(b.Height)
	fmt.Println("height:", heightBytes)

	// 2. conver Timestamp to bye arrary
	timeString := strconv.FormatInt(b.Timestamp, 2) // 2 进制
	timeBytes := []byte(timeString)
	fmt.Println("time", timeBytes)

	// 3. attach all properties
	blockBytes := bytes.Join([][]byte{b.PreBlockHash, heightBytes, timeBytes, b.Data, b.Hash}, []byte{})
	fmt.Println("block bytes:", blockBytes)

	// 4. generate hash
	hash := sha256.Sum256(blockBytes)
	fmt.Println("hash:", hash)

	b.Hash = hash[:32]
	fmt.Println("block.Hash:", hash)

}
