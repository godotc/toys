import "@nomicfoundation/hardhat-toolbox"
import { ethers } from "hardhat";
import { expect } from "chai";

describe("encode call test", () => {

    async function init() {
        const ContractA = await ethers.getContractFactory("A");
        const ContractB = await ethers.getContractFactory("B");

        const A = await ContractA.deploy();
        const B = await ContractB.deploy();

        await A.deployed()
        await B.deployed()

        const [owner, otherAccounts] = await ethers.getSigners();

        return { A, B, owner, otherAccounts };
    }

    it("Should call by delegate and encode with signature", async () => {

        let { A, B, owner, otherAccounts } = await init();

        // console.log(await B.num())
        // console.log(await B.message())

        await A.callBFunction(B.address, BigInt(50), "hello world")

        expect(await B.num()).to.eq(50)
        expect(await B.message()).to.eq("hello world")
        // console.log(await B.num())
        // console.log(await B.message())

    });

})

