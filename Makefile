CC=gcc
SRC=$(shell find -name '*.c')
FLAGS=-Wall
LIB=-lm
BIN=main

all:
	$(CC) $(FLAGS) -o $(BIN) $(SRC) $(LIB)
