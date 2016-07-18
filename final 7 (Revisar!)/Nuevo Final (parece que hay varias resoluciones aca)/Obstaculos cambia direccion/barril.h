#ifndef BARRIL_H
#define BARRIL_H
#include "escenario.h"
#include <stdio.h>
#include <stdbool.h>

#define DERECHA 1
#define IZQUIERDA -1
#define MAX_ORDENES 5
#define NINGUNO 0

typedef struct barril{
	char* codigo;
	float x ,y;
	float distancia;
	bool infinito;
	int ordenes[5];
	unsigned int cant_ordenes;
}barril_t;

barril_t* barril_crear(const char*, float, float);
void barril_destruir(barril_t*);
void barril_imprimir(barril_t* b, FILE* archivo);
void barril_ordenarPorDistancia(barril_t** vector, unsigned int cantidad);
void barril_ordenarPorTiempo(barril_t** vector, unsigned int cantidad);
void barril_agregarOrden(barril_t*, const char*);


#endif
