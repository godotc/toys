
import "@nomicfoundation/hardhat-toolbox"
import {ethers} from "hardhat";
import {expect} from "chai";

describe("Signature", () => {

	it("Verify", async () => {
		const [owner] = await ethers.getSigners();

		const Contract = await ethers.getContractFactory("Signature");
		const contract = await Contract.deploy();
		await contract.deployed();

		console.log(`contract address: ${contract.address}`);

		const message = "Test Signature";
		const msgHash = ethers.utils.solidityKeccak256(["string"], [message]);
		const msgHashByte = ethers.utils.arrayify(msgHash);

		const sig = await owner.signMessage(msgHashByte);

		const sig_V_R_S = ethers.utils.splitSignature(sig);

		expect(await contract.verify(owner.address, message, sig_V_R_S.v, sig_V_R_S.r, sig_V_R_S.s)).to.eq(true);
	});

});
