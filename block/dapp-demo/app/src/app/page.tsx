'use client'


import { ethers } from "ethers";
import { useEffect, useState } from "react"
// import TokenDapp from "./component/TokenDapp"
import Link from "next/link";
import Header from "./header";

export default function HomePage() {

	const [account, setAccount] = useState();
	const [balance, setBalance] = useState();
	const [provider, setProvider] = useState();


	useEffect(() => {
		if (!window.ethereum || !provider || !account) {
			return;
		}
		(provider as ethers.providers.Web3Provider).getBalance(account).then((result) => {
			setBalance(ethers.utils.formatEther(result) as any);
		})
	}, [account])


	function HandleConnectWallet() {
		if (!window.ethereum) {
			alert("Metamask not installed!");
			return;
		}
		if (!provider) {
			const providerweb3 = new ethers.providers.Web3Provider(window.ethereum as ethers.providers.ExternalProvider)
			setProvider(providerweb3 as any)
		}

		const connect = async () => {
			if (provider) {
				const accounts = (provider as ethers.providers.Web3Provider).send("eth_requestAccounts", [])
					.then((accounts) => {
						setAccount(accounts[0])

					})
			}
		}
		connect()

		if (provider != null) {
			(provider as ethers.providers.Web3Provider).on("accountsChange", (accounts) => {
				console.log("new: ", accounts[0])
				setAccount(accounts[0] as any)
			})
		} else console.log("unalbe to listen event")
	}

	return (
		<>
			<Header></Header>

			<Link href={"/pages/ipfs"}>IPFS</Link>

			<div className="Container">

				{/* <TokenDapp /> */}

				<h5>Account = {account}</h5>
				<h5>Balances = {balance}</h5>
				<button onClick={HandleConnectWallet} >Connect</button>

				<ul >
					<li> <Link href={"/NFT"}> NFT </Link > </li>
					<li> <Link href={"/pages/ipfs"}> IPFS </Link > </li>
				</ul>


			</div>

		</>
	)

}

