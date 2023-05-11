// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

import "@openzeppelin/contracts-upgradeable/token/ERC20/ERC20Upgradeable.sol";


contract Facuet{
	ERC20Upgradeable Civil3;

	event Extract(address indexed who, uint256 indexed amount);

	constructor(address contractAddress_){
		Civil3 = ERC20Upgradeable(contractAddress_);
	}

	function getOneBucket() external {
		uint256 amount = 100 ether;
		Civil3.transfer(msg.sender, amount);
		emit Extract(msg.sender,amount);
	}
}
