CC=gcc
CFLAGS=-I.

all: funciones.o main.o
	$(CC) -o dilation funciones.o main.o -lpthread -msse3 -I.
	rm main.o funciones.o