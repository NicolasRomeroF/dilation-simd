#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include <stdlib.h>
#include <stdio.h>
#include <emmintrin.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

int** crearMatriz(int size);
int** copyImage(int** imagen, int size);
int** dilation(int** imagen, int size);
int** leerArchivo(char nombreArchivo[128],int size);
void printMatriz(int** imagen, int size);
void createFile(char nombre[128],int** imagen, int size);
int** dilationSIMD(int** imagen, int size, int cantHebras);
void freeMatriz(int** matriz, int size);

#endif