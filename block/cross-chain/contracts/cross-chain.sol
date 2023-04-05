// SPDX-License-Identifier: MIT
pragma solidity ^0.8.4;

import "@openzeppelin/contracts-upgradeable/token/ERC20/ERC20Upgradeable.sol";
import "@openzeppelin/contracts-upgradeable/proxy/utils/Initializable.sol";
import "@openzeppelin/contracts-upgradeable/proxy/utils/UUPSUpgradeable.sol";
import "@openzeppelin/contracts-upgradeable/crosschain/CrossChainEnabledUpgradeable.sol";
import "@openzeppelin/contracts-upgradeable/access/AccessControlCrossChainUpgradeable.sol";

abstract contract MyTokenCrossChain is
    Initializable,
    ERC20Upgradeable,
    UUPSUpgradeable,
    CrossChainEnabledUpgradeable,
    AccessControlCrossChainUpgradeable
{
    bytes32 public constant MINTER_ROLE = keccak256("MINT_ROLE");
    bytes32 public constant UPGRADER_ROLE = keccak256("UPGRADER_ROLE");

    // crosschain disabled
    // modifier onlyOwner() {
    //     require(owner == _msgSender(), "Not authorized");
    //     _;
    // }

    /// @custom:oz-upgrades-unsafe-allow constructor
    constructor() initializer {}

    function initialize(address initialOwner) public initializer {
        __ERC20_init("MyToken", "MTK");
        __UUPSUpgradeable_init();

        __AccessControl_init();
        _grantRole(_crossChainRoleAlias(DEFAULT_ADMIN_ROLE), initialOwner); // initialOwner on remote(different) chain
        _grantRole(_crossChainRoleAlias(MINTER_ROLE), initialOwner); // initialOwner on remote(different) chain

        mint(_msgSender(), 123456789 * 10 ** decimals());
    }

    function mint(address to, uint256 amount) public onlyRole(MINTER_ROLE) {
        _mint(to, amount);
    }

    function _authorizeUpgrade(address newImplementation) internal override onlyRole(UPGRADER_ROLE) {}
}

import "@openzeppelin/contracts-upgradeable/crosschain/amb/CrossChainEnabledAMBUpgradeable.sol";

// Token on `xDai` <-- govenor on `mainnet`
contract TOKEN_XDAI is MyTokenCrossChain, CrossChainEnabledAMBUpgradeable(0x25ace71c97B33Cc4729CF772ae268934F7ab5fA1) {

}

import "@openzeppelin/contracts-upgradeable/crosschain/optimism/CrossChainEnabledOptimismUpgradeable.sol";

// Token on `mainnet` <-- govenor on `optimism`
contract TOKEN_Optimisim is MyTokenCrossChain, CrossChainEnabledOptimismUpgradeable(0x25ace71c97B33Cc4729CF772ae268934F7ab5fA1) {

}
