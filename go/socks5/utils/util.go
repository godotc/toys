package utils

func Err(err error) {
	if err != nil {
		panic(err)
	}
}
