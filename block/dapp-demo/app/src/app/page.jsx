'use client'


import {ethers} from "ethers";
import {useEffect, useState} from "react"
// import TokenDapp from "./component/TokenDapp"
import Link from "next/link";

export default function HomePage() {

	const [account, setAccount] = useState();
	const [balance, setBalance] = useState();
	const [provider, setProvider] = useState();


	useEffect(() => {
		if (!window.ethereum || !provider || !account) {
			return;
		}
		provider.getBalance(account).then((result) => {
			setBalance(ethers.utils.formatEther(result));
		})
	}, [account])


	function HandleConnectWallet() {
		if (!window.ethereum) {
			alert("Metamask not installed!");
			return;
		}
		const providerweb3 = new ethers.providers.Web3Provider(window.ethereum)
		setProvider(providerweb3)

		providerweb3.send("eth_requestAccounts")
			.then((accounts) => {
				setAccount(accounts[0])
			})

		ethers.on("accountsChange", (accounts) => {
			console.log("new: ", accounts[0])
			setAccount(accounts[0])
		})
	}


	return (
		<>
			<div className="Container">
				{/* <TokenDapp /> */}
				<ul>
					<li> <Link href={"/NFT"}> NFT </Link > </li>
				</ul>
				<h5>Account = {account}</h5>
				<h5>Balances = {balance}</h5>
				<button onClick={HandleConnectWallet} >Connect</button>

			</div>

		</>
	)

}

