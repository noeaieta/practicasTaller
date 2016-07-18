#ifndef PLATAFORMA_H
#define PLATAFORMA_H
#include <stdbool.h>
#define INICIO 0
#define FINAL 1
#define NOHAY 0
#define MAX_OBS 5

typedef struct plataforma{
	float xi, xf;
	float x_obs[MAX_OBS];
	unsigned int cant_obs;
	int ID;
} plataforma_t;

plataforma_t* plataforma_crear(float prim, float sec);
void plataforma_destruir(plataforma_t* p);
void plataforma_obtener_x(plataforma_t* p, float* x, float* y);
bool plataforma_caeBarril(plataforma_t* p, float x);
void plataforma_agregarObstaculo(plataforma_t*, float x);
float plataforma_tiene_obstaculo(plataforma_t*, float, float, char);
void plataforma_asignarID(plataforma_t*, int);

#endif
