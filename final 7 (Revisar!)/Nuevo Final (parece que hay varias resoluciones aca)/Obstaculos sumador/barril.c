#include "barril.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define INFINITO 236

barril_t* barril_crear(const char* nom, float x, float y){
	barril_t* b = malloc (sizeof(barril_t));
	if (!b) return NULL;
	
	b->x = x;
	b->y = y;
	b->distancia = 0;
	b->cant_ordenes = 0;
	b->cant_obstaculos = 0;
	if (!nom){
		b->codigo = NULL;
		return b;
	}
	
	b->codigo = malloc (sizeof(char) * (strlen(nom) + 1));
	strcpy(b->codigo, nom);
	return b;
	
}

void barril_destruir(barril_t* b){
	if (!b) return;
	if (b->codigo)
		free(b->codigo);
	free(b);
}

void barril_imprimir(barril_t* b, FILE* archivo){
		fprintf(archivo, "%s: %u, %f\n", b->codigo, b->cant_obstaculos,b->distancia);
}

void swapeo(barril_t** a, barril_t** b){
	barril_t* aux = *a;
	*a = *b;
	*b = aux;
}

void barril_ordenarPorDistancia(barril_t** vector, unsigned int cantidad){
	unsigned int i, j;
	for (i = 0; i < cantidad - 1; i++)
		for (j = i +1 ; j < cantidad; j++)
			if ((vector[i]->distancia > vector[j]->distancia))
				swapeo(&(vector[i]), &(vector[j]));
			
	
}

void barril_ordenarPorObstaculos(barril_t** vector, unsigned int cantidad){
	unsigned int i, j;
	for (i = 0; i < cantidad - 1; i++)
		for (j = i +1 ; j < cantidad; j++)
			if ((vector[i]->cant_obstaculos < vector[j]->cant_obstaculos))
				swapeo(&(vector[i]), &(vector[j]));
			
	
}

void barril_agregarOrden(barril_t* b, const char* orden){
	
	if (!b) return;
	
	if (b->cant_ordenes == MAX_ORDENES) return;
	
	int valor = NINGUNO;
	if (strcmp(orden, "derecha") == 0)
		valor = DERECHA;
	else if (strcmp(orden, "izquierda") == 0)
		valor = IZQUIERDA;
	
	if (valor == NINGUNO) return;
	
	b->ordenes[b->cant_ordenes] = valor; 
	b->cant_ordenes++;
}
