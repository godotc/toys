import chardet
import os
import sys

def is_csv(file):
    return str(file).endswith(".csv")

def gbk2utf8(path):
    encoding = get_encode_type(path)
    print(f"File:{path} -- Encoding: {encoding}")

    if(encoding == 'utf-8'):
        print(f"{path} type already is utf8.")
        return
    elif (encoding=='gbk'):
        gbk2utf8_cvt(path)        

def gbk2utf8_cvt(path):
    print(f"Decoding {path}....")
    # Todo: convert content

def get_encode_type(file):
    with open(file,'rb') as f:
        tmp = chardet.detect(f.read())
        return tmp['encoding']



def convert_files_to_utf8bom(TargetDirectory):
    "Get files in specific dir"
    for root,_,files in os.walk(TargetDirectory):
        for file in files:
            full_path = os.path.join(root,file)
            if is_csv(full_path) :
                gbk2utf8(full_path)
    

if __name__ == '__main__':
    convert_files_to_utf8bom(sys.argv[1])


















