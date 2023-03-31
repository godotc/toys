// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

contract owned {
    address owner;

    constructor() {
        owner = msg.sender;
    }

    modifier onlyOwner() {
        require(
            msg.sender == owner,
            "only the contract owner can call this func"
        );
        _;
    }
}

contract mortal is owned {
    function destroy() public onlyOwner {
        selfdestruct(owner);
    }
}

contract Faucet is mortal {
    event Withdrawal(address indexed to, uint256 amount);
    event Desposit(address indexed from, uint256 amount);

    function Withdraw(uint256 WithdrawalAmount) public {
        require(WithdrawalAmount <= 0.1 ether);
        require(
            address(this).balance >= WithdrawalAmount,
            "Insufficient balance in faucet for withdrawal request"
        );

        msg.sender.transfer(WithdrawalAmount);
        emit Withdrawal(msg.sender, WithdrawalAmount);
    }

    function() external payable {
        emit Desposit(msg.sender, msg.value);
    }
}
