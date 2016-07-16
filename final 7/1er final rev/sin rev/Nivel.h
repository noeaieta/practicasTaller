#ifndef Nivel_H
#define Nivel_H

#include "Plataforma.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct nivel nivel_t;

nivel_t* crear_Nivel ( plataforma_t** plataformas, int cant, float yNivel );
void ordenarNiveles (  nivel_t** niveles , int cant );
float nivel_obtener_y(nivel_t* nivel);

int nivel_obtener_cant(nivel_t* nivel);
plataforma_t** nivel_obtener_plataformas(nivel_t* nivel);

#endif //PNIVEL_H
