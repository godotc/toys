package main

import (
	"fmt"
	"sync"
	"testing"

	"github.com/kataras/iris/v12/httptest"
)

func TestMVC(t *testing.T) {

	e := httptest.New(t, testApp())

	var wg sync.WaitGroup
	e.GET("/").Expect().Status(200).
		JSON().Object().ContainsKey("count").Value("count").IsEqual(0)

	// test concurrent requests
	for i := range 100 {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			e.POST("/import").
				WithFormField("users", "test_user_"+fmt.Sprint(i)).
				Expect().Status(200)
		}(i)
	}
	wg.Wait()

	e.GET("/").Expect().Status(200).JSON().Object().ContainsKey("count").Value("count").IsEqual(100)
	e.GET("/lucky").Expect().Status(200).JSON().Object().ContainsKey("winner").NotEmpty()
}
