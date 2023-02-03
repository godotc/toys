package service

import "sync"

type QueryPageInfoFlow struct {
	Id       int64
	PageInfo *PageInfo
}

func NewQueryPageInfoFlow() {

}

func (f *QueryPageInfoFlow) Do() (*PageInfo, error) {

	if err := f.checkParam(); err != nil {
		return nil, err
	}
	if err := f.prepareInfo(); err != nil {
		return nil, err
	}
	if err := f.packPageInfo(); err != nil {
		return nil, err
	}

	return f.PageInfo, nil
}

func (f *QueryPageInfoFlow) checkParam() error {
	// Check the id and permission
	return nil
}

func (f *QueryPageInfoFlow) prepareInfo() error {
	wg := sync.WaitGroup{}
	wg.Add(2)

	// get topic info
	go func() {
		wg.Done()
	}()
	// get post list
	go func() {
		wg.Done()
	}()
	wg.Wait()

	return nil
}

func (f *QueryPageInfoFlow) packPageInfo() error {
	f.PageInfo = &PageInfo{}

	return nil
}
