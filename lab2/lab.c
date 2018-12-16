#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

//Estructura para guardar matriz de adyacencias y dimensión de esta matriz o cantidad de nodos
struct adjM{
    int n;
    int** matrix;
};
typedef struct adjM AdjM;

AdjM matriz;
int* visitados;
int* mejorRuta;
int costo = 0;


//Función que ingresa valores a la matriz de adyacencia 
//Entrada: String con valores de la forma NodoX NodoY Distancia
void asignarValor(char* str){
    char a[4];
    char b[4];
    char c[4];
    int i;
    int j = 0; int k = 0;
    for(i = 0; i<strlen(str);i++){
        if(str[i] == ' '){
            j++;
            k = 0;
        }
        else if(j==0){
            a[k] = str[i]; 
            k++;
        }
        else if(j==1){
            b[k] = str[i];
            k++; 
        }
        else if(j==2 && str[i]){
            c[k] = str[i];
            k++; 
        }
    }
    int aux1 = atoi(a); int aux2 = atoi(b); int aux3 = atoi(c);
    costo = costo + aux3;
    matriz.matrix[aux1-1][aux2-1] = aux3;
    matriz.matrix[aux2-1][aux1-1] = aux3;   
}

//Función que lee archivo de entrada con valores iniciales
//Entrada: Nombre de archivo de texto
void leerArchivo(char* nombre){
    FILE *fp;
    char *str = (char*)malloc(sizeof(char)*9);
    fp = fopen(nombre, "r");
    int i = 0;
    int j = 0;
    while (fgets(str, 20, fp) != NULL){
        if( i == 0){
            matriz.n = atoi(str);
            matriz.matrix = (int**)malloc(sizeof(int*)*matriz.n);
            for(j = 0; j<matriz.n;j++){
                    matriz.matrix[j] = (int*)malloc(sizeof(int)*matriz.n);
            }
        }
        else{
            asignarValor(str);  
        }
        i++;
    }
    fclose(fp);
}

//Función que agrega un elemento a un arreglo.
//Entradas: Arreglo, valor entero.
void agregar(int* visitados, int nodo){
    int i = 0;
    for(i ; i<matriz.n;i++){
        if(visitados[i] == -1){
            visitados[i] = nodo;
            return;
        }
    }
}

//Función que indica si un nodo ha sido visitado o no.
//Entrada: Arreglo con nodos (valores enteros), dimensión del arreglo, y nodo.
int visitado(int* arreglo, int n, int nodo){
    int i = 0;
    for(i ; i<n; i++){
        if(arreglo[i] == nodo){
            return true;
        }
    }
    return false;
}

//Función que indica si un arreglo está vacío o no
//Entrada: Arreglo, dimensión del arreglo.
int vacio(int* arreglo, int n){
    int a = 0;
    int i = 0;
    for(i ; i<n; i++){
        if(arreglo[i] == -1){
            a++;
        }
    }
    if(a == n){
        return true;
    }
    else{
        return false;
    }
}

//Función que elimina un elemento de un arreglo
//Entrada: Arreglo, dimensión del arreglo, elemento.
void eliminar(int* matriz, int n, int elem){
    int i = 0;
    for(i ; i<n; i++){
        if(matriz[i] == elem){
            matriz[i] = -1;
            return;
        }
    }
}

//Función que copia elementos de un arreglo en el arreglo mejorRuta
//Entrada: arreglo a copiar, dimensión arreglo.
void copiar( int* arreglo, int n){
    int i = 0;
    int j;
    for(i ; i<n; i++){
        j = 0;
        for(j ; j<n ; j++){
            if(j == arreglo[i]){
                mejorRuta[i] = j;
                j=n;
            }
        }
    }
}

//Función que rellena un arreglo con el valor -1
//Entrada: arreglo, dimension del arreglo.
void rellenarArreglo(int* arreglo, int n ){
    int i= 0;
    for(i ;i<n;i++){
        arreglo[i] = -1;
    }
}

//Función que se encarga de mostrar el estado de un recorrido al agregar un nodo.
void printCurrent(int* visitados, int distanciaActual, int nodo, int distanciaOptima){
    #ifdef DEBUG
    if(visitado(visitados, matriz.n, -1) == false){
        printf("------------------------------------------\n");
        printf("Nodo agregado: %d\n", nodo+1);
        printf("Posible solución: ");
        for(int i = 0; i<matriz.n; i++){
            if(visitados[i]!=-1)
                printf("%d ", visitados[i]+1);
        }
        printf("\n");
        printf("Posible distancia óptima: %d\n", distanciaActual);
        printf("Presione enter para continuar...");
        while(getchar()!='\n');
        printf("------------------------------------------\n");
        return;    
    }
    printf("------------------------------------------\n");
    printf("Nodo agregado: %d\n", nodo+1);
    printf("Visitados: ");
    for(int i = 0; i<matriz.n; i++){
        if(visitados[i]!=-1)
            printf("%d ", visitados[i]+1);
    }
    printf("\n");
    printf("Distancia Actual: %d\n", distanciaActual);
    printf("Distancia optima: %d\n", distanciaOptima);
    printf("Presione enter para continuar...");
    while(getchar()!='\n');
    printf("------------------------------------------\n");
    #endif
}


//Función que se encarga de realizar un recorrido mediante backtracking para encontrar
//el camino mínimo.
//Entrada: Arreglo con nodos visitados, nodo en el que se está, distancia actual, nivel del recorrido
void backtracking(int* visitados, int nodo, int distanciaActual, int nivel){
    if(nivel == matriz.n){
        costo = distanciaActual;    
        copiar(visitados, matriz.n);
        return;
    }
    else{
        int aux = nivel;
        for(int i = 0; i<matriz.n; i++){
            if(nivel == 0 && vacio(visitados, matriz.n)==true){
                agregar(visitados,i);
                int auxiliar = nivel + 1;
                printCurrent(visitados,distanciaActual,i,costo);
                backtracking(visitados, i, 0, auxiliar);
                rellenarArreglo(visitados,matriz.n);

            }
            else if(nodo != i && visitado(visitados, matriz.n, i) == false){
                if(distanciaActual + matriz.matrix[nodo][i] < costo){
                    agregar(visitados,i);    
                    int auxiliar= nivel+1;
                    printCurrent(visitados,distanciaActual + matriz.matrix[nodo][i],i,costo);
                    backtracking(visitados, i, distanciaActual + matriz.matrix[nodo][i], auxiliar );
                    eliminar(visitados,matriz.n,i);
                    
                } 
            }
        }
    }

}

//Función que se encarga de generar el archivo de salida con el resultado
void archivoSalida(){
    FILE *fp;
    fp = fopen("Salida.out", "w");
    fprintf(fp,"%d\n", costo);
    fprintf(fp,"0-");
    int i = 0;
    for(i; i<matriz.n;i++){
        fprintf(fp,"%d-",mejorRuta[i]+1);
    }
    fprintf(fp,"0");
    fclose(fp);

}


int main(){
    char* nombre = malloc(sizeof(char)*20);
    printf("Ingrese nombre archivo de texto con datos iniciales: ");
    scanf("%s", nombre);
    leerArchivo(nombre);
    visitados = (int*)malloc(sizeof(int)*matriz.n);
    rellenarArreglo(visitados, matriz.n);
    mejorRuta = (int*)malloc(sizeof(int)*matriz.n);
    backtracking(visitados, 0, 0,0);
    archivoSalida();
}