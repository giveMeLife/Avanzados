/* strtok example */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct datos{
    int iniciales[2];
    int *contenedores;
    int* solucion;
};
typedef struct datos Datos;

Datos* test;
int total;

void leerArchivo(char* nombre){
    FILE* fp;
    fp = fopen(nombre, "r");
    char* str = (char*)malloc(sizeof(char)*40);
    int i = 0;
    int j = 0;
    int k = 0;
    while(fgets(str,40,fp)!=NULL){
        char* aux;
        aux = strtok(str," ");
        while(aux!=NULL){
            if(i == 0){
                total = atoi(aux);
                test = malloc(sizeof(Datos)*total);
            }
            if(i == 1){    
                test[k].iniciales[j] = (int)atoi(aux);
                if(j == 0){
                    test[k].contenedores = (int*)malloc(sizeof(int)*test[k].iniciales[j]);
                }
                j++;    
            }
            else if(i == 2){
                test[k].contenedores[j] = atoi(aux);
                j++;
            }
            aux = strtok(NULL," ");
        }
        free(aux);
        i++;
        if(i == 3){
            i = 1;
            k++;
        }
        j = 0;
    }
}

void vaciar(int* a, int n){
    for(int i = 0; i<n; i++){
        a[i] = -1;
    }
}

void add(int* a, int n, int x){
    for(int i = 0; i<n; i++){
        if(a[i]==-1){
            a[i] = x;
            return;
        }
    }
}

void goloso(int k){
    test[k].solucion = malloc(sizeof(int)*test[k].iniciales[0]);
    vaciar(test[k].solucion, test[k].iniciales[0]);
    for(int i = 0; i<test[k].iniciales[0]; i++){
        int posible = test[k].contenedores[i];
        if(posible!=-1){
            for(int j = 0; j<test[k].iniciales[0]; j++){
                int posible2 = test[k].contenedores[j];
                if(posible2!=-1 && i!=j){
                    if(posible2+posible>=test[k].iniciales[1]){
                        add(test[k].solucion, test[k].iniciales[0], posible);
                        add(test[k].solucion, test[k].iniciales[0], posible2);
                        test[k].contenedores[i] = -1;
                        test[k].contenedores[j] = -1;
                        j = test[k].iniciales[k];
                    }
                }
            }
        }    
    }
}

void generarSoluciones(){
    for(int i = 0; i<total; i++){
        goloso(i);
        for(int j = 0; j<test[i].iniciales[0]; j++){
            if(test[i].solucion[j] != -1){
                printf("%d ",test[i].solucion[j]);
            }
            
        }
        printf("\n");
    }
}

void imprimirSol(){
    for(int i = 0; i<total; i++){

    }
}


int main (int argc, char **argv)
{
    leerArchivo(argv[1]);
    generarSoluciones();
    printf("\n");
    return 0;
}