package pb

import (
	"testing"

	"github.com/golang/protobuf/proto"
)

func TestPb(t *testing.T) {
	test := &Student{
		Name:    "hello",
		Male:    true,
		Scores:  []int32{99, 98, 97},
		Subject: map[string]int32{"age": 18, "level": 1},
	}

	pb, err := proto.Marshal(test)
	if err != nil {
		t.Log("from struct to pb failed: ", err)
		return
	}

	rv := &Student{}
	err = proto.Unmarshal(pb, rv)
	if err != nil {
		t.Log("from pb to struct failed: ", err)
		return
	}

	t.Log(test.GetScores()[1])
	t.Log(rv.GetScores()[1])

}
