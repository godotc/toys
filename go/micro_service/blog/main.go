package main

import (
	"fmt"
	"net/http"

	"github.com/godotc/gms"
)

func main() {
	engine := gms.New()

	gp_user := engine.NewGnoup("user")
	gp_user.Get("/hello", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "%s welcom to gms blog\n", "godot.link")
	})
	gp_user.Post("/info", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "%s get the /user/info", " godot.link")
	})

	gp_order := engine.NewGnoup("order")
	gp_order.Any("/get", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "%s get the /order/get", " godot.link")
	})

	engine.Run()
}
