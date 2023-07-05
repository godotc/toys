import sys
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



class Token:
    def __init__(self,tk_num, tk_val) -> None:
        self.token_num = tk_num
        self.token_value = tk_val


global current_token


def Current():
    global current_token
    return current_token;


def next(tk):
    toknum, tokval,_,_,_ = tk.next()
    global current_token
    current_token = Token(toknum,tokval)



def expr(tk):
    s1 = term(tk)

    toknum = Current().token_num
    tokval = Current().token_value

    value= s1

    while "+-".find(tokval):
        print (f"expr tokvalue is {tokval}")
        next(tk)

        s2 = term(tk)
        if tokval == "+":
            value += s2
        elif tokval == "-":
            value -=s2

        toknum = Current().token_num
        tokval = Current().token_value

        print (f"expr value is {value}")
        return value

def term(tk):
    f1 = factor(tk)
    toknum = Current().token_num
    tokvalue = Current().token_value

    value = f1

    while tokvalue == "*" or tokvalue == "/":
        print(f"term tokvalue is {tokvalue}")
        next(tk)

        f2 = factor(tk)

        if tokvalue == '*':
            value *= f2
            print(f"term value is {value}")
        elif tokvalue =='/':
            value/=f2
            print(f"term value is {value}")

        toknum = Current().token_num
        tokvalue = Current().token_value
    print(f"term return is {value}")
    return value

def factor(tk):
    if Current().token_num ==  tokenize.NUMBER:
        value = Current().token_value
        next(tk)
    elif Current().token_value =="(":
        next(tk)
        f = expr(tk)
        
        if Current().token_value != ")":
            print (f"parse error! value = {Current().token_value}")
        value = f
        next(tk)

        return int(value)


if __name__ == "__main__":
    f = open(sys.argv[1])
    tk = tokenize.generate_tokens(f.readline)
    next(tk)
    print(expr(tk))

