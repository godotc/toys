// SPDX-License-Identifier: SEE LICENSE IN LICENSE
pragma solidity ^0.8;

contract Store {
    event ItemSet(bytes32 key, bytes32 value);

    string public version;
    mapping(bytes32 => bytes32) public items;

    constructor(string memory theVersion) {
        version = theVersion;
    }

    function SetItem(bytes32 key, bytes32 value) external {
        items[key] = value;
        emit ItemSet(key, value);
    }
}
