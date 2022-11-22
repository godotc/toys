package main

import (
	"fmt"
	"github.com/godotc/simple-chain/p4-Basic-Prototype/BLC"
)

func main() {

	OriginBLC := BLC.InitBlockChain()

	fmt.Println("-- Geneis Block Chain:", *OriginBLC)
	fmt.Println("-- Geneis Block:", OriginBLC.Blocks[0])

	AddNewBlock(OriginBLC, "Send 100 btc to godot42's pocket")

	fmt.Println("-- after add one blc :", *OriginBLC)
	fmt.Println("-- new block:", OriginBLC.Blocks[len(OriginBLC.Blocks)-1])

}

func AddNewBlock(blc *BLC.BlockChain, data string) {
	blc.AddNewBlock(
		data,
		blc.Blocks[len(blc.Blocks)-1].Height+1,
		blc.Blocks[len(blc.Blocks)-1].Hash)
}
