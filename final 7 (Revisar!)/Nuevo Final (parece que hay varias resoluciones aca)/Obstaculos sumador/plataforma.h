#ifndef PLATAFORMA_H
#define PLATAFORMA_H
#include <stdbool.h>

typedef struct plataforma{
	float xi, xf;
	bool tiene_obs;
	float x_obs;
} plataforma_t;

plataforma_t* plataforma_crear(float prim, float sec);
void plataforma_destruir(plataforma_t* p);
void plataforma_obtener_x(plataforma_t* p, float* x, float* y);
bool plataforma_caeBarril(plataforma_t* p, float x);
void plataforma_agregarObstaculo(plataforma_t*, float x);
bool plataforma_tiene_obstaculo(plataforma_t*, float, float);

#endif
