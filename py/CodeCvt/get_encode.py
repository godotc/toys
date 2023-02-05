import chardet
import sys
import os



def is_csv(file):
    return file.endswith(".csv")

def get_under_files_of(theDirectory):
    file_list =   []
    for root,_,files in os.walk(theDirectory):
        for file in files:
            a_full_path = os.path.join(root,file)
            file_list.append((a_full_path))
    return file_list


def get_encode_type(file):
    with open(file,'rb') as f:
        tmp = chardet.detect(f.read())
    f.close()
    return tmp['encoding']

def check(theDirectory):
    "Get files in specific dir"
    file_list = get_under_files_of(theDirectory)
    print(f"Got all files under target directory: {theDirectory}")
    print(file_list,"\n")

    for file in file_list:
        if(all):
            type = get_encode_type(file)
            print(f"file: {file},\n\t code: {type}\n")
        else :
            if(is_csv(file)):
                type = get_encode_type(file)
                print(f"file: {file},\n\t code: {type}\n")
            else:
                print(f"{file} is not csv, Skip...")

if __name__ == "__main__":
   
    global all 
    all = False

    if len(sys.argv)>1:
        if (sys.argv[1]== "all"):
            all = True
            check(sys.argv[2])
    else:
        check(sys.argv[1])
