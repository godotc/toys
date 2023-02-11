// SPDX-License-Identifier: MIT
pragma solidity ^0.8.1;

contract owned {
    address owner;

    constructor() {
        owner = msg.sender;
    }

    modifier onlyOwner() {
        require(
            msg.sender == owner,
            "only the contract owner can call this function"
        );
        _;
    }
}
