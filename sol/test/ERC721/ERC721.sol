// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

interface IERC721Receiver {
    function onERC721Received(
        address operator,
        address from,
        uint256 tokenId, 
        bytes calldata data
    ) external returns (bytes4);
}

interface IERC721Metadata{
    function name() external view returns(string memory);
    function token() external view returns(string memory);
    function tokenURI(uint256 tokenId) external view returns(string memory);
}

interface IERC165 {
    function supportsInterface(bytes4 interfaceId) external view returns(bool);
}

interface IERC721 {
    // Event
    event Transfer(address indexed from, address indexed to, uint256 amount);
    event Approvel(address indexed owner,address indexed approved,uint256 indexed tokenId );
    event ApprovelForAll( address indexed owner,  address indexed operator,  bool approved);

    // Query
    function balanceOf(address owner) external view returns (uint256 balance);
    function ownerOf(uint256 tokenId) external view returns (address owner);

    // Transfer
    function safetransferFrom(address from,address to, uint256 tokenId, bytes calldata data ) external;
    function safeTransferFrom(address from,address to,uint256 tokenId) external;
    function transferFrom(address from, address to,uint256 tokenId) external;

    // Approve
    function approve(address to, uint256 tokenId) external;
    function isApprovedForAll(address owner, address operator) external returns (bool);
    function getApproved(uint256 tokenId) external  view  returns (address operator);
    function setApprovedForAll(address operator, bool _approved) external;

    // Mint
    function mint(address to, uint256 tokenId) external;
    function safeMite( address to, uint256 tokenId, bytes memory data)external;
    function safeMite( address to, uint256 tokenId)external;
   
    // Burn
    function burn(address from, uint256 tokenId) external;
}

contract ERC721 is IERC721, IERC721Metadata, IERC165{
    mapping(address => uint256) kv_balance;
    mapping(uint256 => address) kv_owners;
    mapping(uint256 => address) kv_tokenApprovels;
    mapping(address => mapping(address => bool)) kkv_operatorApprovals;

    string _name;
    string _symbol;
    mapping(uint256 => string) _tokenURIS;

    constructor( string memory name_, string memory symbol_){
        _name = name_;
        _symbol = symbol_;
    }
    function name() public view returns(string memory){
        return _name;
    }
    function token() public view returns(string memory){
        return _symbol;
    }
    function tokenURI(uint256 tokenId) public view returns(string memory){
        address owner = kv_owners [tokenId];
        require(owner != address(0), "ERROR: toekn id is not valud");
        return _tokenURIS[tokenId];
    }

    function setURI(uint256 tokenId, string memory URI)public {
        _tokenURIS[tokenId] = URI;
    }

   function supportsInterface(bytes4 interfaceId) public pure returns(bool){
       return interfaceId == type(IERC721).interfaceId ||
        interfaceId == type(IERC165).interfaceId ||
        interfaceId == type(IERC721).interfaceId;
   }

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

    function getApproved(uint256 tokenId)public view returns (address operator)
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

    function isApprovedForAll(address owner, address operator)public view returns (bool) {
        return kkv_operatorApprovals[owner][operator];
    }

    function safetransferFrom(address from,address to, uint256 tokenId, bytes calldata data ) public{
        _safeTransferFrom(from ,to , tokenId, data);
    }

    function safeTransferFrom(address from,address to,uint256 tokenId) public{
        _safeTransferFrom(from, to , tokenId, "");
    }
    function _safeTransferFrom(address from , address to, uint256 tokenId, bytes memory data) internal{
        _transfer(from, to, tokenId);
        require(_checkOnREC721Received(from, to , tokenId, data), "ERROR: ERC721Receiver is not implented");
    }

    function transferFrom(address from,address to, uint256 tokenId) public {
        _transfer(from ,to  , tokenId);
    }

    function _transfer(address from, address to , uint256 tokenId) internal {
        address theOwner = kv_owners[tokenId];
        require(theOwner == from, "from is not the owner");
        require(theOwner == msg.sender ||isApprovedForAll(theOwner, msg.sender) || getApproved(tokenId) == msg.sender,
            "ERROR: not owner or have been approveld"
        );

        delete kv_tokenApprovels[tokenId];
        kv_balance[from] -= 1;
        kv_balance[to] += 1;
        kv_owners[tokenId] = to;

        emit Transfer(from, to, tokenId);
    }


    function _checkOnREC721Received(address from, address to,uint256 tokenId, bytes memory data) private returns (bool) {
        if (to.code.length <= 0) {
            return false;
        }

        try IERC721Receiver(to).onERC721Received(msg.sender,from,tokenId, data ) returns (bytes4 retval) {
            return retval == IERC721Receiver.onERC721Received.selector;
        } catch (bytes memory reason) {
            if (reason.length == 0) {
                revert("ERC721: transfer to non ERC721Receiver implementer");
            } else {
                // @solidity memory-safe-assembly
                assembly {
                    revert(add(32, reason), mload(reason))
                }
            }
        }
    }

    // Mint
    function mint(address to, uint256 tokenId) public{
        require(to!= address(0), "ERROR: Miting  for 0xxx");
        address theOwner = kv_owners[tokenId];
        require(theOwner == address(0), "ERROR: token has exist");
        kv_balance[to] +=1;
        kv_owners[tokenId] = to;
        emit Transfer(address(0), to , tokenId);
        
    }
    function safeMite( address to, uint256 tokenId, bytes memory data) public{
        mint(to, tokenId);
        require(_checkOnREC721Received(address(0), to , tokenId, data), "ERROR: ERC721Receiver is not implented");
    }
    function safeMite( address to, uint256 tokenId) public{
        safeMite( to , tokenId, "");
    }
   
    // Burn
    function burn(address from, uint256 tokenId) public{
        address aowner = kv_owners[tokenId];
        require(msg.sender == kv_owners[tokenId],"ERROR: only owner can burn");
        kv_balance[kv_owners[tokenId]] -=1;
        kkv_operatorApprovals[aowner][from] = false;
        delete kv_owners[tokenId];
        delete kv_tokenApprovels[tokenId];
        emit Transfer(aowner, address(0), tokenId);    
    }
}
