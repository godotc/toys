package main

import (
	"fmt"
	"github.com/godotc/simple-chain/p2-Basic-Prototype/BLC"
)

func main() {

	genesisBlock := BLC.CreateGenesisBlock("Geneis Block")

	fmt.Println("Geneis Block", genesisBlock)
}
