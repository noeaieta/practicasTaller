#include "nivel.h"
#include "plataforma.h"
#include <stddef.h>
#include <stdlib.h>

nivel_t* nivel_crear(float y){
	if (y <= 0) return NULL;
	
	nivel_t* n = malloc (sizeof(nivel_t));
	if (!n) return NULL;
	
	n->cantidad = 0;
	n->piso = y;
	return n;
}

void nivel_destruir(nivel_t* n){
	if (!n) return;
	
	unsigned int i;
	for (i = 0; i < n->cantidad; i++)
		plataforma_destruir(n->plataformas[i]);
	free(n);
}

void nivel_agregarPlataforma(nivel_t* n, plataforma_t* p){
	if (!n || !p) return;
	
	if (n->cantidad == CANT_PLAT) return;
	
	n->plataformas[n->cantidad] = p;
	n->cantidad++;
}

void swap (nivel_t** a, nivel_t** b){
	nivel_t* aux = *a;
	*a = *b;
	*b = aux;
}

void ordenarNiveles(nivel_t** niveles, unsigned int cantidad){
	unsigned int i,j;
	for (i = 0; i < cantidad - 1; i++)
		for (j = i+1; j < cantidad; j++)
			if (niveles[i]->piso < niveles[j]->piso)
				swap(&niveles[i], &niveles[j]);
	
}
