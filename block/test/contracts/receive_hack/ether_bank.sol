// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

import "hardhat/console.sol";
import "@openzeppelin/contracts/security/ReentrancyGuard.sol";
import "./IEtherBank.sol";

contract Bank is IEtherBank,ReentrancyGuard {

    event Receive(address indexed from, uint256 indexed amount);
    event Deposit(address indexed from, uint256 indexed amount);

	mapping(address => uint) public balances;

    function _msgSender() internal view returns (address) {
        return msg.sender;
    }

    address public immutable owner;

    constructor() {
        owner = _msgSender();
    }

    receive() external payable {
        emit Receive(_msgSender(), msg.value);
    }

    function deposit() external payable override{
		balances[msg.sender] += msg.value;
        emit Receive(_msgSender(), msg.value);
    }

    function withdraw() external 
	// nonReentry
	{
		console.log	("begin withdraw...");
		require(balances[_msgSender()]>0, "empty ether");

		console.log	("");
		console.log	("EtherBank balance: ",address(this).balance);
		console.log	("Attacker balance: ",balances[_msgSender()]);
		console.log	("");


        payable(_msgSender()).transfer(balances[_msgSender()]);
		console.log	("");
		console.log	("");
		console.log	("");
		console.log	("");
		console.log	("");
		console.log	("");
		console.log	("");
		balances[_msgSender()]=0;

		console.log("end withdraw");
    }

	function getBalance()external view returns(uint){
		return address(this).balance;
	}

}
