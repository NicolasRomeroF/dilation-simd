/*
Laboratorio 1
Autor: Nicolas Romero Flores
Asignatura: Computacion de alto rendimiento
*/

#include "funciones.h"

/*
Funcion que retorna un puntero de memoria a una matriz del tamano dado
Entrada: Alto de la matriz
Salida: Puntero con memoria asignada
*/
int** crearMatriz(int size){
    int** matriz = (int**) malloc(sizeof(int*)*size);
    if(matriz==NULL){
        printf("No se pudo asignar memoria\n");
        return NULL;
    }
    int i;
    for(i = 0; i<size;i++){
        matriz[i] = (int*)malloc(sizeof(int)*size);
        if(matriz[i]==NULL){
            printf("No se pudo asignar memoria\n");
            return NULL;
        }
    }
    return matriz;
}

/*
Funcion que copia una matriz de numeros de numero del tamano dado
Entrada: Matriz a copiar, tamano de la matriz
Salida: Puntero a la copia de la matriz
*/
int** copyImage(int** imagen, int size){
    int** copia = crearMatriz(size);
    if(copia==NULL){
        printf("No se pudo copiar la matriz\n");
        return NULL;
    }
    int i,j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            copia[i][j]=imagen[i][j];
        }
    }
    return copia;
}

/*
Funcion que obtiene el mayor entre dos numeros
Entrada: numeros a comparar
Salida: mayor numero
*/
int mayor(int a,int b){
    if(a>b){
        return a;
    }
    return b;
}

/*
Funcion que aplica la operacion dilation de forma secuencial
Entrada: Matriz de valores, tamano de la matriz
Salida: Matriz con operacion aplicada
*/
int** dilation(int** imagen, int size){
    int i,j,newPix;
    int** copia = copyImage(imagen,size);
    if(copia==NULL){
        printf("No se pudo aplicar la operacion\n");
        return NULL;
    }
    for(i = 1; i<size-1; i++){
        for(j = 1; j<size-1; j++){
            newPix = mayor(imagen[i-1][j], mayor(imagen[i+1][j], mayor(imagen[i][j+1], mayor(imagen[i][j-1],imagen[i][j]))));
            copia[i][j] = newPix;
        }
    }
    return copia;
}

/*
Funcion que permite leer el archivo de entrada que contiene la imagen
Entrada: Nombre del archivo, tamano de la matriz a leer
Salida: Matriz de la imagen leida
*/
int** leerArchivo(char nombreArchivo[128],int size){
    FILE* archivo = fopen(nombreArchivo,"rb");

    if(archivo==NULL){
        printf("No se pudo leer el archivo\n");
        return NULL;
    }
    int i,j;
    int** imagen = crearMatriz(size);
    if(imagen==NULL){
        printf("No se pudo leer el archivo\n");
        return NULL;
    }
    int buffer;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            fread(&buffer,sizeof(int),1,archivo);
            imagen[i][j] = buffer;
        }
    }
    fclose(archivo);
    return imagen;
}

/*
Funcion que permite mostrar por pantalla una matriz de valores
Entrada: Matriz de valores, tamano de la matriz
Salida: void
*/
void printMatriz(int** imagen, int size){
    int i,j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            if(imagen[i][j]>=255){
                printf("1 ");
            }
            else{
                printf("0 ");
            }
        }
        printf("\n");
    }
}

/*
Funcion que permite crear un archivo de salida con los valores de la matriz
Entrada: nombre del archivo, matriz de valores, tamano de la matriz
Salida: void
*/
void createFile(char nombre[128],int** imagen, int size){
    FILE* f = fopen(nombre, "wb");

    if(f==NULL){
        printf("No se pudo crear el archivo\n");
        return;
    }
    int i,j,pix;

    for(i = 0; i < size; i++){
        for(j = 0;j < size; j++){
            pix = imagen[i][j];
            fwrite(&pix,sizeof(int),1,f);
        }
        
    }
    fclose(f);
    printf("Archivo %s escrito exitosamente\n",nombre);
}

/*
Funcion que aplica la operacion dilation con registros SIMD
Entrada: Matriz de valores, tamano de la matriz
Salida: Matriz con operacion aplicada
*/
int** dilationSIMD(int** imagen, int size){
    int i,j,k,newPix;
    int** copia = copyImage(imagen,size);
    if(copia==NULL){
        printf("No se pudo aplicar la operacion SIMD\n");
        return NULL;
    }
    int resto = (size-2)%4;
    __m128i up,down,left,right,center;
    for(i = 1; i<size-1; i++){
        for(j = 1; j<size-4; j+=4){
            up = _mm_loadu_si128((__m128i*)&imagen[i-1][j]);
            down = _mm_loadu_si128((__m128i*)&imagen[i+1][j]);
            left = _mm_loadu_si128((__m128i*)&imagen[i][j-1]);
            right = _mm_loadu_si128((__m128i*)&imagen[i][j+1]);
            center = _mm_loadu_si128((__m128i*)&imagen[i][j]);
            center = _mm_max_epi16(center,down);
            center = _mm_max_epi16(center,up);
            center = _mm_max_epi16(center,right);
            center = _mm_max_epi16(center,left);
            
            //newPix = imagen[i-1][j]+imagen[i+1][j]+imagen[i][j+1]+imagen[i][j-1];
            _mm_storeu_si128((__m128i*)&copia[i][j],center);
            
        }
        for(k = size-resto-1;k<size-1;k++){
            newPix = imagen[i-1][k]+imagen[i+1][k]+imagen[i][k+1]+imagen[i][k-1]+imagen[i][k];
            copia[i][k] = newPix;
        }
    }
    return copia;
}

/*
Funcion que permite liberar la memoria de una matriz
Entrada: Puntero a la matriz, tamano de la matriz
Salida: void
*/
void freeMatriz(int** matriz, int size){
    int i;
    for(i=0;i<size;i++){
        free(matriz[i]);
    }
    free(matriz);
}