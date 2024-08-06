
C = gcc
F = -Ilist/ -Wall -std=c89 -fsanitize=address -g

C += $(F)

all: main

main: list.o main.o
	$(C) -o main list.o main.o

list.o: list/list.c
	$(C) -c list/list.c -o ./list.o

main.o: main.c
	$(C) -c main.c
