all:
	gcc -Wall -Wextra -O3 -g main.c -o main
run:
	./main
debug:
	gdb ./main

