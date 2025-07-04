package controllers

import (
	"encoding/json"
	"fmt"
	"log"
	"math/rand"
	"os"
	"time"

	"github.com/kataras/iris/v12"
)

/**
* /lucky one interface for drawing lottery
 */

type giftType int

const (
	BTC            giftType = iota // 虚拟货币
	RandomCoupon                   // 优惠券
	FixCoupon                      // 固定的优惠券
	RealSmallPrize                 // 实物小奖
	RealLargePrize                 // 实物大奖
)

type giftState int

const (
	None  giftState = iota // 没有奖品
	InUse                  // 正在使用
)

type Gift struct {
	Id         int       `json:"id"`
	Name       string    `json:"name"`
	PictureURI string    `json:"picture"`
	Link       string    `json:"link"`
	GType      giftType  `json:"type"`
	DataList   []string  `json:"datas"`  // customized data, like BTC address, coupon code, etc.
	Count      int       `json:"count"`  // how many gifts of this type
	Remain     int       `json:"remain"` // how many gifts left
	State      giftState `json:"state"`
	Rate       float32   `json:"rate"`     // probability of winning this gift
	RateMin    float32   `json:"rate_min"` // minimum probability of winning this gift
	RateMax    float32   `json:"rate_max"` // maximum probability of winning this gift
}

var logger *log.Logger
var gifts []*Gift

type WeChatShakeController struct {
	Ctx iris.Context
}

func (c *WeChatShakeController) Init() {
	fmt.Println("Initializing WeChat Shake Controller...")

	// init log
	err := os.MkdirAll("var/log", 0755)
	if err != nil {
		panic(fmt.Sprintf("Failed to create log directory: %v", err))
	}

	f, err := os.Create("var/log/wechat-shake.log")
	if err != nil {
		panic(fmt.Sprintf("Failed to create log file: %v", err))
	}
	logger = log.New(f, "", log.Ldate|log.Lmicroseconds)

	// init gifts
	gifts = []*Gift{
		{
			Id:         1,
			Name:       "Iphone 2000 Pro Max",
			PictureURI: "https://example.com/iphone.jpg",
			Link:       "https://example.com/iphone",
			GType:      RealLargePrize,
			DataList:   []string{},
			Count:      1000,
			Remain:     1000,
			State:      InUse,
			Rate:       1 / 100.0,
			RateMin:    0,
			RateMax:    0,
		},
		{
			Id:         2,
			Name:       "Phone Recharger",
			PictureURI: "https://example.com/recharger.jpg",
			Link:       "https://example.com/recharger",
			GType:      RealSmallPrize,
			DataList:   []string{},
			Count:      5,
			Remain:     5,
			State:      InUse,
			Rate:       1 / 100.0,
			RateMin:    0,
			RateMax:    0,
		},
		{
			Id:         3,
			Name:       "Coupon: 50 off for over 200",
			PictureURI: "https://example.com/coupon.jpg",
			Link:       "https://example.com/coupon",
			GType:      FixCoupon,
			DataList:   []string{"mall-coupon-2018"},
			Count:      5,
			Remain:     5,
			State:      InUse,
			Rate:       1 / 50.0,
			RateMin:    0,
			RateMax:    0,
		},
		{
			Id:         4,
			Name:       "Coupon: 50 off",
			PictureURI: "https://example.com/coupon.jpg",
			Link:       "https://example.com/coupon",
			GType:      FixCoupon,
			DataList:   []string{"c01", "c02", "c03", "c04", "c05"},
			Count:      5,
			Remain:     5,
			State:      InUse,
			Rate:       1 / 50.0,
			RateMin:    0,
			RateMax:    0,
		},
		{
			Id:         5,
			Name:       "BTC 0.01",
			PictureURI: "https://example.com/btc.jpg",
			Link:       "https://example.com/btc",
			GType:      BTC,
			DataList:   []string{"Bitcoin 0.01"},
			Count:      5,
			Remain:     5,
			State:      InUse,
			Rate:       1 / 50.0,
			RateMin:    0,
			RateMax:    0,
		},
	}

	var rateStart float32 = 0.0
	for idx, data := range gifts {
		if data.State != InUse {
			continue
		}
		data.RateMin = rateStart
		data.RateMax = rateStart + data.Rate
		if data.RateMax > 1.0 {
			data.RateMax = 1.0
			rateStart = 0
		} else {
			rateStart += data.Rate
		}

		dataJSON, _ := json.MarshalIndent(data, "", "  ")
		logger.Printf("Gift: %d,  %s", idx, string(dataJSON))
	}

}

func luckyCode() int32 {

	seed := time.Now().UnixNano()
	r := rand.New(rand.NewSource(seed))

	// get number from 1 to 10000
	return r.Int31n(10000) + 1
}

func canSendGift(gift *Gift) bool {
	if gift.Count == 0 { // unlimited count
		return true
	}

	if gift.Remain > 0 {
		return true
	}

	return false

}
func onSendGift(gift *Gift) bool {
	gift.Remain--
	logger.Printf("Sending gift: %s, Remaining: %d", gift.Name, gift.Remain)
	// UNIMPLEMENTED: any business logic to handle sending gift
	return true
}

func (c *WeChatShakeController) GetLucky() {

	code := luckyCode()
	// TODO: use float will cause precision issue, use int32 instead
	floatCode := float32(code) / 10000.0 // from 1 to 10000, convert to 0.0001 to 1.0
	for _, gift := range gifts {
		if gift.State != InUse {
			continue
		}
		if gift.Count > 0 && gift.Remain <= 0 {
			continue
		}

		if floatCode >= gift.RateMin && floatCode <= gift.RateMax {

			if !canSendGift(gift) {
				c.Ctx.JSON(iris.Map{
					"code": code,
					"gift": gift.Name,
					"msg":  "Sorry, all gifts of this type have been claimed",
				})
			} else {
				onSendGift(gift) // handle business logic for sending gift
				c.Ctx.JSON(iris.Map{
					"code":  code,
					"gift":  gift, //TODO: remove sensitive data in production
					"state": "success",
					"msg":   "Congratulations! You won a gift!",
				})
				logger.Printf("Lucky code: %d, Gift: %s", code, gift.Name)
			}
			return
		}
	}

	c.Ctx.JSON(iris.Map{
		"code":  code,
		"state": "failed",
		"msg":   "Better luck next time!",
	})
}
