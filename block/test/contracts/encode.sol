// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

contract A {
    function callBFunction(address address_, uint256 num_, string memory message_) public returns (bool) {
        (bool success, ) = address_.call(abi.encodeWithSignature("bFunction(uint256,string)", num_, message_));
        return success;
    }
}

contract B {
    uint256 public num = 1;
    string public message = "bllo";

    function bFunction(uint256 num_, string memory msg_) public returns (uint256, string memory) {
        num = num_;
        message = msg_;
        return (num, message);
    }
}
