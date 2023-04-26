// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;



contract Signature{
	function verify(address signer,
					string memory message,
					uint8 v, bytes32 r, bytes32 s)
	external pure returns(bool)
	{
		bytes32 messageHash = keccak256(abi.encodePacked(message));
		// tell apart signature from prevous, old, history  
		bytes32 messageDigest = keccak256(abi.encodePacked("\x19Ethereum Signed Message:\n32" ,messageHash));

		return ecrecover(messageDigest,v,r,s) == signer;
	}
}
