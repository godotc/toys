package main

import (
	"bufio"
	"flag"
	"io"
	"log"
	"net/http"
	"os"
	"os/exec"
	"time"

	"github.com/gorilla/websocket"
)

var (
	addr     = flag.String("addr", "localhost:8080", "http service addres")
	upgrader = websocket.Upgrader{}
	cmdPath  string
)

const (
	maxMessageSize = 8192

	writeWait  = 20 * time.Second
	pongWait   = 60 * time.Second
	pingPeriod = (pongWait * 9) / 10

	closeGracePeriod = 10 * time.Second
)

func main() {
	flag.Parse()
	if len(flag.Args()) < 1 {
		log.Fatal("Must specify at least onee argument")
	}

	var err error
	cmdPath, err = exec.LookPath(flag.Args()[0])
	if err != nil {
		log.Fatal("exec.LookPath: ", err)
	}

	http.HandleFunc("/", serveHome)
	http.HandleFunc("/ws", serveWs)

	log.Fatal(http.ListenAndServe(*addr, nil))
}

func serveHome(w http.ResponseWriter, r *http.Request) {
	if r.URL.Path != "/" {
		http.Error(w, "Not found this path", http.StatusNotFound)
		return
	}
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}
	http.ServeFile(w, r, "index.html")
}

func serveWs(w http.ResponseWriter, r *http.Request) {
	ws, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println("upgrade: ", err)
	}
	defer ws.Close()

	// create the pipe to stdout
	outr, outw, err := os.Pipe()
	if err != nil {
		internalError(ws, "stdout: ", err)
		return
	}
	defer outr.Close()
	defer outw.Close()

	// create the stdin  pipe
	inr, inw, err := os.Pipe()
	if err != nil {
		internalError(ws, "stdin: ", err)
		return
	}
	defer inr.Close()
	defer inw.Close()

	_, err = os.StartProcess(cmdPath, flag.Args(), &os.ProcAttr{
		Files: []*os.File{inr, outw, outw},
	})

	if err != nil {
		internalError(ws, "start: ", err)
		return
	}

	inr.Close()
	outw.Close()

	stdoutDone := make(chan struct{})
	go pumpStdout(ws, outr, stdoutDone)
	go ping(ws, stdoutDone)

	pumpStdin(ws, inw)
}

func internalError(ws *websocket.Conn, msg string, err error) {
	log.Println(msg, err)
	msg = "Internal server error."
	ws.WriteMessage(websocket.TextMessage, []byte(msg))
}

func ping(ws *websocket.Conn, done chan struct{}) {
	ticker := time.NewTicker(pingPeriod)
	defer ticker.Stop()

	for {
		select {
		case <-ticker.C:
			if err := ws.WriteControl(websocket.PingMessage, []byte{}, time.Now().Add(writeWait)); err != nil {
				log.Println("ping: ", err)
			}

		case <-done:
			return
		}
	}
}

func pumpStdin(ws *websocket.Conn, inw io.Writer) {
	defer ws.Close()
	ws.SetReadLimit(maxMessageSize)
	ws.SetReadDeadline(time.Now().Add(pongWait))
	ws.SetPongHandler(func(appData string) error { ws.SetReadDeadline(time.Now().Add(pongWait)); return nil })

	for {
		_, message, err := ws.ReadMessage()
		if err != nil {
			break
		}
		message = append(message, '\n')
		if _, err := inw.Write(message); err != nil {
			break
		}
	}
}

func pumpStdout(ws *websocket.Conn, inr io.Reader, done chan struct{}) {
	s := bufio.NewScanner(inr)

	for s.Scan() {
		ws.SetWriteDeadline(time.Now().Add(writeWait))
		if err := ws.WriteMessage(websocket.TextMessage, s.Bytes()); err != nil {
			ws.Close()
			break
		}
	}
	if s.Err() != nil {
		log.Println("scan:", s.Err())
	}
	close(done)

	ws.SetWriteDeadline(time.Now().Add(writeWait))
	ws.WriteMessage(websocket.CloseMessage, websocket.FormatCloseMessage(websocket.CloseMessage, ""))
	time.Sleep(closeGracePeriod)
	ws.Close()

}
