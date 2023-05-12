
import "./style.css"

import Header from "./header"


export const metadata = {
	title: 'home',
	description: 'the home page',
}

export default function RootLayout({children, }: {children: React.ReactNode}) {
	return (
		< html lang="en" >
			<body>
				<Header />
				<main>
					{children}
				</main>
			</body>
		</html >
	)
}
