import {ethers, upgrades} from "hardhat";
import * as fs from "fs"
import * as path from "path"
import {log} from "console";

async function main() {
	const [account, userB] = await ethers.getSigners();
	log(`user address: ${account.address}`);

	const Token = await ethers.getContractFactory("Civil3");
	const instnace = await upgrades.deployProxy(Token);
	await instnace.deployed();
	console.log(`Civil3 deployed address: ${instnace.address}`);

	const Facuet = await ethers.getContractFactory("Facuet");
	const facuet_instance = await Facuet.deploy(instnace.address, account.address);
	await facuet_instance.deployed();
	console.log(`Facuet deployed address: ${facuet_instance.address}`);


	// Wirte info 
	const token_abi = require("../artifacts/contracts/Demo/ERC20.sol/Civil3.json")["abi"];
	const facuet_abi = require("../artifacts/contracts/Demo/Facuet.sol/Facuet.json")["abi"];
	const Info = {
		token: {
			address: instnace.address,
			signer: await instnace.signer.getAddress(),
			abi: token_abi,
		},
		facuet: {
			address: facuet_instance.address,
			signer: await facuet_instance.signer.getAddress(),
			abi: facuet_abi,
		}
	}
	RecordeInfo(Info);



	const Proxy = Token.attach(instnace.address);
	await Proxy.connect(account).approve(facuet_instance.address, ethers.utils.parseEther("10000")).then((tx) => {
		log(tx.hash);
	});

	log("allowance:", await Proxy.allowance(account.address, facuet_instance.address));
	log("UserB balacne:", await Proxy.balanceOf(userB.address));

	await facuet_instance.connect(userB).getOneBucket().then(async (tx) => {
		log(tx.hash)
	})

	log("UserB balacne:", await Proxy.balanceOf(userB.address));
	log("allowance:", await Proxy.allowance(account.address, facuet_instance.address));


}


const RecordeInfo = (Info: any) => {
	const output_dir = __dirname + "/../Info/"

	fs.mkdir(output_dir, (err) => {log(err)});
	const file = path.join(output_dir + "info.json");

	fs.writeFile(file, JSON.stringify(Info), (err) => {
		if (err) console.log(err);
	});
}

// We recommend this pattern to be able to use async/await everywhere
// and properly handle errors.
main().catch((error) => {
	console.error(error);
	process.exitCode = 1;
});

