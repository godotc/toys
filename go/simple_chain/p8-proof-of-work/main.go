package main

import (
	"fmt"

	"github.com/godotc/simple-chain/p5-proof-of-work/BLC"
)

func main() {

	OriginBLC := BLC.InitBlockChain()

	AddNewBlock(OriginBLC, "Send 100 btc to godot42's pocket")

	fmt.Println(OriginBLC)
	fmt.Println(OriginBLC.Blocks[0])

}

func AddNewBlock(blc *BLC.BlockChain, data string) {
	blc.AddNewBlock(
		data,
		blc.Blocks[len(blc.Blocks)-1].Height+1,
		blc.Blocks[len(blc.Blocks)-1].Hash,
	)
}
