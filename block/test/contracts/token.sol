// SPDX-License-Identifier: MIT
pragma solidity ^0.8.9;

import "@openzeppelin/contracts/token/ERC20/IERC20.sol";
import "@openzeppelin/contracts/token/ERC20/ERC20.sol";
import "@openzeppelin/contracts/security/Pausable.sol";
import "@openzeppelin/contracts/access/AccessControl.sol";

contract Token is ERC20, Pausable, AccessControl {
    bytes32 public constant PAUSER_ROLE = keccak256("PAUSER_ROLE");
    bytes32 public constant MINTER_ROLE = keccak256("MINTER_ROLE");

    constructor() ERC20("Token", "TK") {
        _grantRole(DEFAULT_ADMIN_ROLE, msg.sender); // 管理员
        _grantRole(PAUSER_ROLE, msg.sender); // 异常暂停合约
        _grantRole(MINTER_ROLE, msg.sender); // 铸造者

		mint(msg.sender,123456789*10**decimals());
    }

    function pause() public onlyRole(PAUSER_ROLE) {
        _pause();
    }

    function unpause() public onlyRole(PAUSER_ROLE) {
        _unpause();
    }

    function mint(address to, uint256 amount) public onlyRole(MINTER_ROLE) {
        _mint(to, amount);
    }

    function _beforeTokenTransfer(address from, address to, uint256 amount)
        internal
        whenNotPaused
        override
    {
        super._beforeTokenTransfer(from, to, amount);
    }
}

contract DEX{
	event Brought(address indexed who, uint256 indexed amount);
	event Sold(address indexed who, uint256 indexed amount);

	IERC20 public _token;


	constructor(){
		_token = new Token();
	}


	function buy() payable public{
		uint256 amountToBuy = msg.value*100; // 1 ether == 100 TK
		uint256 dexBalance =getDexBalance();

		require(amountToBuy>0, "you need to send ether");
		require(amountToBuy<=dexBalance, "Not enough tokens in this DEX that reserved");

		_token.transfer(msg.sender, amountToBuy);
		emit Brought(msg.sender, amountToBuy);
	}

	function sell(uint256 amount) payable public{
		require(amount>0, "Token amount you sold is 0");

		uint256 allowance = _token.allowance(msg.sender, address(this));
		require(allowance>=amount, "Not enough allowance for this DEX");

		_token.transferFrom(msg.sender,address(this), amount);

		payable(msg.sender).transfer(1 ether);

		emit Sold(msg.sender, amount);
	}

	function aapprove(uint256 amount) public returns(bool){
		bool isApprove = _token.approve(address(this),amount);
		return isApprove;
	}


	function getDexBalance() public view returns(uint256){ return _token.balanceOf(address(this)); }
	function getTotalsupply() public view returns(uint256) {return _token.totalSupply();}
	function getSenderAddress() public view returns(address) {return address(msg.sender);}

	function getAllowance() public view returns(uint256) {
		uint256 allowance = _token.allowance(msg.sender, address(this));
		return allowance;
	}

}
