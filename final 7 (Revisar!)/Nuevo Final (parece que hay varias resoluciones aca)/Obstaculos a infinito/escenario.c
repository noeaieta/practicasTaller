#include <stddef.h>
#include <stdlib.h>
#include "nivel.h"
#include "escenario.h"

escenario_t* escenario_crear(){
	escenario_t* es = malloc (sizeof(escenario_t));
	if (!es) return NULL;
	
	es->cantidad = 0;
	return es;
}

void escenario_destruir(escenario_t* es){
	if (!es) return;
	
	unsigned int i;
	for (i = 0; i < es->cantidad; i++)
		nivel_destruir(es->niveles[i]);
	free(es);
}

void escenario_ordenar(escenario_t* es){
	ordenarNiveles(es->niveles, es->cantidad);
}

void escenario_agregarNivel(escenario_t* es, nivel_t* n){
	if (!n || !es) return;
	
	if (es->cantidad == CANT_NIVELES) return;
	
	es->niveles[es->cantidad] = n;
	es->cantidad++;
}
