var fs = require('fs')
var Web3 = require('web3')


var web3 = new Web3("http://127.0.0.1:8545")

combine = fs.readFileSync("./optout/combined.json")
//combine = fs.readFileSync("./out/Store_meta.json")
data = JSON.parse(combine)

var abi = (data.contracts['store.sol:Store'].abi)
//console.log(abi)
var bin = (data.contracts['store.sol:Store'].bin)

var contract = new web3.eth.Contract(abi)
//console.log(contract)

contract.deploy({
    data: bin,
    arguments: ["1.0"]
}).send({
    from: "0x3b0fDd25DAff2c4D91Bf8EF1439D92817ed54Ec9",
    // gas: 150000,
    // gasPrice: 5000000
}).on("recepit", (recepit) => {
    fs.writeFileSync("./deploy/recepit.json", recepit)
}).on("transactionHash", (hash) => {
    fs.writeFileSyne("./deploy/transactionHash.json", hash)
}).on('confirmation', (confirmationNumber, recipt) => {
    fs.writeFileSyne("./deploy/confim.json", confirmationNumber + "\n" + recepit)
})




