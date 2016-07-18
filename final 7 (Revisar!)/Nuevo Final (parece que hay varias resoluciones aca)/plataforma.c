#include "plataforma.h"
#include <stdlib.h>
#include <stddef.h>


struct plataforma{
	float xi, xf;
};

plataforma_t* plataforma_crear(float prim, float sec){
	plataforma_t* p = malloc (sizeof(plataforma_t));
	
	if (!p) return NULL;
	
	p->xi = prim;
	p->xf = sec;
	
	return p;
}

void plataforma_destruir(plataforma_t* p){
	if (p)
		free(p);
}

void plataforma_obtener_x(plataforma_t* p, float* x, float* y){
	if (x)
		*x = p->xi;
	if (y)
		*y = p->xf;
}

bool plataforma_caeBarril(plataforma_t* p, float x){
	return(x < p->xf && x > p->xi);
}
