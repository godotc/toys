import {ethers, upgrades} from "hardhat";
import * as fs from "fs"
import * as path from "path"
import {log} from "console";
import {BigNumber} from "ethers";

async function main() {
	const Token = await ethers.getContractFactory("Civil3");
	const instnace = await upgrades.deployProxy(Token);
	await instnace.deployed();
	console.log(`Civil3 deployed address: ${instnace.address}`);

	const Facuet = await ethers.getContractFactory("Facuet");
	const facuet_instance = await Facuet.deploy(instnace.address);
	await facuet_instance.deployed();
	console.log(`Facuet deployed address: ${facuet_instance.address}`);

	fs.mkdir(__dirname + "/../Info", () => {});
	const file = path.join(__dirname + "/../Info/info.json");

	const [account, userB] = await ethers.getSigners();
	log(`user address: ${account.address}`);

	const Text = {
		owner: account.address,
		contracts: {
			token_address: instnace.address,
			facuet_address: facuet_instance.address,
		}
	}


	//log(instnace.interface);

	const Proxy = Token.attach(instnace.address);
	await Proxy.connect(account).transfer(facuet_instance.address, ethers.utils.parseEther("1000")).then((tx) => {
		log(tx.hash);
	});


	log("UserB balacne:", await Proxy.balanceOf(userB.address));
	await facuet_instance.connect(userB).getOneBucket().then(async (tx) => {
		log(tx.hash)
		log("UserB balacne:", await Proxy.balanceOf(userB.address));
	})


	fs.writeFile(file, JSON.stringify(Text), (err) => {
		if (err) console.log(err);
	});

}

// We recommend this pattern to be able to use async/await everywhere
// and properly handle errors.
main().catch((error) => {
	console.error(error);
	process.exitCode = 1;
});

