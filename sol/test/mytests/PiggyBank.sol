// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

contract PiggyBank {
    address public owner;
    event Create(address owner, uint256 value);
    event Receive(address indexed sender, uint256 value);
    event Withdraw(address owner, uint256 value);

    modifier onlyOwner() {
        require(owner == msg.sender, "Ony owner cant call this");
        _;
    }

    constructor() payable {
        owner = msg.sender;
        emit Create(owner, msg.value);
    }

    receive() external payable {
        emit Receive(msg.sender, msg.value);
    }

    function withdraw() external onlyOwner {
        address payable Receiver = payable(msg.sender);
        uint256 value = address(this).balance;
        Receiver.transfer(value);
        emit Withdraw(Receiver, value);
    }
}
