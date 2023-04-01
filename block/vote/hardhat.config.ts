/** @type import('hardhat/config').HardhatUserConfig */


require("@nomiclabs/hardhat-ethers")
require('dotenv').config();

const {API_URL, PK} = process.env;

export const solidity = "0.8.17";

export const defaultNetwork = "sepolia";

export const networks = {
	sepolia: {
		url: API_URL,
		accounts: [`0x${PK}`]

	},
};

