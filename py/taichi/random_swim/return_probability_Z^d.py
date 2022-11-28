import taichi as ti

ti.init(arch=ti.gpu)


d = 3
num_round = 100000
max_steps = 100000

ivec = ti.types.vector(d, int)
origin = ivec(0)  # ivec([0]* d)


dirs = ti.Vector.field(d, int, shape=2*d)

# generate 6 directions
for k in range(d):
    dirs[2*k][k] = 1
    dirs[2*k+1][k] = -1

print(dirs)


@ti.func
def choose_random_direction():
    ind = int(ti.random() * 2*d)
    return dirs[ind]


@ti.kernel
def walk() -> float:
    success = 0
    for _ in range(num_round):
        pos = origin
        for step in range(max_steps):
            pos += choose_random_direction()
            if all(pos == origin):
                success += 1
                break

    return success/num_round


print(walk())
