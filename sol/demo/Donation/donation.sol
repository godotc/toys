// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

contract Donation{
    address public owner;
    mapping (address => uint256) donationList;

    event Donate(address indexed sender, uint256 value);
    event Withdraw(address indexed owner, uint256 value);

    modifier onlyOwner(){
        require(owner == msg.sender, "Only owner can call this");
        _;
    }

    constructor(){
        owner = msg.sender;
    }

    function donate() public payable{
        donationList[msg.sender] += msg.value;
        emit Donate(msg.sender, msg.value);
    }

    // Query total donate amount
    function getHistory() public view returns(uint256){
        return donationList[msg.sender];
    }

    // Query donation level
    function getLevel() public view returns(string memory){
        if(donationList[msg.sender] > 10 ether){
            return "UR";
        }else if(donationList[msg.sender] > 5 ether){
            return "SR";
        }else if(donationList[msg.sender] > 1 ether){
            return "R";
        }else if(donationList[msg.sender] > 0 ether){
            return "N";
        }else  {
            return "None";
        }
    }

    // get eth
    function withdraw() onlyOwner public{
        address payable receiver = payable(owner);
        uint256 value = address(this).balance;
        receiver.transfer(value);
        emit Withdraw(receiver, value);

    }
}