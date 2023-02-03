package controller

import (
	"strconv"
	"userservice/service"
)

type PageData struct {
	Code int64       `json:"code"`
	Msg  string      `json:"msg"`
	Data interface{} `json:"data"`
}

func QueryPageInfo(topicIdStr string) *PageData {
		return &PageData{}
	}

}
