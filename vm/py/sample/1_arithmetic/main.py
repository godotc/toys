import os.path as p
import tokenize

if 0:
    curdir = p.abspath(p.dirname(__file__))

    path = p.join(curdir, "test_expression.txt")
    print(path)

    with open(path, "r") as f:
        tk = tokenize.generate_tokens(f.readline)

        for tknum, tkval, _, _, _ in tk:
            print(tknum, tkval)
