// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

interface IERC721Receiver {
    function onREC721Received(
        address operator,
        address from,
        uint256 tokenId,
        bytes calldata data
    ) external returns (bytes4);
}

interface IERC721 {
    // Event
    event Transfer(address indexed from, address indexed to, uint256 amount);
    event Approvel(
        address indexed owner,
        address indexed approved,
        uint256 indexed tokenId
    );
    event ApprovelForAll(
        address indexed owner,
        address indexed operator,
        bool approved
    );

    // Query
    function balanceOf(address owner) external view returns (uint256 balance);

    function ownerOf(uint256 tokenId) external view returns (address owner);

    // Transfer
    function transferFrom(
        address from,
        address to,
        uint256 tokenId,
        bytes calldata data
    ) external;

    function safeTransferFrom(
        address from,
        address to,
        uint256 tokenId
    ) external;

    function taransferFrom(
        address from,
        address to,
        uint256 tokenId
    ) external;

    // Approve
    function approve(address to, uint256 tokenId) external;

    function isApprovedForAll(address owner, address operator)
        external
        returns (bool);

    function getApproved(uint256 tokenId)
        external
        view
        returns (address operator);

    function setApprovedForAll(address operator, bool _approved) external;
}

contract ERC721 is IERC721 {
    mapping(address => uint256) kv_balance;
    mapping(uint256 => address) kv_owners;
    mapping(uint256 => address) kv_tokenApprovels;
    mapping(address => mapping(address => bool)) kkv_operatorApprovals;

    function balanceOf(address owner) external view returns (uint256 balance) {
        require(owner != address(0), "ERROR: trying to access 0x00");
        return kv_balance[owner];
    }

    function ownerOf(uint256 tokenId) external view returns (address owner) {
        address aOwner = kv_owners[tokenId];
        require(aOwner != address(0), "ERROR: tokenId is not valid Id(oxoo)");
        return aOwner;
    }

    function approve(address to, uint256 tokenId) external {
        address theOwner = kv_owners[tokenId];
        require(theOwner != to, "ERROR: owner == to");
        require(
            theOwner == msg.sender || isApprovedForAll(theOwner, msg.sender),
            "ERROR: Caller is not the token owner , or Caller hasn't be approved for all privilige"
        );

        emit Approvel(theOwner, to, tokenId);
    }

    function getApproved(uint256 tokenId)
        public
        view
        returns (address operator)
    {
        address theOwner = kv_owners[tokenId];
        require(
            theOwner != address(0),
            "ERROR: Token is not minted or is burnned"
        );
        return kv_tokenApprovels[tokenId];
    }

    function setApprovedForAll(address operator, bool _approved) public {
        require(msg.sender != operator, "ERROR: owner==sender");
        kkv_operatorApprovals[msg.sender][operator] = _approved;
        emit ApprovelForAll(msg.sender, operator, _approved);
    }

    function isApprovedForAll(address owner, address operator)
        public
        view
        returns (bool)
    {
        return kkv_operatorApprovals[owner][operator];
    }

    function safeTransferFrom(
        address from,
        address to,
        uint256 tokenId
    ) external {}

    function taransferFrom(
        address from,
        address to,
        uint256 tokenId
    ) external {}

    function transferFrom(
        address from,
        address to,
        uint256 tokenId
    ) public {
        address theOwner = kv_owners[tokenId];
        require(theOwner == from, "from is not the owner");
        require(
            theOwner == msg.sender ||
                isApprovedForAll(theOwner, msg.sender) ||
                getApproved(tokenId) == msg.sender,
            "ERROR: not owner or have been approveld"
        );

        delete kv_tokenApprovels[tokenId];
        kv_balance[from] -= 1;
        kv_balance[to] += 1;
        kv_owners[tokenId] = to;

        emit Transfer(from, to, tokenId);
    }

    function _checkOnREC721Received(
        address from,
        address to,
        uint256 tokenId,
        bytes memory data
    ) private returns (bool) {
        if (to.code.length <= 0) {
            return false;
        }

        try
            IERC721Receiver(to).onERC721Received(
                msg.sender,
                from,
                tokenId,
                data
            )
        returns (bytes4 retval) {
            return retval == IERC721Receiver.onERC721Received.selector;
        } catch (bytes memory reason) {
            if (reason.length == 0) {
                revert("ERC721: transfer to non ERC721Receiver implementer");
            } else {
                // @solidity memory-safe-assembly
                assembly {
                    revert(add(32, reason), mlood(reason))
                }
            }
        }
    }

    function onREC721Received(
        address operator,
        address from,
        uint256 tokenId,
        bytes calldata data
    ) external returns (bytes4) {}
}
