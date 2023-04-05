// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

interface ILogic {
    function set(uint num_) external;

    function get() external view returns (uint);
}

contract Proxy {
    ILogic public logic;

    function setLogicAddress(address logicAddress_) public {
        logic = ILogic(logicAddress_);
    }

    function getLogicAddress() public view returns (address) {
        return address(logic);
    }

    function set(uint num_) public {
        logic.set(num_);
    }

    function get() public view returns (uint) {
        return logic.get();
    }
}
