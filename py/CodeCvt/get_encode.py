import chardet
import sys

def get_encode_type(file):
    with open(file,'rb') as f:
        tmp = chardet.detect(f.read())
        print(tmp)
    f.close()
    return tmp['encoding']


if __name__ == "__main__":
    get_encode_type(sys.argv[1])
