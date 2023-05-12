'use client'


import {useEffect, useState} from "react"
// import TokenDapp from "./component/TokenDapp"
import Link from "next/link";
import {BrowserProvider, ethers} from "ethers";


export default function HomePage() {


	const [account, setAccount] = useState("");
	const [balance, setBalance] = useState("");
	const [provider, setProvider] = useState();
	const [chainId, setChainId] = useState(-1)

	useEffect(() => {
		if (!window.ethereum || !provider || !account) {
			return;
		}
		(provider as ethers.Provider).getBalance(account).then((result: any) => {
			console.log("new value: " + result)
			setBalance(ethers.formatEther(result) as any);
		})
	}, [account])



	function HandleConnectWallet() {
		if (!window.ethereum) {
			alert("Metamask not installed!");
			return;
		}
		if (!provider) {
			const providerweb3 = new ethers.BrowserProvider(window.ethereum);
			setProvider(providerweb3 as any)
			console.log("update provider")
		}

		const connect = async () => {
			if (provider) {
				await (provider as BrowserProvider).send("eth_requestAccounts", [])
					.then((accounts: any) => {
						setAccount(accounts[0])
					})
			}
		}
		connect()

	}

	const init = async () => {
		const chainId = await window.ethereum.request({method: 'eth_chainId'});
		setChainId(chainId)

		window.ethereum.on('chainChanged', chainId => {
			console.log("change to new chain, id: " + chainId)
			window.location.reload();
		})

		window.ethereum.on('accountsChanged', accounts => {
			if (accounts.length === 0) {
				console.log('Please connect to MetaMask.');
			} else if (accounts[0] !== account) {
				console.log("account change to new: ", accounts[0]);
				setAccount(accounts[0]);
			}
		})
	}
	init()



	return (
		<div>

			<div className="homeContent">

				<h4>Account = {account}</h4>
				<h4>Balances = {balance}</h4>
				<h4>ChainId = {chainId}</h4>
				<button onClick={HandleConnectWallet} >Connect</button>

				<ul >
					<li> <Link href={"/pages/NFT"}> NFT </Link > </li>
					<li> <Link href={"/pages/ipfs"}> IPFS </Link > </li>
				</ul>


			</div>

		</div>
	)

}

