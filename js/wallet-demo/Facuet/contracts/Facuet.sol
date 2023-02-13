// SPDX-License-Identifier: MIT
pragma solidity ^0.8.1;

contract Owner {
    address m_Owner;

    constructor() {
        m_Owner = msg.sender;
    }

    modifier onlyOwner() {
        require(
            msg.sender == m_Owner,
            "only the contract owner can call this function"
        );
        _;
    }
}

contract Mortal is Owner {
    function destory() public onlyOwner {
        selfdestruct();
    }
}
