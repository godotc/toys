package main

import (
	"bufio"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"io"
	"log"
	"net"
	"os"
	"strconv"
	"time"

	"github.com/davecgh/go-spew/spew"
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

func handleConn(conn net.Conn) {
	defer conn.Close()

	io.WriteString(conn, "Enter a new BPM:")

	scanner := bufio.NewScanner(conn)

	go func() {
		for scanner.Scan() {
			bpm, err := strconv.Atoi(scanner.Text())
			if err != nil {
				log.Printf("%v not a number: %v", scanner.Text(), err)
				continue
			}

			new_block, err := generateBlock(Blockchain[len(Blockchain)-1], bpm)
			if err != nil {
				log.Println(err)
				continue
			}

			if isValidBlock(new_block, Blockchain[len(Blockchain)-1]) {
				new_block_chain := append(Blockchain, new_block)
				replaceChain(new_block_chain)
			}

			BlockChainServer <- Blockchain
			io.WriteString(conn, "\nEnter a new BPM:")
		}
	}()

	go func() {
		for {
			time.Sleep(30 * time.Second)
			out, err := json.Marshal(Blockchain)
			if err != nil {
				log.Fatal(err)
			}
			io.WriteString(conn, string(out))
		}
	}()

	for _ = range BlockChainServer {
		spew.Dump(Blockchain)
	}
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

	// start server listen
	port := os.Getenv("PORT")
	server, err := net.Listen("tcp", "127.0.0.1:"+port)
	if err != nil {
		log.Fatal(err)
	}
	log.Printf("Listening on %v...\n", port)
	defer server.Close()

	// handel manualy without router and callback
	for {
		conn, err := server.Accept()
		if err != nil {
			log.Fatal(err)
		}
		go handleConn(conn)
	}
}
