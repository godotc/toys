


import express from 'express'

import * as user from './src/user.js'


const app = express()
const port = 8080


app.use(express.json())

app.post("/user/login", user.login)


app.listen(port, () => {
    console.log(`Listenning on ${port}...`);
})