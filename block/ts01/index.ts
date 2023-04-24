
let message: string = "Hello world";
console.log(message)

import {ethers} from "ethers"

async function main() {
	const provider = new ethers.WebSocketProvider("wss://sepolia.infura.io/ws/v3/19cd45d3b29b4b558bb0c86031515a54")
	//provider.on("event", async (tx: any) => {
	//	console.log(tx)
	//	const txDetail = provider.getTransaction(tx)
	//})

	provider.on("block", async (block: any) => {
		console.log(block)
		const b = await provider.getBlock(block)
		if (b !== null) {
			const txs = b.transactions
			console.log(txs)
		}
	})
}

main()
