package main

import (
	"bufio"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strconv"
	"sync"
	"time"

	"github.com/davecgh/go-spew/spew"
	"github.com/joho/godotenv"
)

var (
	Blockchain []Block
	TempBlocks []Block

	CandidateBlocks = make(chan Block)
	Annoucements    = make(chan string)

	mutex      = &sync.Mutex{}
	validators = make(map[string]int)
)

type Block struct {
	Index     int
	Timestamp string
	BPM       int
	PrevHash  string
	Hash      string
	Validator string
}

func hash(s string) string {
	h := sha256.New()
	h.Write([]byte(s))
	hashed := h.Sum(nil)
	return hex.EncodeToString(hashed)

}

func calcHash(block Block) string {
	record := string(block.Index) + block.Timestamp + string(block.BPM) + block.PrevHash
	return hash(record)
}

func generateBlock(old Block, BPM int, address string) (Block, error) {
	hash := calcHash(old)
	hash += string(BPM)
	new_block := Block{
		Index:     old.Index + 1,
		Timestamp: time.Now().String(),
		BPM:       BPM,
		PrevHash:  old.Hash,
		Validator: address,
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

	go func() {
		for {
			msg := <-Annoucements
			io.WriteString(conn, msg)
		}
	}()

	var validator_address string
	io.WriteString(conn, "Enter the token balance to stake:")
	scanner := bufio.NewScanner(conn)

	for scanner.Scan() {
		balance, err := strconv.Atoi(scanner.Text())
		if err != nil {
			log.Printf("%v not a number: %v", scanner.Text(), err)
			return
		}

		validator_address := hash(time.Now().String())
		validators[validator_address] = balance
		fmt.Println(validators)
		break
	}

	io.WriteString(conn, "Enter a new BPM:")
	scanner = bufio.NewScanner(conn)

	go func() {
		for scanner.Scan() {
			bpm, err := strconv.Atoi(scanner.Text())

			if err != nil {
				log.Printf("%v not a number: %v", scanner.Text(), err)
				delete(validators, validator_address)
				conn.Close()
			}

			if err != nil {
				log.Println(err)
			}

			mutex.Lock()
			old_Last_block := Blockchain[len(Blockchain)-1]
			mutex.Unlock()

			new_block, err := generateBlock(old_Last_block, bpm, validator_address)
			if err != nil {
				log.Println(err)
				continue
			}

			if isValidBlock(new_block, Blockchain[len(Blockchain)-1]) {
				CandidateBlocks <- new_block
			}

			io.WriteString(conn, "\nEnter a new BPM:")

		}
	}()

	for {
		time.Sleep(time.Minute)
		mutex.Lock()
		outputs, err := json.Marshal(Blockchain)
		mutex.Unlock()
		if err != nil {
			log.Fatal(err)
		}
		io.WriteString(conn, string(outputs)+"\n")
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
