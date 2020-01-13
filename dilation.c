/*
Laboratorio 1
Autor: Nicolas Romero Flores
Asignatura: Computacion de alto rendimiento
*/

#include "funciones.h"

int main(int argc, char **argv){
    char* sName = NULL;
    char* iName = NULL;
    int Nsize = 0;
    int Dopcion = 0;
    int cantHebras = 1;
    int index;
    int c;

    /*
    Entradas por linea de comando
    iName: nombre de la imagen de entrada
    sName: nombre de la imagen de salida secuancial
    pName: nombre de la imagen de salida con registros SIMD
    Nsize: tamano de la imagen de entrada (NxN)
    Dopcion: opcion que indica si mostrar o no por pantalla las matrices resultantes
    */

    opterr = 0;

    while ((c = getopt (argc, argv, "i:s:N:H:D:")) != -1)
        switch (c)
        {
        case 'i':
            iName = optarg;
            break;
        case 's':
            sName = optarg;
            break;
        case 'N':
            sscanf(optarg, "%d", &Nsize);
			break;
        case 'H':
            sscanf(optarg, "%d", &cantHebras);
			break;
        case 'D':
            sscanf(optarg, "%d", &Dopcion);
			break;
        case ':':
            printf("Opcion necesita un valor\n");
            break;
        case '?':
            if (optopt == 'c')
            fprintf (stderr, "Opcion -%c requiere un argumento\n", optopt);
            else if (isprint (optopt))
            fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
            else
            fprintf (stderr,
                    "Caracter desconocido`\\x%x'.\n",
                    optopt);
            return 1;
        default:
            abort ();
        }

    for (index = optind; index < argc; index++){
        printf ("Argumento sin opcion %s\n", argv[index]); 
    }

    printf("i: %s s: %s N: %d H: %d D:%d\n",iName,sName,Nsize,cantHebras,Dopcion);
              


    int size = Nsize;
    printf("Leyendo archivo...\n");
    int** imagen = leerArchivo(iName,size);
    if(imagen==NULL){
        return -1;
    }

    clock_t tSIMD;

    printf("Aplicando operacion SIMD...\n");
    tSIMD = clock();
    int** imagenDSIMD = dilationSIMD(imagen, size,cantHebras);
    tSIMD = clock() - tSIMD;
    double tiempoSIMD = ((double)tSIMD)/CLOCKS_PER_SEC;

    printf("Tiempo SIMD: %f s\n",tiempoSIMD);

    freeMatriz(imagen,size);

    if(imagenDSIMD==NULL){
        return -1;
    }

    if(Dopcion==1){
        printf("\nResultado SIMD: \n");
        printMatriz(imagenDSIMD,size);
        printf("\n");
    }
    printf("Escribiendo archivo SIMD...\n");
    createFile(sName,imagenDSIMD,size);

    printf("Tiempo SIMD: %f s\n",tiempoSIMD);

    return 0;
}