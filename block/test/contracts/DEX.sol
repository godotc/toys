// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

// Dencentralied Exchange

import "@openzeppelin/contracts/token/ERC20/IERC20.sol";
import "@openzeppelin/contracts/token/ERC20/ERC20.sol";
import "@openzeppelin/contracts/utils/math/SafeMath.sol";

contract DEX {
    using SafeMath for uint;
    address public token1;
    address public token2;

    constructor(address token1_, address token2_) {
        token1 = token1_;
        token2 = token2_;
    }

    function swap(address from, address to, uint amount) public {
        require(((from == token1 && to == token2) || (to == token1 && from == token2)), "Invalid tokens");
        require(IERC20(from).balanceOf(msg.sender) >= amount, "Not enough to swap");

        uint swap_amount = get_swap_price(from, to, amount);

        IERC20(from).transferFrom(msg.sender, address(this), amount);
        IERC20(to).approve(address(this), swap_amount);
        IERC20(to).transferFrom(address(this), msg.sender, swap_amount);
    }

    function add_liquidty(address token_address, uint amount) public {
		require(token_address==token1||token_address==token2, "Wrong tokens");
        IERC20(token_address).transferFrom(msg.sender, address(this), amount);
    }

    function get_swap_price(address from, address to, uint amount) public view returns (uint) {
        // sold amount * ratio of 2 tokens this DEX owned
        return (amount * IERC20(to).balanceOf(address(this))) / IERC20(from).balanceOf(address(this));
    }

    function approve(address token_, uint amount_) public {
        IERC20(token_).approve(address(this), amount_);
    }
}
