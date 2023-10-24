import random
import sys
from pathlib import Path


runables =[]


#def sys_write(s): print(s)
#def sys_choose(xs): return random.choice(xs)
#def sys_spawn(t): runables.append(t)
#

class OS():
    SYSCALLS = ['choose', 'write', 'spawn','sched']


    class Thread:
        def __init__(self, func, *args) -> None:
            self._func = func(*args)
            self.retval =None

        def step(self):
            syscall, args, *_ = self._func.send(self.retval)
            self.retval= None
            return syscall, args


    
    def __init__(self, src) -> None:
        variables = {}
        exec(src, variables)
        self._main = variables['main']




    def run(self):
        threads = [OS.Thread(self._main)]
        while threads:
            global t
            try:
                match(t := threads[0]).step():
                    case 'choose', xs:
                        t.retval = random.choice(xs)

                    case 'write', xs:
                        print(xs, end='')

                    case 'spawn',(fn, args):
                        threads+=[OS.Thread(fn, *args)]

                    case 'sched', _:
                        random.shuffle(threads)

            except StopIteration:
                threads.remove(t)
                random.shuffle(threads)



if __name__ == '__main__':
    if  len(sys.argv)<2:
        print(f"Usage: {sys.argv[0]} file")
        exit(1)


    src = Path(sys.argv[1]).read_text()
    for syscall in OS.SYSCALLS:
        src = src.replace( f'sys_{syscall}',  # sys_choose(...)
                          f'yield "{syscall}", ') # yield "choose", (...)

    OS(src).run()

