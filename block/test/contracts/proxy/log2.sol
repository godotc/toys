// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

contract logic02 {
    uint private number;

    function set(uint num_) public {
        number = num_;
    }

    function get() public view returns (uint) {
        return number;
    }
}
