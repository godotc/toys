package main

import (
	"fmt"
	"github.com/godotc/simple-chain/p3-Basic-Prototype/BLC"
)

func main() {

	genesisBlockChain := BLC.InitBLC()

	fmt.Println("-- Geneis Block Chain:", *genesisBlockChain)
	fmt.Println("-- Geneis Block:", genesisBlockChain.Blocks[0])
}
