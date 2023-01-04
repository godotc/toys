package main

import (
	"fmt"
)

type Hub struct {
	// Identity of room
	roomId string
	// Register clients
	clients map[*Client]bool
	// Inbound messages from clients
	broadcast chan []byte

	// Register requests from clients
	register chan *Client
	// Unregister request from clients
	unregister chan *Client
}

func newHub(roomId string) *Hub {
	return &Hub{
		roomId:     roomId,
		clients:    make(map[*Client]bool),
		register:   make(chan *Client),
		unregister: make(chan *Client),
		broadcast:  make(chan []byte),
	}
}

func (h *Hub) run() {
	// Close chann when Hub close
	defer func() {
		close(h.register)
		close(h.unregister)
		close(h.broadcast)
	}()

	for {
		select {
		/* remvoe register logic to aovid deadlock */
		// case client := <-h.register:
		// 	h.clients[client] = true
		// mutex.UnLock()

		case client := <-h.unregister:

			roomMutex := roomMutexs[h.roomId]
			roomMutex.Lock()

			if _, ok := h.clients[client]; ok {
				delete(h.clients, client)
				close(client.send)

				// not conns, close this Hub and return
				if len(h.clients) == 0 {

					//delete(house, h.roomId)
					house.Delete(h.roomId)
					fmt.Println("Close the Hub of ", h.roomId)

					roomMutex.Unlock()

					mutexForRoomMutex.Lock()
					if roomMutex.TryLock() {
						if len(h.clients) == 0 {
							delete(roomMutexs, h.roomId)
						}
						roomMutexs[h.roomId].Unlock()
					}
					mutexForRoomMutex.Unlock()

					return
				}
			}
			roomMutex.Unlock()

		case message := <-h.broadcast:
			// multicast
			for Client := range h.clients {
				select {
				case Client.send <- message:

				default:
					close(Client.send)
					delete(h.clients, Client)
				}
			}
		}

	}
}
