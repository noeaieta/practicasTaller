#ifndef BARRIL_H
#define BARRIL_H
#include "escenario.h"
#include <stdio.h>

typedef struct barril{
	char* codigo;
	float x ,y;
	float distancia;
	float tiempo;
}barril_t;

barril_t* barril_crear(const char*, float, float);
void barril_destruir(barril_t*);
void barril_imprimir(barril_t* b, FILE* archivo);
void barril_ordenarPorDistancia(barril_t** vector, unsigned int cantidad);
void barril_ordenarPorTiempo(barril_t** vector, unsigned int cantidad);


#endif
