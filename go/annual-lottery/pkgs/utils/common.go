package utils

import (
	"flag"
	"math/rand"
	"os"
	"time"
)

var devMode = flag.Bool("dev", false, "Enable development mode")

func IsDevelopment() bool {
	if !*devMode {
		return false
	}
	env := os.Getenv("GO_ENV")
	return env == "" || env == "development" || env == "dev"
}

func GetRNG() *rand.Rand {
	seed := time.Now().UnixNano()
	generator := rand.New(rand.NewSource(seed))
	return generator

}
