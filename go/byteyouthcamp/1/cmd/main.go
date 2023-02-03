package main

import (
	"os"
	"userservice/controller"
	"userservice/repository"

	"github.com/gin-gonic/gin"
)

func main() {
	if err := Init("../data/"); err != nil {
		os.Exit(-1)
	}

	g := gin.Default()
	g.GET("/community/page/get/:id", func(ctx *gin.Context) {
		topicId := ctx.Param("id")
		data := controller.QueryPageInfo(topicId)
		ctx.JSON(200, data)
	})
	err := g.Run()
	if err != nil {
		return
	}
}

func Init(path string) error {
	repository.NewPostDaoInstance()
	repository.NewTopicDaoInstance()

	err := repository.InitPostIndexMap(path)
	repository.InitTopicIndexMap(path)

}
