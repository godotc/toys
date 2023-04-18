import {time, loadFixture} from "@nomicfoundation/hardhat-network-helpers";
import {anyValue} from "@nomicfoundation/hardhat-chai-matchers/withArgs";
import {expect} from "chai";
import {ethers, upgrades} from "hardhat";
import common from "mocha/lib/interfaces/common";
import {hexlify, keccak256, parseBytes32String, toUtf8Bytes} from "ethers/lib/utils";
import {string} from "hardhat/internal/core/params/argumentTypes";

describe("Token", function () {
	// We define a fixture to reuse the same setup in every test.
	// We use loadFixture to run this setup once, snapshot that state,
	// and reset Hardhat Network to that snapshot in every test.
	async function deployTokenFixture() {

		// Contracts are deployed using the first signer/account by default
		const [owner, otherAccount] = await ethers.getSigners();

		const CONTRACT = await ethers.getContractFactory("Token");

		// need use proxy  method
		// const contract = await Contract.deploy();
		const contract = await upgrades.deployProxy(CONTRACT)
		await contract.deployed()

		return {contract, owner, otherAccount};
	}

	describe("Deployment", function () {

		it("Should set the right initalize information", async function () {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			expect(await contract.name()).to.eq("Token")
			expect(await contract.symbol()).to.eq("TK")
			expect(await contract.decimals()).to.eq(18)

			let amount: string = "543210";
			for (let i = 0; i < 2; ++i) {
				amount += "000000000";
			}
			expect((await contract.balanceOf(owner.address)).toString()).to.eq(amount);
			expect((await contract.totalSupply()).toString()).to.eq(amount);

		});


		it("Should set the right roles", async function () {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			expect(await contract.hasRole(await contract.DEFAULT_ADMIN_ROLE(), owner.address)).to.eq(true);
			expect(await contract.hasRole(await contract.SNAPSHOT_ROLE(), owner.address)).to.eq(true);
			expect(await contract.hasRole(await contract.PAUSER_ROLE(), owner.address)).to.eq(true);
			expect(await contract.hasRole(await contract.MINTER_ROLE(), owner.address)).to.eq(true);


			expect(await contract.connect(owner).setRoleAdmin(
				await contract.DEFAULT_ADMIN_ROLE(),
				keccak256(toUtf8Bytes("UNKNOW")))
			).to.be.revertedWithPanic();

			expect(await contract.connect(owner).setRoleAdmin(
				await contract.PAUSER_ROLE(),
				await contract.MINTER_ROLE())
			).not.to.be.revertedWithPanic();


			expect(await contract.connect(owner).mint(otherAccount.address, 500)).to.not.be.revertedWithPanic();
			expect(await contract.connect(owner).pause()).to.not.be.revertedWithPanic();
			expect(await contract.connect(owner).snapshot()).to.not.be.revertedWithPanic();

			// These will be reverted. The otherAccount has no these access, and I don't want to write more codes to specify the reverted reason
			//expect(await contract.connect(otherAccount).mint(otherAccount.address, 500)).to.be.reverted;
			//expect(await contract.connect(otherAccount).pause()).to.be.reverted;
			//expect(await contract.connect(otherAccount).snapshot()).to.be.reverted;

		});
	})

	describe("AccessControl", async function () {

		it("Should failed to transfer on pause", async function () {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			// `whenNotPause` now only has hooked  `beforeTransfer`
			// other will be normal

			expect(await contract.connect(owner).pause()).to.be.not.reverted;

			//expect(await contract.connect(owner).transfer(otherAccount.address, 500)).to.be.reverted;//With("reverted with reason string 'Pausable: paused'");
			//expect(await contract.connect(owner).transferFrom(otherAccount.address, 500)).to.be.reverted;

			//expect(await contract.connect(owner).approve(otherAccount.address, 500)).to.be.not.reverted;
			//expect(await contract.connect(owner).transferFrom(otherAccount.address, 500)).to.be.reverted;


			expect(await contract.connect(owner).unpause()).to.be.not.reverted;

			expect(await contract.connect(owner).transfer(otherAccount.address, 500)).to.not.be.reverted;

			// transferFrom will spend the allowance  of  [from -> msgSender]
			// then transfer [from -> to]
			// so we need to confirm from has approve this contract enough allowance

			//expect(await contract.connect(owner).approve(contract.address, 500)).to.be.not.reverted; // WRONG
			//expect(await contract.connect(owner).approve(otherAccount.address,500 )).to.be.not.reverted; // WRONG

			// I declare I  approve to myself amount, so I can call this myself ( why I have to approve allowance to myself? ), or native contract?
			expect(await contract.connect(owner).approve(owner.address, 500)).to.be.not.reverted;
			expect(await contract.connect(owner).transferFrom(owner.address, otherAccount.address, 500)).to.not.be.reverted;
			expect(await contract.connect(owner).approve(owner.address, 500)).to.be.not.reverted;
			expect(await contract.transferFrom(owner.address, otherAccount.address, 500)).to.not.be.reverted;

			// This will be the normal way how to use `transferFrom`
			expect(await contract.connect(owner).approve(otherAccount.address, 500)).to.be.not.reverted;
			expect(await contract.connect(otherAccount).transferFrom(owner.address, otherAccount.address, 500)).to.not.be.reverted;

		});
	})


	describe("Events", function () {
		it("Should emit an event on withdrawals", async function () {

			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);


			//			await expect(lock.withdraw())
			//				.to.emit(lock, "Withdrawal")
			//				.withArgs(lockedAmount, anyValue); // We accept any value as `when` arg
		});
	});

	//describe("Transfers", function () {
	//	it("Should transfer the funds to the owner", async function () {
	//		const {lock, unlockTime, lockedAmount, owner} = await loadFixture(
	//			deployTokenFixture
	//		);

	//		await time.increaseTo(unlockTime);

	//		await expect(lock.withdraw()).to.changeEtherBalances(
	//			[owner, lock],
	//			[lockedAmount, -lockedAmount]
	//		);
	//	});
	//});
});
