/* strtok example */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Estructura que contiene los datos para cada problema a solucionar.
struct datos{
    int iniciales[2];
    int *contenedores;
    int* solucion;
    int cantidadVuelos;
};
typedef struct datos Datos;

Datos* test;
int total;


//Función que lee archivo de entrada.
//Entrada: String con nombre de archivo
//Salida: 
void leerArchivo(char* nombre){
    FILE* fp;
    fp = fopen(nombre, "r");
    if(fp==NULL){
        exit(1);
    }
    char* str = (char*)malloc(sizeof(char)*40);
    int i = 0;
    int j = 0;
    int k = 0;
    while(fgets(str,40,fp)!=NULL){
        char* aux = malloc(sizeof(char)*100);
        aux = strtok(str," ");
        while(aux!=NULL){
            if(i == 0){
                total = atoi(aux);
                test = malloc(sizeof(Datos)*total);
            }
            if(i == 1){    
                test[k].iniciales[j] = atoi(aux);
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
            test[k].cantidadVuelos = 0;
            i = 1;
            k++;
        }
        j = 0;
    }
    fclose(fp);
}

//Función que cambia todos los valores de un arreglo de enteros a -1
//Entrada: Arreglo de enteros, largo del arreglo
//Salida:
void vaciar(int* a, int n){
    for(int i = 0; i<n; i++){
        a[i] = -1;
    }
}

//Función que agrega un elemento al final de un arreglo
//Entrada: Arreglo de enteros, largo del arreglo, elemento a agregar
//Salida:
void add(int* a, int n, int x){
    for(int i = 0; i<n; i++){
        if(a[i]==-1){
            a[i] = x;
            return;
        }
    }
}

//Funcion que indica el estado de cada combinación que se genera para 
//solucionar el problema. Se presenta el número de problema, la capacidad del avión
//la capacidad de cada barril, y si estos cumplen o no con que sean mayor o iguales al sumarse
//a la capacidad del avión
//Entradas: número de problema, barril a, barril b, capacidad del avión.
void printCurrent(int prob, int a, int b, int capacidad ){
    #ifdef DEBUG
        printf("Problema: %d\n", prob);
        printf("Capacidad avion: %d\n", capacidad);
        printf("Barril 1: %d\n", a);
        printf("Barril 2: %d\n", b);
        printf("Suma barriles: %d\n", a+b);
        if(a+b>=capacidad){
            printf("Cumple\n");
        }
        else{
            printf("No cumple\n");
        }
        while(getchar()!='\n');
    #endif
}

//Función que realiza el algoritmo goloso, para esto,
//dado un arreglo de valores entero, en base a un doble for se originan las combinaciones de estos
//valores juntandolos en parejas, y al encontrar una pareja que cumpla con una condición dada
//(para este problema, que la suma del par sea mayor o igual a la capacidad del avion)
//se considera solución y se agrega al arreglo solución de la estructura.
//Entrada: Un entero que indica el problema a resolver, el cual está contenido en la
//          variable global test, la cual es un arreglo de estructuras Datos, que contiene
//          los datos para cada problema.
//Salida:
void goloso(int k){
    test[k].solucion = malloc(sizeof(int)*test[k].iniciales[0]);
    vaciar(test[k].solucion, test[k].iniciales[0]);
    for(int i = 0; i<test[k].iniciales[0]; i++){
        int posible = test[k].contenedores[i];
        if(posible!=-1){
            for(int j = 0; j<test[k].iniciales[0]; j++){
                int posible2 = test[k].contenedores[j];
                if(posible2!=-1 && i!=j){
                    printCurrent(k,posible,posible2,test[k].iniciales[1]);
                    if(posible2+posible>=test[k].iniciales[1]){
                        add(test[k].solucion, test[k].iniciales[0], posible);
                        add(test[k].solucion, test[k].iniciales[0], posible2);
                        test[k].contenedores[i] = -1;
                        test[k].contenedores[j] = -1;
                        test[k].cantidadVuelos++;
                        j = test[k].iniciales[0];
                    }
                }
            }
        }   
    }
}

//Función que se llama para aplicar goloso a todos los problemas
void generarSoluciones(){
    for(int i = 0; i<total; i++){
        goloso(i);
    }
}

//Función que guarda las soluciones en un archivo de texto
//Entrada: Nombre de archivo de texto de salida.
void guardarSol(char* salida){
    FILE* fp;
    fp = fopen(salida, "w");
    if(fp==NULL){
        exit(1);
    }
    for(int i = 0; i<total; i++){
        fprintf(fp, "%d\n", test[i].cantidadVuelos);
        for(int j = 0; j<test[i].iniciales[0]; j++){
            if(test[i].solucion[j] == -1){
                j = test[i].iniciales[0];
            }
            else if( j != 0 && j%2 == 0){
                fprintf(fp, "|| %d ", test[i].solucion[j]);
            }
            else{
                fprintf(fp,"%d ", test[i].solucion[j]);
            }
        }
        fprintf(fp,"\n---\n");  
    }
    fclose(fp);
    
}


int main(int argc, char **argv)
{
    if(argc > 3){
        printf("Faltan argumentos en la línea de ejecución del programa\n");
        exit(1);
    }
    else if(argc < 3){
        printf("Sobran argumentos en la línea de ejecución del programa\n");
        exit(1);
    }
    leerArchivo(argv[1]);
    generarSoluciones();
    guardarSol(argv[2]);
    return 0;
}