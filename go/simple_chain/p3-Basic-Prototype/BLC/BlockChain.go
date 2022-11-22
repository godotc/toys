package BLC

type BlockChain struct {
	Blocks []*Block
}

// ! 创建带有Genesi BLock 的区块链
func InitBLC() *BlockChain {
	genesisBlock := CreateGenesisBlock("Geneis Data......")

	blockchain := &BlockChain{
		Blocks: []*Block{genesisBlock},
	}

	return blockchain
}
