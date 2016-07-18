#include "barril.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

barril_t* barril_crear(const char* nom, float x, float y){
	barril_t* b = malloc (sizeof(barril_t));
	if (!b) return NULL;
	
	b->x = x;
	b->y = y;
	b->distancia = 0;
	b->tiempo = 0;
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
	fprintf(archivo, "%s: %f; tiempo: %f\n", b->codigo, b->distancia, b->tiempo);
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
			if (vector[i]->distancia > vector[j]->distancia)
				swapeo(&(vector[i]), &(vector[j]));
	
}


void barril_ordenarPorTiempo(barril_t** vector, unsigned int cantidad){
	unsigned int i, j;
	for (i = 0; i < cantidad - 1; i++)
		for (j = i +1 ; j < cantidad; j++)
			if (vector[i]->tiempo > vector[j]->tiempo)
				swapeo(&(vector[i]), &(vector[j]));
	
}
