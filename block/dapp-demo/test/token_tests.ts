import {loadFixture} from "@nomicfoundation/hardhat-network-helpers";
import {expect} from "chai";
import {ethers, upgrades} from "hardhat";
import {keccak256, toUtf8Bytes} from "ethers/lib/utils";

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

			await expect(contract.connect(owner).pause()).to.be.not.reverted;
			await expect(contract.connect(owner).transfer(otherAccount.address, 500)).to.be.reverted;//With("reverted with reason string 'Pausable: paused'");
			await expect(contract.connect(owner).transferFrom(owner.address, otherAccount.address, 500)).to.be.reverted;

			await expect(contract.connect(owner).approve(otherAccount.address, 500)).to.be.not.reverted;
			await expect(contract.connect(owner).transferFrom(owner.address, otherAccount.address, 500)).to.be.reverted;


			await expect(contract.connect(owner).unpause()).to.be.not.reverted;
			await expect(contract.connect(owner).transfer(otherAccount.address, 500)).to.not.be.reverted;

			/*
			 The `transferFrom` will spend the allowance  of  [from -> msgSender]
			 Then transfer [from -> to]
			 So we need to confirm from has approve this contract enough allowance

			expect(await contract.connect(owner).approve(contract.address, 500)).to.be.not.reverted; // WRONG
			expect(await contract.connect(owner).approve(otherAccount.address, 500)).to.be.not.reverted; // WRONG
			*/

			// I declare I  approve to myself amount, so I can call this myself ( why I have to approve allowance to myself? ), or native contract?
			await expect(contract.connect(owner).approve(owner.address, 500)).to.be.not.reverted;
			await expect(contract.connect(owner).transferFrom(owner.address, otherAccount.address, 500)).to.not.be.reverted;

			await expect(contract.connect(owner).approve(owner.address, 500)).to.be.not.reverted;
			await expect(contract.transferFrom(owner.address, otherAccount.address, 500)).to.not.be.reverted;

			// This will be the normal way how to use `transferFrom`
			expect(await contract.connect(owner).approve(otherAccount.address, 500)).to.be.not.reverted;
			expect(await contract.connect(otherAccount).transferFrom(owner.address, otherAccount.address, 500)).to.not.be.reverted;
		});
		it("Should failed to mint by not Minter", async () => {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);
			await expect(contract.connect(otherAccount).mint(otherAccount.address, 100)).to.be.reverted;
		})
		it("Should failed to pause by not Pausable", async () => {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);
			await expect(contract.connect(otherAccount).pause()).to.be.reverted;
		})
		it("Should failed to snapshot by not SNAPSHOT_ROLE", async () => {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);
			await expect(contract.connect(otherAccount).snapshot()).to.be.reverted;
		})
		it("Should failed to `setRoleAdmin` by not DEFAULT_ADMIN_ROLE", async () => {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);
			await expect(contract.connect(otherAccount).setRoleAdmin(
				contract.MINTER_ROLE(), contract.DEFAULT_ADMIN_ROLE()
			)).to.be.reverted;
			await expect(contract.connect(owner).setRoleAdmin(
				contract.MINTER_ROLE(), contract.DEFAULT_ADMIN_ROLE()
			)).to.not.be.reverted;
		})
		it("Shoule failed to setRoleAdmin even after grant ADMIN or MINTER ROLE to someone", async () => {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			await expect(contract.connect(owner).grantRole(contract.MINTER_ROLE(), otherAccount.address)).to.not.be.reverted;
			await expect(contract.connect(otherAccount).setRoleAdmin(contract.MINTER_ROLE(), contract.DEFAULT_ADMIN_ROLE())).to.be.reverted;

		})
		it("Shoule success to setRoleAdmin after grant ADMIN  ROLE to someone", async () => {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			await expect(contract.connect(owner).grantRole(contract.DEFAULT_ADMIN_ROLE(), otherAccount.address)).to.not.be.reverted;
			await expect(contract.connect(otherAccount).setRoleAdmin(contract.MINTER_ROLE(), contract.DEFAULT_ADMIN_ROLE())).not.to.be.reverted;

			await expect(contract.connect(owner).grantRole(contract.MINTER_ROLE(), otherAccount.address)).to.not.be.reverted;
			await expect(contract.connect(otherAccount).setRoleAdmin(contract.PAUSER_ROLE(), contract.MINTER_ROLE())).not.to.be.reverted;
		})
		it("Shoule success to mint by other who granted role", async () => {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			await expect(contract.connect(owner).grantRole(contract.MINTER_ROLE(), otherAccount.address)).to.not.be.reverted;
			await expect(contract.connect(otherAccount).mint(otherAccount.address, 12345)).to.not.be.reverted;
		})
	})


	describe("Events", function () {
		it("Should emit an event `Transfer` on transfer", async function () {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			expect(await contract.connect(owner).transfer(otherAccount.address, 123)).
				to.emit(contract, "Transfer")
				.withArgs(owner.address, otherAccount.address, 123);

			// Why default transfer token from owner here?
			console.log("\t\t", await contract.balanceOf(owner.address));
			expect(await contract.transfer(otherAccount.address, 500)).to.emit(contract, "Transfer").withArgs(owner.address, otherAccount.address, 500);
			console.log("\t\t", await contract.balanceOf(owner.address));

		});

		it("Should emit event Approval with right args ", async function () {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			await expect(contract.connect(owner).approve(otherAccount.address, 123)).
				emit(contract, "Approval").
				withArgs(owner.address, otherAccount.address, 123);
			await expect(contract.connect(owner).approve(contract.address, 123)).
				emit(contract, "Approval").
				withArgs(owner.address, contract.address, 123);
			await expect(contract.connect(otherAccount).approve(contract.address, 123)).
				emit(contract, "Approval").
				withArgs(otherAccount.address, contract.address, 123);
		});

		it("Should emit an event `Transfer` on transferFrom", async function () {

			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			await expect(contract.transferFrom(owner.address, otherAccount.address, 123)).
				to.be.revertedWith("ERC20: insufficient allowance")


			await expect(contract.connect(owner).approve(otherAccount.address, 123)).to.not.be.reverted;
			await expect(contract.connect(otherAccount).transferFrom(owner.address, otherAccount.address, 123)).
				to.emit(contract, "Transfer")
				.withArgs(owner.address, otherAccount.address, 123);
		});
	});

	describe("Transfers", function () {
		it("Should transfer the right amounts", async function () {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			await expect(contract.connect(owner).transfer(otherAccount.address, 500)).
				to.changeTokenBalances(
					contract,
					[owner, otherAccount],
					[-500, 500]
				);

		});
	});

	describe("Approve", function () {
		it("Should spend  the right allowance", async function () {
			const {contract, owner, otherAccount} = await loadFixture(deployTokenFixture);

			await contract.connect(owner).approve(otherAccount.address, 200);
			const before = await contract.allowance(owner.address, otherAccount.address);
			expect(before).to.eq(200);

			await (contract.connect(otherAccount).transferFrom(owner.address, otherAccount.address, 100))
			const after = await contract.allowance(owner.address, otherAccount.address);
			expect(after).to.eq(100);


		});
	})
});
