


import express from 'express'
import swaggerUI from 'swagger-ui-express'
import swaggerJSDoc from 'swagger-jsdoc'

import swaggerOptions from './swagger.js'

import * as user from './src/user.js'


const app = express()
const port = 8080

const specs = swaggerJSDoc(swaggerOptions)
app.use('/docs', swaggerUI.serve, swaggerUI.setup(specs))

app.use(express.json())

app.post("/api/user/login", user.login)
app.post("/api/user/register", user.register)


app.listen(port, () => {
    console.log(`Listenning on ${port}...`);
})