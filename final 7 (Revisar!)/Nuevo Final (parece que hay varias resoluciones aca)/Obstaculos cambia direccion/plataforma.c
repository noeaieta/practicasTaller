#include "plataforma.h"
#include <stdlib.h>
#include <stddef.h>


plataforma_t* plataforma_crear(float prim, float sec){
	plataforma_t* p = malloc (sizeof(plataforma_t));
	
	if (!p) return NULL;
	
	p->xi = prim;
	p->xf = sec;
	p->cant_obs = 0;
	p->ID = 0;
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
	
	if (p->cant_obs == MAX_OBS) return;
	
	if (p->xf > x && p->xi < x){
		p->x_obs[p->cant_obs] = x;
		p->cant_obs++;
	}
		
}

float plataforma_tiene_obstaculo(plataforma_t* p, float xi, float xf, char cercano){
	if (!p) return false;
	unsigned int i;
	bool no_hay = true;
	unsigned d;
	for (i = 0; i < p->cant_obs; i++)
		if ((p->x_obs[i] < xf) && (p->x_obs[i] > xi)){
			no_hay = false;
			d = i;
			if (cercano == INICIO) return p->x_obs[d];
		}
	if (!no_hay) return p->x_obs[d];
	
	return NOHAY;
}

void plataforma_asignarID(plataforma_t* p, int id){
	if(!p) return;
	p->ID = id;
}
