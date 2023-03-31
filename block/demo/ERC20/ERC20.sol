// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;
import "@openzeppelin/contracts/interfaces/IERC165.sol";

interface IERC20 {

    event Transfer(address indexed from, address indexed to, uint256 value);
    event Approval(
        address indexed owner,
        address indexed spender,
        uint256 value
    );

    function totalSupply() external view returns (uint256);

    function balanceOf(address account) external view returns (uint256);

    function allowance(address owner, address spender)
        external
        view
        returns (uint256);

    function approvel(address spender, uint256 amount) external returns (bool);

    function transfer(address to, uint256 amount) external returns (bool);

    function transferFrom(
        address from,
        address to,
        uint256 amount
    ) external returns (bool);
}

contract ERC20 is IERC20, IERC165 {
    address owner;

    uint256 _totalSupply;
    mapping(address => uint256) _balance;
    mapping(address => mapping(address => uint256)) _allowance;

    string _name;
    string _symbol;

    modifier onlyOwner() {
        require(owner == msg.sender, "ERROR: only owner");
        _;
    }

    constructor(string memory theName, string memory theSymbol) {
        owner = msg.sender;

        _balance[msg.sender] = 100000;
        _totalSupply = 100000;

        _name = theName;
        _symbol = theSymbol;
    }

    // Create coin
    function mint(address account, uint256 amount) public onlyOwner {
        require(account != address(0), "ERROR: mint for the addrex 0x00");
        _totalSupply += amount;
        _balance[account] += amount;
        emit Transfer(address(0), account, amount);
    }

    // Destroy
    function burn(address account, uint256 amount) public onlyOwner {
        require(account != address(0), "ERROR: burnning for the addrex 0x00");
        _totalSupply -= amount;
        uint256 accountBalance = _balance[account];
        require(
            accountBalance >= amount,
            "ERROR: not enough coin to burn on this address"
        );

        _balance[account] = accountBalance - amount;
        emit Transfer(account, address(0), amount);
    }

    function name() public view returns (string memory) {
        return _name;
    }

    function symbol() public view returns (string memory) {
        return _symbol;
    }

    function decimals() public pure returns (uint8) {
        return 18;
    }

    function totalSupply() external view returns (uint256) {
        return _totalSupply;
    }

    function balanceOf(address account) external view returns (uint256) {
        return _balance[account];
    }

    function allowance(address owner_, address spender)
        external
        view
        returns (uint256)
    {
        return _allowance[owner_][spender];
    }

    function approvel(address spender, uint256 amount) external returns (bool) {
        _allowance[msg.sender][spender] = amount;
        emit Approval(msg.sender, spender, amount);
        return true;
    }

    function _transfer(
        address from,
        address to,
        uint256 amount
    ) public {
        require(_balance[from] >= amount, "No enough money");
        require(to != address(0), "Trying transfer to address 0 ");

        _balance[from] -= amount;
        _balance[to] += amount;

        emit Transfer(from, to, amount);
    }

    function transfer(address to, uint256 amount) external returns (bool) {
        _transfer(msg.sender, to, amount);
        return true;
    }

    function transferFrom(
        address from,
        address to,
        uint256 amount
    ) external returns (bool) {
        // Check the allowence(额度) from bank
        require(
            _allowance[from][msg.sender] >= amount,
            "ERROR: m_allowance < amount"
        );
        _allowance[from][msg.sender] -= amount;
        emit Approval(from, msg.sender, _allowance[from][msg.sender] - amount);

        // Difinetly tranfer
        _transfer(from, to, amount);
        return true;
    }

    // Check if is the availiable interface
    function supportsInterface(bytes4 interfaceId)
        public
        view
        virtual
        override
        returns (bool)
    {
        return
            interfaceId == type(IERC20).interfaceId ||
            interfaceId == type(IERC165).interfaceId;
    }
}
