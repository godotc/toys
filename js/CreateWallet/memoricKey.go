package main

import (
	"fmt"
	"log"
	"os"

	hdwallet "github.com/miguelmota/go-ethereum-hdwallet"
	"github.com/tyler-smith/go-bip39"
)

func main() {

	//entropy, err := bip39.NewEntropy(128)
	//if err != nil {
	//	log.Fatal(err)
	//}

	// 助记词
	//mnemonic, _ := bip39.NewMnemonic(entropy)
	mnemonic := os.Args[1]
	//var mnemonic = "pepper hair process town say voyage exhibit over carry property follow define"
	fmt.Println("mnemonic:", mnemonic)

	var passowrd string = os.Args[2]
	seed := bip39.NewSeed(mnemonic, passowrd) //这里可以选择传入指定密码或者空字符串，不同密码生成的助记词不同

	wallet, err := hdwallet.NewFromSeed(seed)
	if err != nil {
		log.Fatal(err)
	}

	//最后一位是同一个助记词的地址id，从0开始，相同助记词可以生产无限个地址
	path := hdwallet.MustParseDerivationPath("m/44'/60'/0'/0/0")
	account, err := wallet.Derive(path, false)
	if err != nil {
		log.Fatal(err)
	}

	address := account.Address.Hex()
	privateKey, _ := wallet.PrivateKeyHex(account)
	publicKey, _ := wallet.PublicKeyHex(account)

	fmt.Println("address0:", address)      // id为0的钱包地址
	fmt.Println("privateKey:", privateKey) // 私钥
	fmt.Println("publicKey:", publicKey)   // 公钥

	fmt.Println("-----------------Account by another id-----------------------------")
	path = hdwallet.MustParseDerivationPath("m/44'/60'/0'/0/1") //生成id为1的钱包地址
	account, err = wallet.Derive(path, false)
	if err != nil {
		log.Fatal(err)
	}

	address = account.Address.Hex()
	privateKey, _ = wallet.PrivateKeyHex(account)
	publicKey, _ = wallet.PublicKeyHex(account)

	fmt.Println("address1:", address)      // id为0的钱包地址
	fmt.Println("privateKey:", privateKey) // 私钥
	fmt.Println("publicKey:", publicKey)   // 公钥

}
