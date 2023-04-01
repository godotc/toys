import '@nomiclabs/hardhat-ethers';

async function main() {
	const Token = await ethers.getContractFactory("Token")
	const token = await Token.deploy();

	await token.deployed();
	console.log("Contract addresss: ", token.address)
}


main()
	.then(() => process.exit(0))
	.catch(error => {
		console.log(error);
		process.exit(-1);
	});
