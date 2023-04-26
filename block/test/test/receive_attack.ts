
import "@nomicfoundation/hardhat-toolbox"
import {ethers} from "hardhat";
import {expect} from "chai";
import {BigNumber} from "ethers";

describe("Hack", () => {

	it("Receive", async () => {
		const [owner, other] = await ethers.getSigners();

		const Bank = await ethers.getContractFactory("Bank");
		const Attacker = await ethers.getContractFactory("Attacker");

		const bank = await Bank.connect(other).deploy();
		await bank.deployed();

		await bank.connect(other).deposit({value: 10000});
		console.log(bank.balances(other.address));

		const bank_address = bank.address;

		const attacker = await Attacker.connect(owner).deploy(bank_address);
		await attacker.deployed();

		attacker.connect(owner).attack({value: 1000});

	});

});
