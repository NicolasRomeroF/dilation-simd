#ifndef FUNCIONES_H_
#define FUNCIONES_H_

int** crearMatriz(int size);
int** copyImage(int** imagen, int size);
int** dilation(int** imagen, int size);
int** leerArchivo(char nombreArchivo[128],int size);
void printMatriz(int** imagen, int size);
void createFile(char nombre[128],int** imagen, int size);
int** dilationSIMD(int** imagen, int size);
void freeMatriz(int** matriz, int size);

#endif