import sys
import os.path as p
import tokenize
from typing import Generator
from AST import *


if 0:
    curdir = p.abspath(p.dirname(__file__))

    path = p.join(curdir, "test_expression.txt")
    print(path)

    with open(path, "r") as f:
        tk = tokenize.generate_tokens(f.readline)

        for tknum, tkval, _, _, _ in tk:
            print(tknum, tkval)


class Token:
    def __init__(self, tk_num: int, tk_val: str) -> None:
        self.token_num: int = tk_num
        self.token_value: str = tk_val


global current_token


def Current() -> Token:
    global current_token
    return current_token


def next_token(tk: Generator[tokenize.TokenInfo, None, None]) -> None:
    toknum, tokvalue, _, _, _ = next(tk)
    global current_token
    current_token = Token(toknum, tokvalue)


def expr(tk: Generator[tokenize.TokenInfo, None, None]):
    s1 = term(tk)

    toknum = Current().token_num
    tokval = Current().token_value

    value = s1

    while tokval in "+-":
        print(f"expr tokvalue is {tokval}")
        next_token(tk)

        s2 = term(tk)
        if tokval == "+":
            value = AddNode(value, s2)
        elif tokval == "-":
            value = SubNode(value, s2)

        toknum = Current().token_num
        tokval = Current().token_value

        #print(f"expr value is {value.visit()}")

    return value


def term(tk: Generator[tokenize.TokenInfo, None, None]):
    f1 = factor(tk)
    toknum = Current().token_num
    tokvalue = Current().token_value

    value = f1

    while tokvalue in "*/":
        print(f"term tokvalue is {tokvalue}")
        next_token(tk)

        f2 = factor(tk)

        if tokvalue == '*':
            value = MulNode(value, f2)
            #print(f"term value is {value.value()}")
        elif tokvalue == '/':
            value = DevNode(value, f2)
            print(f"term value is {value.visit()}")

        toknum = Current().token_num
        tokvalue = Current().token_value

    #print(f"term return is {value.visit()}")
    return value


def factor(tk: Generator[tokenize.TokenInfo, None, None]):
    value = 0
    if Current().token_num == tokenize.NUMBER:
        value = Current().token_value
        next_token(tk)

        return ConstNode(int(value))

    elif Current().token_value == "(":
        next_token(tk)
        f = expr(tk)

        if Current().token_value != ")":
            print(f"parse error! value = {Current().token_value}")
        value = f
        next_token(tk)

        return value


if __name__ == "__main__":
    f = open(sys.argv[1])
    tk = tokenize.generate_tokens(f.readline)
    next_token(tk)
    print(expr(tk).visit())
