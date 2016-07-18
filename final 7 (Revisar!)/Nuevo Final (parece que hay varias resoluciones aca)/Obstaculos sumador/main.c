#include <stdio.h>
#include "barril.h"
#include "escenario.h"
#include "nivel.h"
#include "plataforma.h"
#include "calculador.h"
#include <stddef.h>	
#include <stdlib.h>
#include <string.h>
#define TAM 100

const char* RUTA_NIVELES = "archivoNiveles.txt";
const char* RUTA_BARRILES = "archivoBarriles.txt";
const char* SEPARADOR = ";";

char** obtenerLineas(FILE* archivo, unsigned int *cant){
       char** lineas = malloc (sizeof(char*) * TAM);
       if (!lineas) return NULL;
       char c;
       unsigned int i, j = 0;
       char* buffer; 
       unsigned int tam = TAM, tam_buffer;
       
       while (!feof(archivo)){
             i = 0;
             buffer = malloc (sizeof(char) * TAM);
             tam_buffer = TAM;
             do{
                    c = fgetc(archivo);
                    if (i == tam_buffer){
                          tam_buffer += TAM;
                          buffer = realloc(buffer, sizeof(char) * tam_buffer);
                    }
                    if (c == '\0' || c == '\n')
                       c = '\0';
                    buffer[i] = c;
                    i++;    
                }while (!feof(archivo) && c!= '\n' && c!= '\0');
                
                if (strlen(buffer) == 0||feof(archivo)){free(buffer); continue;}
                
                if ( j == tam){
                     tam += TAM;
                     lineas = realloc (lineas, sizeof(char*) * tam);
                }
                lineas[j] = buffer;
                j++;
       }
       *cant = j;
       return lineas;
}

nivel_t* parsearLineaEscenario(char* linea, const char* separador){
	char* sep;
	char* sep1;
	char* sep2;
	char* sep3;
	
	sep = strtok(linea, separador);
	sep1 = strtok(NULL, separador);
	sep2 = strtok(NULL, separador);
	sep3 = strtok(NULL, separador);
	nivel_t* nivel = nivel_crear(strtof(sep, NULL));
	while (sep1 != NULL && sep2 != NULL && sep3 != NULL){
		plataforma_t* plat = plataforma_crear(strtof(sep1, NULL), strtof(sep2, NULL));
		plataforma_agregarObstaculo(plat, strtof(sep3,NULL));
		nivel_agregarPlataforma(nivel, plat);
		sep1 = strtok(NULL, separador);
		sep2 = strtok(NULL, separador);
		sep3 = strtok(NULL, separador);
	}
	
	return nivel;
}


escenario_t* crear_escenario(){
	FILE* archivo = fopen(RUTA_NIVELES, "r");
	unsigned int cant;
	char** lineas = obtenerLineas(archivo, &cant);
	
	escenario_t* es = escenario_crear();
	unsigned int i;
	for (i = 0; i < cant; i++){
		nivel_t* nivel = parsearLineaEscenario(lineas[i], SEPARADOR);
		escenario_agregarNivel(es, nivel);
		free(lineas[i]);
	}
	escenario_ordenar(es);	
	free(lineas);
	return es;
}

void calcularEImprimir(barril_t** barriles, unsigned int cantidad, escenario_t* escenario){
	FILE* archivo = stdout;
	
	unsigned int i;
	for (i = 0; i < cantidad; i++)
		calcular_distancia(escenario, barriles[i]);
	
	barril_ordenarPorObstaculos(barriles, cantidad);
	//barril_ordenarPorTiempo(barriles, cantidad);
	
	for (i = 0; i < cantidad; i++)
		barril_imprimir(barriles[i], archivo);
}

barril_t* parsearLineaBarril(char* linea, const char* separador){
	char* sep;
	char* sep1;
	char* sep2;
	
	sep = strtok(linea, separador);
	sep1 = strtok(NULL, separador);
	sep2 = strtok(NULL, separador);
	
	barril_t* barril = barril_crear(sep, strtof(sep1, NULL), strtof(sep2, NULL));
	
	sep = strtok(NULL, separador);
	while (sep != NULL){
		barril_agregarOrden(barril, sep);
		sep = strtok(NULL, separador);
	}
		
	return barril;
}

barril_t** crear_barriles(unsigned int* cantidad){
	FILE* archivo = fopen(RUTA_BARRILES, "r");
	unsigned int cant;
	char** lineas = obtenerLineas(archivo, &cant);
	
	barril_t** barriles = malloc (sizeof(barril_t*) * TAM);
	unsigned int i;
	for (i = 0; i < cant; i++){
		barril_t* barril = parsearLineaBarril(lineas[i], SEPARADOR);
		barriles[i] = barril;
		free(lineas[i]);
	}
	
	free(lineas);
	*cantidad = cant;
	return barriles;
	
}

int main (void){
	escenario_t* escenario = crear_escenario();

	if (!escenario) return -1;
	
	unsigned int cant = 0;
	barril_t** barriles = crear_barriles(&cant);
	
	if (!barriles) return -1;
	
	calcularEImprimir(barriles, cant, escenario);
	
	return 0;
}

