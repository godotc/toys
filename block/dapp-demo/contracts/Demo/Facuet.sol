// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

import "@openzeppelin/contracts-upgradeable/token/ERC20/ERC20Upgradeable.sol";


contract Facuet{

	ERC20Upgradeable private Civil3;
	address public Civil3Owner;

	event Extract(address indexed who, uint256 indexed amount);

	constructor(address contractAddress_, address contractOwner){
		Civil3 = ERC20Upgradeable(contractAddress_);
		Civil3Owner = contractOwner;
	}

	function getOneBucket() external {
		uint256 amount = 100 ether;
		if(Civil3.balanceOf(address(this))<amount) {
			Civil3.transferFrom(Civil3Owner,address(this), amount*10);
		}

		Civil3.transfer(msg.sender, amount);
		emit Extract(msg.sender,amount);
	}
}
