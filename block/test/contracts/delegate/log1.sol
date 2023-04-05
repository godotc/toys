// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

contract logic01 {
    uint private number;

    function set(uint num_) public {
        number += num_ + 2;
    }

    function get() public view returns (uint) {
        return number;
    }
}
