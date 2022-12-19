// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

contract Hello {
    string public name;

    constructor(string memory _name) {
        name = _name;
    }

    function getName() public view returns (string memory) {
        return name;
    }

    function setName(string memory _name) public {
        name = _name;
    }
}
