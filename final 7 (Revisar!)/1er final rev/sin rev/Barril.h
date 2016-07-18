#ifndef BARRIL_H
#define BARRIL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct barril barril_t;

barril_t* crear_barril ( char* id, float xi, float yi );
void ordenarBarriles (  barril_t** barriles , int cant );
void asignarDistMin(barril_t*, float d);

float barril_obtener_xi(barril_t* barril);
float barril_obtener_yi(barril_t* barril);

#endif //BARRIL_H
