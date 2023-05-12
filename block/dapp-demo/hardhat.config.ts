import {HardhatUserConfig} from "hardhat/config";
import '@openzeppelin/hardhat-upgrades';
import "@nomicfoundation/hardhat-toolbox";

import dotenv from "dotenv"
dotenv.config()

const config: HardhatUserConfig = {
	solidity: {
		version: "0.8.17",
		// When contract size too big
		settings: {
			optimizer: {
				enabled: true,
				runs: 1000,
			}
		},
		//	allowUnlimitedContractSize: false,
	},

	networks: {
		local: {
			url: "http://0.0.0.0:8545",
			accounts: [process.env.PK as string, process.env.PK1 as string]
		},
		sepolia: {
			url: "https://sepolia.infura.io/v3/19cd45d3b29b4b558bb0c86031515a54",
			accounts: [process.env.PK as string]
		}

	},
	etherscan: {
		apiKey: {
			sepolia: process.env.ETHERSCAN_APIKEY as string
		}
	}



};

export default config;
