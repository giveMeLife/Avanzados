#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Estructura usada para guardar los datos iniciales*/
struct datos{
    int ctidad_utilidades;
    int* utilidades;
    int* precios;
    int presupuesto;
}; typedef struct datos Datos;

/*Estructura usada para guardar los datos de cada combinación*/
struct infoCombinacion{
    int total;
    int utilidad; 
    int indice;
    int cumple;
}; typedef struct infoCombinacion InfoCombinacion;

Datos data;
InfoCombinacion* infoCombinaciones;

/*Función que corta un string al encontrar un espacio
Entrada: String a ser cortado y un arreglo de strings para guardar cada uno
Salida: Void
*/
void split(char* str, char** words){
    int j = 0;
    int k = 0;
    for(int i = 0; i<strlen(str); i++){
        if(str[i] == ' '){
            j++;
            k = 0;
        }
        else if(str[i] == '\n'){
            i = strlen(str);
        }
        else{
            words[j][k] = str[i];
            k++;
        }
    }
}

/*Función que lee archivo de texto y obtiene datos iniciales
Entrada: String con nombre de archivo a ser abierto.
*/
void leerArchivo(char* nombre){
    FILE *fp;
    char *str = (char*)malloc(sizeof(char)*20);
    char** palabras = (char**)malloc(sizeof(char*)*2);
    palabras[0] = (char*)malloc(sizeof(char)*20);
    palabras[1] = (char*)malloc(sizeof(char)*20);
    fp = fopen(nombre, "r");
    if (fp == NULL){
        printf("No se puede abrir archivo: %s\n",nombre);
        exit(1);
    }
    int i = 0;
    while (fgets(str, 20, fp) != NULL){
        if(i == 0){
            data.presupuesto = atoi(str);
            i++;
        }
        else if(i == 1){
            data.ctidad_utilidades = atoi(str);
            data.utilidades = (int*)malloc(sizeof(int)*data.ctidad_utilidades);
            data.precios = (int*)malloc(sizeof(int)*data.ctidad_utilidades);
            i++;
        }
        else{
            split(str,palabras);
            data.utilidades[i-2] = atoi(palabras[1]);
            data.precios[i-2] = atoi(palabras[0]);
            free(palabras[0]); free(palabras[1]);
            palabras[0] = (char*)malloc(sizeof(char)*20); palabras[1] = (char*)malloc(sizeof(char)*20);
            i++;
        }
    }
    free(palabras[0]); free(palabras[1]);    
    fclose(fp);
    

}

/*Función que calcula la potencia de un número
Entrada: int a (base), int b(exponente)
Salida: un entero que es el resultado de la potencia
*/
int potencia(int a, int b){
    int aux = a;
    for(int i = 1; i<b; i++){
        aux = aux*a;
    }
    return aux;
}


/*Función que genera un arreglo de 2^n*n
Entrada: Cantidad de elementos que puede tener una combinacion (int elementos)
Salida: arreglo del tipo int[2^elementos][elementos]
*/
int** generarArregloUtilidades(int elementos){
    int filas = potencia(2,elementos);
    infoCombinaciones = (InfoCombinacion*)malloc(sizeof(InfoCombinacion)*filas);
    int **arr = (int**)malloc(sizeof(int*)*filas);
    for(int i = 0; i<filas; i++){
        arr[i] = (int*)malloc(sizeof(int)*elementos);
        for(int j = 0; j<elementos; j++){
            arr[i][j] = -1;
        }
    }
    return arr;
}

/*Función que se encarga de obtener el índice de algun arreglo que aún no se ha llenado con elementos
Entrada: Arreglo bidimensional de eneteros.
Salida: Un entero que indica el índice del arreglo que no ha sido llenado. -1 si están todos llenos
*/
int obtenerIndice(int** a, int cantidad){
    for(int i = 0; i<potencia(2,cantidad);i++){
        if(a[i][0] == -1){
            return i;
        }
    }
    return -1;
}

/*Función que copia un array en otro
Entradas: arreglo bidimensional en donde se desea copiar arreglo, arreglo a copiar, cantidad de elementos
por arreglo
Salida: void
*/
void copiarArray(int **a, int* b, int elementos){
    int indice = obtenerIndice(a,elementos);
    for(int i = 0; i<elementos; i++){
        if(b[i] == 0){
            a[indice][i] = 0;
        }
        else{
            a[indice][i] = 1;
        }
    }
}

