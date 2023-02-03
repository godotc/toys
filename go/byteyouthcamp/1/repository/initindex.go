package repository

import (
	"bufio"
	"encoding/json"
	"os"
)

var (
	topicIndexMap map[int64]*Topic
	postIndexMap  map[int64][]*Post
)

func InitTopicIndexMap(filepath string) error {
	f, err := os.Open(filepath + "topic")
	if err != nil {
		return err
	}

	scanner := bufio.NewScanner(f)
	topicTmpMap := make(map[int64]*Topic)
	for scanner.Scan() {
		text := scanner.Text()
		var topic Topic
		if err := json.Unmarshal([]byte(text), &topic); err != nil {
			return err
		}
	}

	topicIndexMap = topicTmpMap
	return nil
}

func InitPostIndexMap(filepath string) error {
	f, err := os.Open(filepath + "post")
	if err != nil {
		return err
	}

	scanner := bufio.NewScanner(f)

	postTmpMap := make(map[int64][]*Post)
	for scanner.Scan() {
		text := scanner.Text()
		var post Post
		if err := json.Unmarshal([]byte(text), &post); err != nil {
			return err
		}
	}

	postIndexMap = postTmpMap
	return nil
}
