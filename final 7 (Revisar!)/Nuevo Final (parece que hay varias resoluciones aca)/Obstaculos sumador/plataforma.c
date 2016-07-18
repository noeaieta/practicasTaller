#include "plataforma.h"
#include <stdlib.h>
#include <stddef.h>


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

void plataforma_agregarObstaculo(plataforma_t* p, float x){
	if (!p) return;
	
	if (p->tiene_obs) return;
	
	if (p->xf > x && p->xi < x){
		p->tiene_obs = true;
		p->x_obs = x;
	}
		
}

bool plataforma_tiene_obstaculo(plataforma_t* p, float xi, float xf){
	if (!p) return false;
	if (!p->tiene_obs) return false;
	
	return (p->x_obs < xf && p->x_obs > xi);
}