/*Función que se encarga de generar un arreglo bidimensional con todas las combinaciones de 0 y 1
posibles dada una cantidad a ser combinada (genera una tabla de números binarios de n bits de 0 a n-1)
Entrada: Cantidad de números bits, cantidad de números bits, arreglo de tamaño n, index=0, arreglo bidimensional
donde se guarda la tabla*/
void recursivo(int n, int a,  int* arr, int index, int** mochilas){
    if(n == 0){
        copiarArray(mochilas, arr,a);
        return;
    }
    if( n == a){
        for(int i = 0; i<2; i++){
            arr[0] = i;
            recursivo(n-1,a,arr, index+1,mochilas);
        }
    }
    else{
        for(int i = 0; i<2; i++){
            arr[index] = i;
            recursivo(n-1,a,arr,index+1,mochilas);
        }
    }
    
}

/*Función que se encarga de imprimir todas las combinaciones que se generan*/
void printCurrent(int** combinacion, int* utilidades, int ctidad_utilidades ){
    #ifdef DEBUG
    for(int i = 0; i<potencia(2,ctidad_utilidades); i++){
        printf("Combinacion: ");
        for(int j = 0; j<ctidad_utilidades; j++){
            printf("%d ", combinacion[i][j]*utilidades[j]);
        }
        printf("Utilidad: %d , Precio: %d \n", infoCombinaciones[i].utilidad, infoCombinaciones[i].total);
        while(getchar()!='\n');
    }
    #endif
}

/*Función que se encarga de obtener los datos para cada combinación de inversiones
Entradas: Arreglo bidimensional con las combinaciones, arreglo con las utilidades de cada inversión
arreglo con el precio de cada inversión, y la cantidad de utilidades existentes*/
void obtenerDatos(int** combinaciones,int* utilidades, int* precios, int ctidad_utilidades){
    for(int i = 0; i <potencia(2,ctidad_utilidades); i++){
        infoCombinaciones[i].total = 0;
        infoCombinaciones[i].utilidad = 0;
        for(int j = 0; j<ctidad_utilidades; j++){
            infoCombinaciones[i].total = infoCombinaciones[i].total + combinaciones[i][j]*precios[j];
            infoCombinaciones[i].utilidad = infoCombinaciones[i].utilidad + combinaciones[i][j]*utilidades[j];
        }
        infoCombinaciones[i].indice = i;
    }
    
}


/*Función que se encarga de obtener la solución óptima de todas las combinaciones
Entrada: Número entero con el presupuesto y número entero que indica la cantidad de inversiones.*/
InfoCombinacion obtenerSolucion(int presupuesto, int ctidad_utilidades){
    InfoCombinacion solucion;
    int max = 0; 
    for(int i = 0; i < potencia(2,ctidad_utilidades); i++){
        if(infoCombinaciones[i].total<=presupuesto){
            if(infoCombinaciones[i].utilidad > max){
                solucion = infoCombinaciones[i];
                max = infoCombinaciones[i].utilidad;
            }
        }
        
    }
    return solucion;
}

/*Función que se encarga de escribir la solución en un archivo de texto
Entrada: solución, arreglo con los precios de cada inversión, arreglo con la combinación óptima
y la cantidad de inversiones.
*/
void escribirSolucion(InfoCombinacion infoSol, int* precios, int* combinacion, int ctidad_utilidades){
    FILE *fp;
    fp = fopen("solucion.txt", "w");
    fprintf(fp, "%d %d\n", infoSol.total, infoSol.utilidad );
    for(int j = 0; j<ctidad_utilidades; j++){
        if(combinacion[j]*precios[j]!=0)
            fprintf(fp,"%d\n", combinacion[j]*precios[j]);
    }
    fclose(fp);
}

/*Función en la que se realizan todos los procedimientos para generar la solución.
Entradas: entero que es la cantidad de inversiones, entero que es el presupuesto, un arreglo de enteros
con las utilidades de cada inversión, y un arreglo con los precios de cada inversión
*/
int* bruteForce(int ctidad_utilidades,int presupuesto, int* utilidades, int* precios ){
    int **combinaciones = generarArregloUtilidades(ctidad_utilidades);
    int arr[ctidad_utilidades];
    recursivo(ctidad_utilidades,ctidad_utilidades,arr,0,combinaciones);
    obtenerDatos(combinaciones, utilidades, precios, ctidad_utilidades);
    printCurrent(combinaciones,utilidades,ctidad_utilidades);
    InfoCombinacion solucion = obtenerSolucion(presupuesto, ctidad_utilidades);
    escribirSolucion(solucion,precios,combinaciones[solucion.indice],ctidad_utilidades);

    int* a;
    return a;
}

int main(){
    char* nombre = malloc(sizeof(char)*20);
    printf("Ingrese nombre archivo de texto con datos iniciales: ");
    scanf("%s", nombre);
    leerArchivo(nombre);
    int* a = bruteForce(data.ctidad_utilidades, data.presupuesto, data.utilidades, data.precios);
    printf("\nPrograma finalizado exitosamente\n");
    return 0;
}