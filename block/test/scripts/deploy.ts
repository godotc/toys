import {ethers} from "hardhat";

async function main() {

	const Contract = await ethers.getContractFactory("Lock");
	const contract = await Contract.deploy();

	await contract.deployed();

	console.log(`deployed to ${contract.address}`);
}

// We recommend this pattern to be able to use async/await everywhere
// and properly handle errors.
main().catch((error) => {
	console.error(error);
	process.exitCode = 1;
});
