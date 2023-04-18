import {HardhatUserConfig} from "hardhat/config";
import '@openzeppelin/hardhat-upgrades';
import "@nomicfoundation/hardhat-toolbox";

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
	}
};

export default config;
