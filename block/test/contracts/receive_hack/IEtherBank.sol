// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

interface IEtherBank{
	function deposit()external payable;
	function withdraw() external;
}
