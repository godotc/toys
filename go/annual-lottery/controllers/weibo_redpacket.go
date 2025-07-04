package controllers

import (
	"fmt"
	"log"
	"math/rand"
	"time"

	"github.com/kataras/iris/v12"
)

// red-pack uid -> money list
var packageList map[uint32][]uint32 = map[uint32][]uint32{}

type WeiBoRedPacketController struct {
	Ctx iris.Context
}

func (c *WeiBoRedPacketController) PostSet() {
	type Request struct {
		Uid   uint32 `json:"uid"`
		Money uint32 `json:"money"`
		Size  uint32 `json:"size"`
	}

	var request Request
	fmt.Print(c.Ctx.Values())
	if err := c.Ctx.ReadJSON(&request); err != nil {
		c.Ctx.StatusCode(400)
		c.Ctx.JSON(iris.Map{"error": "Invalid JSON: " + err.Error()})
		return
	}

	if request.Uid == 0 || request.Money == 0 || request.Size == 0 {
		c.Ctx.StatusCode(400)
		c.Ctx.JSON(iris.Map{"error": "Invalid parameters"})
		return
	}

	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	rMax := 0.55
	list := make([]uint32, request.Size)
	for i := uint32(0); i < request.Size; i++ {
		if i == request.Size-1 {
			list[i] = request.Money
		} else {
			max := uint32(float64(request.Money) * rMax)
			min := uint32(1)
			if max < min {
				max = min
			}
			list[i] = r.Uint32()%(max-min+1) + min
			request.Money -= list[i]
		}
	}

	maxTimes := 10
	ok := false
	var id uint32
	for i := 0; i < maxTimes; i++ {
		id = rand.Uint32()
		if packageList[id] == nil {
			packageList[id] = list
			ok = true
			log.Println("Set red packet for uid:", request.Uid, "with money list:", list)
			break
		}
	}
	if !ok {
		c.Ctx.StatusCode(500)
		c.Ctx.JSON(iris.Map{"error": "Failed to set red packet"})
		return
	}

	c.Ctx.JSON(iris.Map{
		"uid":  request.Uid,
		"id":   id,
		"msg":  "Red packet set successfully!",
		"list": list,
	})

}

func (c *WeiBoRedPacketController) GetGet() {

	type Request struct {
		Uid uint32 `json:"uid"`
		Id  uint32 `json:"id"`
	}

	var request Request
	if err := c.Ctx.ReadJSON(&request); err != nil {
		c.Ctx.StatusCode(400)
		c.Ctx.JSON(iris.Map{"error": "Invalid JSON: " + err.Error()})
		return
	}

	if request.Uid < 1 || request.Id < 1 {
		c.Ctx.StatusCode(400)
		c.Ctx.JSON(iris.Map{"error": "Invalid parameters"})
		return
	}

	list, ok := packageList[request.Id]
	if !ok || len(list) == 0 {
		c.Ctx.StatusCode(404)
		c.Ctx.JSON(iris.Map{"error": "No red packet found for this user"})
		return
	}

	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	randomIndex := r.Intn(len(list))
	money := list[randomIndex]
	// update
	if len(list) > 1 {
		if randomIndex == len(list)-1 {
			println("Removing last element from list for id:", request.Id)
			packageList[request.Id] = list[:randomIndex]
		} else if randomIndex == 0 {
			println("Removing first element from list for id:", request.Id)
			packageList[request.Id] = list[1:]
		} else {
			println("Removing element at index", randomIndex, "from list for id:", request.Id)
			packageList[request.Id] = append(list[:randomIndex], list[randomIndex+1:]...)
		}
	} else {
		println("red packet is empty, removing id:", request.Id)
		delete(packageList, request.Id)
	}

	c.Ctx.JSON(iris.Map{
		"uid":   request.Uid,
		"id":    request.Id,
		"money": money,
		"msg":   "Congratulations! You got a red packet!",
	})
}
