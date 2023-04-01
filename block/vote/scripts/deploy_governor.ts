import '@nomiclabs/hardhat-ethers';


const TOKEN_ADDRESS = "0xde915663062fA96dF41Ec4E84A5F47F42F408385";

async function main() {
	const Governor = await ethers.getContractFactory("MyGovernor")
	const governor = await Governor.deploy(TOKEN_ADDRESS);

	await governor.deployed();
	console.log("Contract addresss: ", governor.address)
}


main()
	.then(() => process.exit(0))
	.catch(error => {
		console.log(error);
		process.exit(-1);
	});
