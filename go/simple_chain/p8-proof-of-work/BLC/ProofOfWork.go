package BLC

import (
	"bytes"
	"crypto/sha256"
	"fmt"
	"math/big"
)

// ! 256 bit HASH need 16 of zero behind
const targetBit = 16

type ProofOfWork struct {
	//!Curretnt need to be validted  block
	Block *Block
	//!Big data contain
	target *big.Int
}

// ! Crete new POF obj
func NewProofOfWork(block *Block) *ProofOfWork {

	// ahead of 16 of 0
	target := big.NewInt(1)
	target = target.Lsh(target, 256-targetBit)

	return &ProofOfWork{
		Block:  block,
		target: target,
	}

}

func (pow *ProofOfWork) Run() ([]byte, int64) {

	//1. Attach block properties to byte array
	//2. genreate hash
	//3. check validation of hash, if not --> loop
	nonce := 0
	var hashInt big.Int // store per round genreate's [hash]
	var hashb [32]byte
	for {
		dataBytes := pow.prepareData(int64(nonce))
		hashb = sha256.Sum256(dataBytes)

		fmt.Printf("\r%x\n", hashb)

		hashInt.SetBytes(hashb[:])

		if pow.target.Cmp(&hashInt) == 1 {
			break
		}

		nonce += 1
	}

	return hashb[:], int64(nonce)
}

func (pow *ProofOfWork) prepareData(nonce int64) []byte {
	data := bytes.Join(
		[][]byte{
			pow.Block.PreBlockHash,
			pow.Block.Data,
			IntToHex(pow.Block.Timestamp),
			IntToHex(int64(targetBit)),
			IntToHex(int64(nonce)),
			IntToHex(int64(pow.Block.Height)),
		},
		[]byte{},
	)

	return data
}
