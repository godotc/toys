package test_controllers

import (
	"fmt"
	"math/rand"
	"time"

	"github.com/kataras/iris/v12"
)

/**
* 彩票形式
 */

type TicketController struct {
	Ctx iris.Context
}

// 即开即得型
func (c *TicketController) Get() {

	type Result struct {
		Prize       string `json:"prize"`
		Level       int8   `json:"level"`
		Code        int8   `json:"code"`
		Info        string `json:"info"`
		Description string `json:"description"`
	}

	var result Result

	seed := time.Now().UnixNano()
	r := rand.New(rand.NewSource(seed))

	code := r.Intn(10) + 1 // 生成1-10的随机数

	switch {
	case code == 1:
		result.Info = "一等奖"
		result.Level = 1
	case code >= 2 && code <= 3:
		result.Info = "二等奖"
		result.Level = 2
	case code >= 4 && code <= 6:
		result.Info = "三等奖"
		result.Level = 3
	default:
		result.Level = -1
		result.Description = fmt.Sprintf("code = %d, 没有中奖", code)
	}
	result.Code = int8(code)

	// iris with JSON response, by marshal Result struct
	c.Ctx.JSONP(result)
}

// 双色球自选型号
func (c *TicketController) GetPrize() {

	seed := time.Now().UnixNano()
	r := rand.New(rand.NewSource(seed))

	var prize [7]int
	// 6 red balls and 1 blue ball
	for i := 0; i < 6; i++ {
		prize[i] = r.Intn(33) + 1 // 1-33
	}
	prize[6] = r.Intn(16) + 1 // 1-16 for blue ball

	c.Ctx.JSON(iris.Map{
		"red_balls": prize[:6],
		"blue_ball": prize[6],
		"description": fmt.Sprintf("今日双色球中奖号码为：%d, %d, %d, %d, %d, %d + 蓝球 %d",
			prize[0], prize[1], prize[2], prize[3], prize[4], prize[5], prize[6]),
	})
}
