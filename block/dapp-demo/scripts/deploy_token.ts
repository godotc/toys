import {ethers, upgrades} from "hardhat";

async function main() {
	const Token = await ethers.getContractFactory("Token");

	const instnace = await upgrades.deployProxy(Token);
	await instnace.deployed();

	const contract_address = instnace.address;

	console.log(`deployed address: ${instnace.address}`);
}

// We recommend this pattern to be able to use async/await everywhere
// and properly handle errors.
main().catch((error) => {
	console.error(error);
	process.exitCode = 1;
});

