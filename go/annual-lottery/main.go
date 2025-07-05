package main

import (
	"flag"
	"fmt"
	"lottery/pkgs/test_controllers"
	"lottery/pkgs/utils"

	"github.com/kataras/iris/v12"
	"github.com/kataras/iris/v12/mvc"
)

func testApp() *iris.Application {
	app := iris.New()

	// Development middleware to log request URLs (only in dev mode)
	if utils.IsDevelopment() {
		app.Use(func(ctx iris.Context) {
			fmt.Printf("Request: %s %s\n", ctx.Method(), ctx.Request().URL.Path)
			ctx.Next()
		})
	}

	mvc.New(app.Party("/lottery")).Handle(&test_controllers.LotteryController{})
	mvc.New(app.Party("/ticket")).Handle(&test_controllers.TicketController{})

	wechatShakeController := &test_controllers.WeChatShakeController{}
	wechatShakeController.Init()
	mvc.New(app.Party("/wechat-shake")).Handle(wechatShakeController)

	mvc.New(app.Party("/weibo-redpacket")).Handle(&test_controllers.WeiBoRedPacketController{})

	return app
}

func main() {
	flag.Parse()
	if utils.IsDevelopment() {
		// Development code here
	}

	app := testApp()
	// userList = []string{"Alice", "Bob", "Charlie"}

	addr := "localhost:8080"
	fmt.Printf("Listening on %s\n", addr)
	app.Run(iris.Addr(addr))
}
