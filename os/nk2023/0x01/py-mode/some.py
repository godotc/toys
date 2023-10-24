

def sys_write(msg):
    print(msg)


def main():
    n = sys_choose([3, 4, 5])
    sys_write(f"$Thread = {n}\n")
