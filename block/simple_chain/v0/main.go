package main

import (
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"io"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/davecgh/go-spew/spew"
	"github.com/gorilla/mux"
	"github.com/joho/godotenv"
)

var Blockchain []Block
var BlockChainServer chan []Block

type Block struct {
	Index     int
	Timestamp string
	BPM       int
	PrevHash  string
	Hash      string
}

func calcHash(block Block) string {
	record := string(block.Index) + block.Timestamp + string(block.BPM) + block.PrevHash
	h := sha256.New()
	h.Write([]byte(record))
	hashed := h.Sum(nil)
	return hex.EncodeToString(hashed)
}

func generateBlock(old Block, BPM int) (Block, error) {
	hash := calcHash(old)
	hash += string(BPM)
	new_block := Block{
		Index:     old.Index + 1,
		Timestamp: time.Now().String(),
		BPM:       BPM,
		PrevHash:  old.Hash,
	}
	new_block.Hash = calcHash(new_block)

	return new_block, nil
}

func isValidBlock(new, old Block) bool {
	if old.Index+1 != new.Index ||
		old.Hash != new.PrevHash ||
		calcHash(new) != new.Hash {
		return false
	}
	return true

}

func replaceChain(new_block_chain []Block) {
	if len(new_block_chain) > len(Blockchain) {
		Blockchain = new_block_chain
	}
}

type Message struct{ BPM int }

func respondWithJSON(w http.ResponseWriter, r *http.Request, code int, payload interface{}) {
	response, err := json.MarshalIndent(payload, "", " ")
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		w.Write([]byte("HTTP 500: internal server error"))
		return
	}
	w.WriteHeader(code)
	w.Write(response)
}

func handleGetBlockchain(w http.ResponseWriter, r *http.Request) {
	bytes, err := json.MarshalIndent(Blockchain, "", " ")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	io.WriteString(w, string(bytes))
}
func handleWritehBlockchain(w http.ResponseWriter, r *http.Request) {
	var m Message

	decoder := json.NewDecoder(r.Body)
	if err := decoder.Decode(&m); err != nil {
		respondWithJSON(w, r, http.StatusBadRequest, r.Body)
		return
	}

	defer r.Body.Close()

	new_block, err := generateBlock(Blockchain[len(Blockchain)-1], m.BPM)
	if err != nil {
		respondWithJSON(w, r, http.StatusInternalServerError, m)
		return
	}
	if isValidBlock(new_block, Blockchain[len(Blockchain)-1]) {
		new_block_chain := append(Blockchain, new_block)
		replaceChain(new_block_chain)
		spew.Dump(Blockchain)
	}

	respondWithJSON(w, r, http.StatusCreated, new_block)
}

func initMuxRouter() http.Handler {
	mux := mux.NewRouter()
	mux.HandleFunc("/", handleGetBlockchain).Methods("GET")
	mux.HandleFunc("/", handleWritehBlockchain).Methods("POST")
	return mux
}

func run() error {
	mux := initMuxRouter()
	port := os.Getenv("PORT")
	log.Printf("Listening on %v...\n", port)

	s := &http.Server{
		Addr:           "127.0.0.1:" + port,
		Handler:        mux,
		ReadTimeout:    10 * time.Second,
		WriteTimeout:   10 * time.Second,
		MaxHeaderBytes: 1 << 20,
	}

	if err := s.ListenAndServe(); err != nil {
		return err
	}

	return nil
}

func main() {
	err := godotenv.Load()
	if err != nil {
		log.Fatal(err)
	}

	BlockChainServer = make(chan []Block)

	// genesis block
	generateBlock := Block{
		Index:     0,
		Timestamp: time.Now().String(),
		BPM:       0,
		PrevHash:  "",
		Hash:      "",
	}
	spew.Dump(generateBlock)
	Blockchain = append(Blockchain, generateBlock)

	log.Fatal(run())

}
