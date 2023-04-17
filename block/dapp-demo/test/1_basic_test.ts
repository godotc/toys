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
			for (let i = 0; i < 18; ++i) {
				amount += "0";
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


			expect(
				await contract.connect(owner).setRoleAdmin(
					await contract.DEFAULT_ADMIN_ROLE(),
					keccak256(
						toUtf8Bytes("UNKNOW")
					)
				)).to.be.
				revertedWithPanic();

			expect(await contract.connect(owner).setRoleAdmin(
				await contract.PAUSER_ROLE(),
				await contract.MINTER_ROLE(),
			)).to.eq(true);

		});

		//   it("Should receive and store the funds to lock", async function () {
		//     const { lock, lockedAmount } = await loadFixture(
		//       deployTokenFixture
		//     );

		//     expect(await ethers.provider.getBalance(lock.address)).to.equal(
		//       lockedAmount
		//     );
		//   });

		//   it("Should fail if the unlockTime is not in the future", async function () {
		//     // We don't use the fixture here because we want a different deployment
		//     const latestTime = await time.latest();
		//     const Lock = await ethers.getContractFactory("Lock");
		//     await expect(Lock.deploy(latestTime, { value: 1 })).to.be.revertedWith(
		//       "Unlock time should be in the future"
		//     );
		//   });
		// });

		// describe("Withdrawals", function () {
		//   describe("Validations", function () {
		//     it("Should revert with the right error if called too soon", async function () {
		//       const { lock } = await loadFixture(deployTokenFixture);

		//       await expect(lock.withdraw()).to.be.revertedWith(
		//         "You can't withdraw yet"
		//       );
		//     });

		//     it("Should revert with the right error if called from another account", async function () {
		//       const { lock, unlockTime, otherAccount } = await loadFixture(
		//         deployTokenFixture
		//       );

		//       // We can increase the time in Hardhat Network
		//       await time.increaseTo(unlockTime);

		//       // We use lock.connect() to send a transaction from another account
		//       await expect(lock.connect(otherAccount).withdraw()).to.be.revertedWith(
		//         "You aren't the owner"
		//       );
		//     });

		//     it("Shouldn't fail if the unlockTime has arrived and the owner calls it", async function () {
		//       const { lock, unlockTime } = await loadFixture(
		//         deployTokenFixture
		//       );

		//       // Transactions are sent using the first signer by default
		//       await time.increaseTo(unlockTime);

		//       await expect(lock.withdraw()).not.to.be.reverted;
		//     });
		//   });

		//   describe("Events", function () {
		//     it("Should emit an event on withdrawals", async function () {
		//       const { lock, unlockTime, lockedAmount } = await loadFixture(
		//         deployTokenFixture
		//       );

		//       await time.increaseTo(unlockTime);

		//       await expect(lock.withdraw())
		//         .to.emit(lock, "Withdrawal")
		//         .withArgs(lockedAmount, anyValue); // We accept any value as `when` arg
		//     });
		//   });

		//   describe("Transfers", function () {
		//     it("Should transfer the funds to the owner", async function () {
		//       const { lock, unlockTime, lockedAmount, owner } = await loadFixture(
		//         deployTokenFixture
		//       );

		//       await time.increaseTo(unlockTime);

		//       await expect(lock.withdraw()).to.changeEtherBalances(
		//         [owner, lock],
		//         [lockedAmount, -lockedAmount]
		//       );
		//     });
		//   });
	});
});
