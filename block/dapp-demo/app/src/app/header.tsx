'use client'

import {useEffect, useState} from "react"

export default function Header() {

	const [color, setColor] = useState('blue')

	//useEffect(() => setColor('red'), [])

	return (
		<div className="HeaderBar" >
			<h1 color={color}> Civil3 Dapp Demo </h1>
			<marker>------</marker>
		</div>
	)
}
