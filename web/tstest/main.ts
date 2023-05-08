import {PrismaClient} from '@prisma/client'


const prisma = new PrismaClient()

async function main() {

	const user = await prisma.user.create({
		data: {
			name: 'godot',
			email: "godot42@proton.me",
		},
	})
	console.log(user)
}


main().
	then(async () => {
		await prisma.$disconnect()
	})
	.catch(async (e) => {
		console.log(e)
		await prisma.$disconnect()
		process.exit(-1)
	})
