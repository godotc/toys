package service

import "userservice/repository"

type PageInfo struct {
	Topic    *repository.Topic
	PostList []*repository.Post
}
