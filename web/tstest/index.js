const express = require('express')
const app = express()
const port = 3000

app.get('/', (req, resp) => {
	resp.send("hello world!")
})


app.listen(port, () => {
	console.log(`Listen on port ${port}`);
})
