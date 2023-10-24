
import express from 'express'
import path from 'path'

const app = express()
const port = 3000


app.use('/public', express.static('public'))

app.get("/", (req, resp) => {
    resp.send({ msg: "Hello world" })
})

app.get("/arch", (x, y) => {
    y.send({ url: '/public/arch.png' })
})


app.listen(port, () => {
    console.log(`Listen in ${port}...`)
})