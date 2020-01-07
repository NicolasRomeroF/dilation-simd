main: main.c funciones.c
	gcc main.c funciones.c -o dilation -msse3 -Wall -fopenmp