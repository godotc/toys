package controllers

import (
	"encoding/json"
	"fmt"
	"lottery/pkgs/utils"
	"math/rand"
	"strings"
	"sync"
	"time"

	"github.com/kataras/iris/v12"
)

/**
* 年会抽奖形式
 */

var userList []string
var userListMutex sync.RWMutex

type LotteryController struct {
	Ctx iris.Context
}

func (c *LotteryController) Get() {
	c.Ctx.ContentType("application/json")

	// userListMutex.RLock()
	count := len(userList)
	// userListMutex.RUnlock()

	if count == 0 {
		c.Ctx.StatusCode(200)
		json.NewEncoder(c.Ctx).Encode(iris.Map{
			"status":  "error",
			"message": "No users available for the lottery.",
			"count":   0,
		})
		return
	}

	s := fmt.Sprintf("Current user count: %d", count)

	c.Ctx.StatusCode(200)

	json.NewEncoder(c.Ctx).Encode(iris.Map{
		"status":  "success",
		"message": s,
		"count":   count,
	})
}

func (c *LotteryController) GetLucky() {
	c.Ctx.ContentType("application/json")

	userListMutex.Lock()
	defer userListMutex.Unlock()

	count := len(userList)
	if count == 0 {
		message := "No users available for the lottery. Please add users first."
		if utils.IsDevelopment() {
			message += "(use POST /import to add users)"
		}
		c.Ctx.StatusCode(200)
		json.NewEncoder(c.Ctx).Encode(iris.Map{
			"status":  "error",
			"message": message,
			"count":   0,
		})
		return
	}

	seed := time.Now().UnixNano()
	r := rand.New(rand.NewSource(seed))
	idx := r.Int31n(int32(count))
	selectedUser := userList[idx]
	userList = append(userList[:idx], userList[idx+1:]...) // Remove the selected user from the list.

	c.Ctx.StatusCode(200)
	json.NewEncoder(c.Ctx).Encode(iris.Map{
		"status":        "success",
		"message":       fmt.Sprintf("Currently %d users in the lottery. The lucky user is: %s", count, selectedUser),
		"winner":        selectedUser,
		"previousCount": count,
		"currentCount":  len(userList),
	})
}

// post
func (c *LotteryController) PostImport() string {
	inUsers := c.Ctx.FormValue("users")
	fmt.Printf("Importing users: %s\n", inUsers)
	users := strings.Split(inUsers, ",")

	userListMutex.Lock()
	preCount := len(userList)

	for _, user := range users {
		user = strings.TrimSpace(user)
		if len(user) > 0 {
			userList = append(userList, user)
		}
	}

	postCount := len(userList)
	userListMutex.Unlock()

	return fmt.Sprintf("Added %d users to the lottery. Total users now: %d (previously: %d)",
		postCount-preCount, postCount, preCount)

}
