#ifndef NIVEL_H
#define NIVEL_H
#include "plataforma.h"
#define CANT_PLAT 10

typedef struct nivel{
	plataforma_t* plataformas[CANT_PLAT];
	unsigned int cantidad;
	float piso;
}nivel_t;

nivel_t* nivel_crear(float y);
void nivel_destruir(nivel_t* n);
void nivel_agregarPlataforma(nivel_t* n, plataforma_t* p);
void ordenarNiveles(nivel_t** niveles, unsigned int cantidad);

#endif
