




count = 0 


def Tprint(name):
    global count
    for i in range(3):
        count +=1
        sys_write(f"#Thread = {name}\n")
        sys_sched()


def main():
    n = sys_choose( [3,4,5] )
    sys_write(f"#Thread = {n}\n")

    for name in 'ABCD'[:n]:
        sys_spawn(Tprint, name)
    sys_sched()

