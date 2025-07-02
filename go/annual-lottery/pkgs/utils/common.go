package utils

import (
	"flag"
	"os"
)

var devMode = flag.Bool("dev", false, "Enable development mode")

func IsDevelopment() bool {
	if !*devMode {
		return false
	}
	env := os.Getenv("GO_ENV")
	return env == "" || env == "development" || env == "dev"
}
