// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

contract A {
    function callBFunction(address address_, uint256 num_, string memory message_) public returns (bool) {
        (bool success, ) = address_.call(abi.encodeWithSignature("bFunction(uint256,string)", num_, message_));
        return success;
    }

    function callBFunction__sig(address address_, uint256 num_, string memory message_) public returns (bool) {
        bytes4 sig = bytes4(keccak256("bFunction(uint256,string)"));
        bytes memory _num = abi.encode(num_);
        bytes memory _message = abi.encode(message_);

        (bool success, ) = address_.call(abi.encodePacked(sig, _num, _message));
        return success;
    }

    function callBFunction__selsector(address address_, uint256 num_, string memory message_) public returns (bool) {
        bytes4 sig = bytes4(keccak256("bFunction(uint256,string)"));
        (bool success, ) = address_.call(abi.encodeWithSelector(sig, num_, message_));
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
