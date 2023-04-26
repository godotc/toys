'use client'

import {useEffect, useState} from "react"

export default function Header() {

	const [color, setColor] = useState('blue')

	useEffect(() => setColor('red'), [])

	return (
		<>
			<header>
				<div className="Header" align="middle">
					<h1 > Civil3 Dapp Demo </h1>
					<marker>------</marker>
				</div>
			</header>
		</>
	)
}
