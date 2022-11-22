package BLC

type BlockChain struct {
	Blocks []*Block
}

// ! 创建带有Genesi BLock 的区块链
func InitBlockChain() *BlockChain {
	genesisBlock := CreateGenesisBlock("Geneis Data......")

	blockchain := &BlockChain{
		Blocks: []*Block{genesisBlock},
	}

	return blockchain
}

func (blc *BlockChain) AddNewBlock(data string, height int64, preHash []byte) {
	newBlock := NewBlock(data, preHash, height)
	blc.Blocks = append(blc.Blocks, newBlock)
}
