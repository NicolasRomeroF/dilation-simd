#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int valores[2] = {0,255};

void createFile(char nombre[128], int size){
    FILE* f = fopen(nombre, "w");
    int i,j,r;
    int matriz[14][14] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,255,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,255,255,255,0,0,0,0,0,0,0},
        {0,0,0,0,0,255,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,255,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,255,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,255,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };
    for(i = 0; i < size; i++){
        for(j=0; j<size;j++){
            r = rand()%2;
            fwrite(&(valores[r]),sizeof(int),1,f);
            //fwrite(&(matriz[i][j]),sizeof(int),1,f);
        }
        
    }
}

int main(){

    srand(time(NULL));   // Initialization, should only be called once.
    int filas=1024;
    createFile("1024.raw",filas);
    return 0;

}