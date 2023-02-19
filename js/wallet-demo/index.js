// const Web3 = require('web3')
// var web3 = new Web3('ws://localhost:8545')
var fs = require('fs');
var Web3 = require('web3')

var web3 = new Web3("ws://127.0.0.1:8545")

var config

try {
    const data = fs.readFileSync("./transfer_test/build/contracts/CIVIL3.json", 'utf-8');
    config = JSON.parse(data)

    //console.log(config.abi)

} catch (err) {
    console.log(`Error : ${err}`);
}


var accounts = await web3.eth.getAccounts()

var ContractAddress = "0x5369b82F18a409aC6a98C1C7ceC6F864a9815c3e";
var Owner = accounts[0];

var CIVIL3 = new web3.eth.Contract(config.abi, ContractAddress, { from: Owner });


async function MT(addr, numberstr) {
    CIVIL3.methods.mint(addr, numberstr).send({ from: Owner });
}

async function BalanceOf(addr) {
    return CIVIL3.methods.balanceOf(addr).call()
}

async function TF(from, to, ammount) {
    CIVIL3.methods.transfer(to, ammount).send({ from:  })
}
