CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wpedantic

main:
	$(CC) $(CFLAGS) main.c -o main

main_light:
	$(CC) $(CFLAGS) main_light.c -o main_light

check: main main_light
	valgrind ./main
	valgrind ./main_light
