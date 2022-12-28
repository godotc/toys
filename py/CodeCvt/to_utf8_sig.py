import chardet
import os
import sys
import codecs

def is_csv(file):
    return str(file).endswith(".csv")


def to_utf8_bom (path,originEncoding):
    print(f"Encoding {path}....")

    try:
        f=codecs.open(path,'r',originEncoding)
        new_content = f.read()
        codecs.open(path,'w','utf_8_sig').write(new_content)
    except IOError as err:
        msg=  format(err)
        print(f"I/O error: {msg}")

        

def toUtf8(path):
    print("\n---------------------")
    encoding = get_encode_type(path)
    print(f"File:{path}, Encoding: {encoding}")

    if(encoding == 'utf_8_sig'):
        print(f"{path} type already is utf8.")
        return
    else:
        to_utf8_bom(path, encoding)
    print("-----------------")


def get_encode_type(file):
    with open(file,'rb') as f:
        tmp = chardet.detect(f.read())
    f.close()
    return tmp['encoding']


def get_under_files_of(theDirectory):
    file_list =   []
    for root,_,files in os.walk(theDirectory):
        for file in files:
            a_full_path = os.path.join(root,file)
            file_list.append((a_full_path))
    return file_list
     

def convert_files_to_utf8_bom(theDirectory):
    "Get files in specific dir"
    file_list = get_under_files_of(theDirectory)
    print(f"Got all files under target directory: {theDirectory}")
    print(file_list,"\n")


    for file in file_list:
        if is_csv(file):
                toUtf8(file)
        else:
            print("Not csv, Skip...")
    

if __name__ == '__main__':
    convert_files_to_utf8_bom(sys.argv[1])
