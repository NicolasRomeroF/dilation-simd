#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int valores[2] = {0,255};

void createFile(char nombre[128], int size){
    FILE* f = fopen(nombre, "w");
    int i,r;

    for(i = 0; i < size; i++){
        r = rand()%2;
        fwrite(&(valores[r]),sizeof(int),1,f);
    }
}

int main(){

    srand(time(NULL));   // Initialization, should only be called once.
    int filas=12;
    createFile("12.raw",filas*filas);
    return 0;

}