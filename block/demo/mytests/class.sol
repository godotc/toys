// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

contract Class{
    mapping(uint => uint) Students;

    function update(uint id, uint score) public{
        Students[id] = score;
    }
    function get(uint id) public view returns(uint){
        return Students[id];
    }
}