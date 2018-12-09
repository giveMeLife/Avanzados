#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

struct adjM{
    int n;
    int** matrix;
};
typedef struct adjM AdjM;

AdjM matriz;
int* visitados;
int* mejorRuta;
int costo = 0;

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

void leerArchivo(char* nombre){
    printf("%s\n",nombre);
    FILE *fp;
    char *str = (char*)malloc(sizeof(char)*9);
    fp = fopen(nombre, "r");
    int i = 0;
    int j = 0;
    while (fgets(str, 9, fp) != NULL){
        if( i == 0){
            printf("jaja\n");
            matriz.n = atoi(str);
            printf("%d\n", matriz.n);
            matriz.matrix = (int**)malloc(sizeof(int*)*matriz.n);
            for(j = 0; j<matriz.n;j++){
                    matriz.matrix[j] = (int*)malloc(sizeof(int)*matriz.n);
            }
        }
        else{
            asignarValor(str);
            printf("asigne\n");    
        }
        i++;
        
    }
}

void agregar(int* visitados, int nodo){
    int i = 0;
    for(i ; i<matriz.n;i++){
        if(visitados[i] == -1){
            visitados[i] = nodo;
            return;
        }
    }
}

int visitado(int* arreglo, int n, int nodo){
    int i = 0;
    for(i ; i<n; i++){
        if(arreglo[i] == nodo){
            return true;
        }
    }
    return false;

}
void imprimirArreglo(int* arreglo, int n){
    int i = 0;
    for(i;i<n;i++){
        printf("%d ", arreglo[i]);
    }
    printf("\n");
}

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

void eliminar(int* matriz, int n, int elem){
    int i = 0;
    for(i ; i<n; i++){
        if(matriz[i] == elem){
            matriz[i] = -1;
            return;
        }
    }
}

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

void rellenarArreglo(int* arreglo, int n ){
    int i= 0;
    for(i ;i<n;i++){
        arreglo[i] = -1;
    }
}

void backtracking(int* visitados, int nodo, int distanciaActual, int nivel){
    if(nivel == matriz.n){
        costo = distanciaActual;
        printf("Soy arreglo: ");
        imprimirArreglo(visitados,matriz.n);
        printf("Mi costo es: %d", costo);
        copiar(visitados, matriz.n);
        printf("\n");
        return;
    }
    else{
        int aux = nivel;
        for(int i = 0; i<matriz.n; i++){
            if(nivel == 0 && vacio(visitados, matriz.n)==true){
                agregar(visitados,i);
                printf("Soy el padre: %d \n",i);
                int auxiliar = nivel + 1;
                backtracking(visitados, i, 0, auxiliar);
                rellenarArreglo(visitados,matriz.n);
                printf("desps de rellenar\n");

            }
            else if(nodo != i && visitado(visitados, matriz.n, i) == false){
                if(distanciaActual + matriz.matrix[nodo][i] < costo){
                    printf("Distancia actual: %d  Nodo: %d   Nivel: %d\n",distanciaActual,nodo,nivel);
                    agregar(visitados,i);    
                    imprimirArreglo(visitados,matriz.n);
                    int auxiliar= nivel+1;
                    backtracking(visitados, i, distanciaActual + matriz.matrix[nodo][i], auxiliar );
                    eliminar(visitados,matriz.n,i);
                    
                } 
            }
        }
    }

}


void imprimirMatriz(){
    for(int i = 0; i<matriz.n;i++){
        for(int j = 0; j<matriz.n;j++){
            printf("%d ", matriz.matrix[i][j]);
        }
        printf("\n");
    }
}


int main(){
    char* nombre = malloc(sizeof(char)*20);
    printf("Ingrese nombre archivo de texto con datos iniciales: ");
    scanf("%s", nombre);
    leerArchivo(nombre);
    printf("hola\n");
    visitados = (int*)malloc(sizeof(int)*matriz.n);
    rellenarArreglo(visitados, matriz.n);
    printf("hola\n");
    mejorRuta = (int*)malloc(sizeof(int)*matriz.n);
    imprimirMatriz();
    backtracking(visitados, 0, 0,0);
    imprimirArreglo(visitados,matriz.n);
    imprimirArreglo(mejorRuta,matriz.n);
    int a = 3;
    int b = a;
    a = 4;
    printf("%d %d\n", a,b);
}