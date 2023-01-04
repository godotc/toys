package main

import (
	"flag"
	"log"
	"net/http"
	"sync"

	"github.com/gorilla/mux"
)

var addr = flag.String("addr", "localhost:8080", "http server address")
var house sync.Map
var roomMutexs = make(map[string]*sync.Mutex)

// Locak for operator of add roomMutex
var mutexForRoomMutex = new(sync.Mutex)

func main() {
	flag.Parse()

	r := mux.NewRouter()
	r.HandleFunc("/{room}", serveHome)
	r.HandleFunc("/ws/{room}", func(w http.ResponseWriter, r *http.Request) {
		vars := mux.Vars(r)
		roomId := vars["room"]

		mutexForRoomMutex.Lock()
		roomMutex, ok := roomMutexs[roomId]
		if ok {
			roomMutex.Lock()
		} else {
			roomMutexs[roomId] = new(sync.Mutex)
			roomMutexs[roomId].Lock()
		}
		mutexForRoomMutex.Lock()

		var hub *Hub

		room, ok := house.Load(roomId)
		if ok {
			hub = room.(*Hub)
		} else {
			hub = newHub(roomId)
			//house[roomId] = hub
			house.Store(roomId, hub)
			go hub.run()
		}
		serveWs(hub, w, r)
	})

	err := http.ListenAndServe(*addr, r)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}

}

func serveHome(w http.ResponseWriter, r *http.Request) {
	log.Println(r.URL)
	// if r.URL.Path != "/" {
	// 	http.Error(w, "The page Not found", http.StatusNotFound)
	// 	return
	// }
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}
	http.ServeFile(w, r, "index.html")
}

func serveWs(hub *Hub, w http.ResponseWriter, r *http.Request) {
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println("upgrade: ", err)
		return
	}

	client := &Client{
		hub:  hub,
		conn: conn,
		send: make(chan []byte, 256),
	}
	//client.hub.register <- client
	client.hub.clients[client] = true
	roomMutexs[hub.roomId].Unlock()

	// clent read and write loop goroutine
	go client.writePump()
	go client.readPump()

}
