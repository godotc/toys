
const {log} = require('console');
const express = require('express')
const http = require('http')
const {Server} = require('socket.io')

const PORT = 3000;

const app = express();
const server = http.createServer(app);
const io = new Server(server)


app.get('/', (req, res) => {
	res.sendFile(__dirname + '/index.html');
})

io.on('connection', (socket) => {
	log('a user connected');
	socket.broadcast.emit('hi')

	socket.
		on('disconnect', () => {
			console.log('user disconnected');
			io.emit('chat message', {
				name: '',
				msg: 'a user disconnected'
			});
		})
		.on('chat message', (msg) => {
			log(msg);
			io.emit('chat message', msg);
		})

})


server.listen(PORT, () => {
	log(`listening on *:${PORT}`);
});

