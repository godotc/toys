// ref: https://web3.storage/docs/how-tos/store/?lang=go

package main

import (
	"context"
	"fmt"
	"io/fs"
	"log"
	"os"

	"github.com/web3-storage/go-w3s-client"
	w3fs "github.com/web3-storage/go-w3s-client/fs"
)

const API_TOKEN = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJkaWQ6ZXRocjoweGE0REEwZTFCZUVjZmE5OTBDNjQzQWFFMTEzMzZCYmI2NjA1M2Q2REEiLCJpc3MiOiJ3ZWIzLXN0b3JhZ2UiLCJpYXQiOjE2ODAyODgwODM1ODAsIm5hbWUiOiJ0ZXN0MDEifQ.BoPyBvb6CNvYS2KnNaVOvRtE1VDvY3VR6DfVLWtZsoU"

func main() {
	client, e := w3s.NewClient(w3s.WithToken(API_TOKEN))
	if e != nil {
		log.Fatalln(e)
	}

	os.Mkdir("./file", 0777)

	f1, e := os.Create("./file/test01.jpg")
	if e != nil {
		log.Fatalln(e)
	}
	f1.Write([]byte("hello world"))

	f2, e := os.Create("./file/test02.jpg")
	if e != nil {
		log.Fatalln(e)
	}
	f2.Write([]byte("bye-bye hell"))

	dir := w3fs.NewDir("images", []fs.File{f1, f2})

	// upload to web3.storage
	cid, e := client.Put(context.Background(), dir)
	if e != nil {
		log.Fatalln(e)
	}

	fmt.Println("CID: ", cid)

	gatewayURL := fmt.Sprintf("ipfs://%s\n", cid.String())

	fmt.Printf("Store files on web3.storage! visit it: %s\n", gatewayURL)

}
