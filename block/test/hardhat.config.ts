import {HardhatUserConfig} from "hardhat/config";
import "@nomicfoundation/hardhat-toolbox";

const config: HardhatUserConfig = {
	solidity: "0.8.19",


	networks: {
		ganache: {
			url: "127.0.0.1:8545"
		}
	}
};

export default config;
