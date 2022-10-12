CC=gcc
OBJS=*.o


all:gsh  clean



gsh:$(OBJS)
	$(CC) $^ -o gsh


# gcc -c  *.c *,o
%.o:%.c
	$(CC) -c $^

clean:
	rm *.o -rf

uninstall:
	rm gsh
