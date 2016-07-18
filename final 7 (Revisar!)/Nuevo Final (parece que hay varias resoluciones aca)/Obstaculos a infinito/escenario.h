#ifndef ESCENARIO_H
#define ESCENARIO_H
#define CANT_NIVELES 8
#include "nivel.h"

typedef struct escenario{
	nivel_t* niveles[CANT_NIVELES];
	unsigned int cantidad;
}escenario_t;


escenario_t* escenario_crear();
void escenario_destruir(escenario_t* es);
void escenario_ordenar(escenario_t* es);
void escenario_agregarNivel(escenario_t* es, nivel_t* n);

#endif
