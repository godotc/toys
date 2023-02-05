import chardet
import os
import sys


def name_is_not_utf8(file):
    fname = str(os.path.basename(file))
    result = chardet.detect(str.encode(fname))
    encode = result["encoding"]
    print(encode)

    b = str.encode(fname, "gb18030")
    c = str.encode(fname, "utf_8_sig")
    print(b)
    print(c)

    # o1 = str(b, 'utf_8_sig')
    o2 = str(c, 'utf_8_sig')
    # print(o1)


def rename_to_utf8(file):
    if (name_is_not_utf8(file)):
        return


def get_under_files_of(theDirectory):
    file_list = []
    for root, _, files in os.walk(theDirectory):
        for file in files:
            a_full_path = os.path.join(root, file)
            file_list.append((a_full_path))
    return file_list


def convert_files_name_to_utf8(theDirectory):
    "Get files in specific dir"
    file_list = get_under_files_of(theDirectory)
    print(f"Got all files under target directory: {theDirectory}")
    print(file_list, "\n")

    for file in file_list:
        rename_to_utf8(file)


if __name__ == '__main__':
    convert_files_name_to_utf8(sys.argv[1])
