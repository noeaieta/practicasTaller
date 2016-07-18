#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <stdio.h>
#include <stdlib.h>

typedef struct plataforma plataforma_t;

plataforma_t* crear_plat ( float xi, float xf, float yNivel );

float plat_obtener_xi(plataforma_t* plat);
float plat_obtener_xf(plataforma_t* plat);

bool aterrizaEnPlat(plataforma_t* p, float x);

#endif //PLATAFORMA_H
