CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wshadow -O3 -g

all: main

main: main.c
	$(CC) $(CFLAGS) threshold.c threshold.h main.c -o main

clean:
	rm -f main
