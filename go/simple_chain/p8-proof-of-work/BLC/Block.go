package BLC

import (
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
	//! 工作量证明
	Nonce int64
}

// ! Create the ORIGINAL block
func CreateGenesisBlock(data string) *Block {
	return NewBlock(
		data, []byte{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0},
		1)
}

// ! Create New Block
func NewBlock(data string, preBlockHash []byte, height int64) *Block {
	block := &Block{
		Data:         []byte(data),
		PreBlockHash: preBlockHash,
		Height:       height,
		Timestamp:    time.Now().Unix(),
		Hash:         nil,
	}

	// Call proof of work func , return validational [Hash] and [Nonce]
	pow := NewProofOfWork(block)

	// Mining validation
	hash, nonce := pow.Run()

	block.Hash = hash[:]
	block.Nonce = nonce

	return block
}
