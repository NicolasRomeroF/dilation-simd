dilation: dilation.c funciones.c
	gcc dilation.c funciones.c -o dilation -msse3 -Wall -fopenmp