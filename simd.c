#include <stdlib.h>
#include <stdio.h>
#include <emmintrin.h>

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

int** copyImage(int** imagen, int size){
    int** copia = crearMatriz(size);
    if(copia==NULL){
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

int** dilation(int** imagen, int size){
    int i,j,newPix;
    int** copia = copyImage(imagen,size);
    if(copia==NULL){
        return NULL;
    }
    for(i = 1; i<size-1; i++){
        for(j = 1; j<size-1; j++){
            newPix = imagen[i-1][j]+imagen[i+1][j]+imagen[i][j+1]+imagen[i][j-1];
            copia[i][j] = newPix;
        }
    }
    return copia;
}

int** leerArchivo(char nombreArchivo[128],int size){
    FILE* archivo = fopen(nombreArchivo,"r");
    int i,j;
    int** imagen = crearMatriz(size);
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

void printMatriz(int** imagen, int size){
    int i,j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("%d ",imagen[i][j]);
        }
        printf("\n");
    }
}

void createFile(char nombre[128],int** imagen, int size){
    FILE* f = fopen(nombre, "w");

    if(f==NULL){
        printf("No se puede crear el archivo de salida\n");
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
}

int** dilationSIMD(int** imagen, int size){
    int i,j,k,newPix;
    int** copia = copyImage(imagen,size);
    if(copia==NULL){
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
            center = _mm_add_epi32(up,down);
            center = _mm_add_epi32(center,right);
            center = _mm_add_epi32(center,left);
            //newPix = imagen[i-1][j]+imagen[i+1][j]+imagen[i][j+1]+imagen[i][j-1];
            _mm_storeu_si128((__m128i*)&copia[i][j],center);
            
        }
        for(k = size-resto-1;k<size-1;k++){
            newPix = imagen[i-1][k]+imagen[i+1][k]+imagen[i][k+1]+imagen[i][k-1];
            copia[i][k] = newPix;
        }
    }
    return copia;
}

int main(){
        


    int size = 256;

    int** imagen = leerArchivo("circulosD.raw",size);
    if(imagen==NULL){
        return -1;
    }


    //int** imagenD = dilation(imagen, size);
    int** imagenD = dilationSIMD(imagen, size);
    if(imagenD==NULL){
        return -1;
    }
    printMatriz(imagenD,size);
    
    createFile("circulosD.raw",imagenD,size);

    return 0;
}