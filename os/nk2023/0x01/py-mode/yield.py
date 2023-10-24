def numbers():
     i =0
     while True:
             ret = yield f'{i:b}'
             i+= ret

n =numbers()
type(n)
n.send(None)
n.send(1)

def my_generator():
    yield 1
    yield 1
    yield 1
    yield "hello", ("hello world")
    yield 2
    yield 3

gen = my_generator()

a = gen.send(None)
#print(a)


a = gen.send("hello")
print(a)

a = gen.send(2)
print(a)



