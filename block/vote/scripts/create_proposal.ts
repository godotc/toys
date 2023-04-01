require('@nomiclabs/hardhat-ethers')

const TOKEN_ADDRESS = "0xde915663062fA96dF41Ec4E84A5F47F42F408385";
const GOVERNOR_ADDRESS = "0x64670148249301e5BEAd2a20eC9bA8a92556Ab2e";
const OWNER_ADDRESS = "0x39f972BA743f1B976E6Af13530705c05DaE94c63";

async function main() {
	const token = ethers.getContractAt('ERC20', TOKEN_ADDRESS)

	const teamAddress = OWNER_ADDRESS;
	const grantAmount = 115415;

	const transferCalldata = token.interface.encodeFunctionData('transfer', [teamAddress, grantAmount]);


}
