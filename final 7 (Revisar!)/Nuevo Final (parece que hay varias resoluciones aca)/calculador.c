#include "escenario.h"
#include "barril.h"
#include "nivel.h"
#include "plataforma.h"
#include <stdio.h>

#define VEL_X 10
#define VEL_Y 3

float calcularDistanciaX(escenario_t*, barril_t*);

void calcular_distancia(escenario_t* escenario, barril_t* barril){
	float distancia_y = barril->y;
	
	float distancia_x = calcularDistanciaX(escenario, barril);
	
	barril->distancia = (distancia_y + distancia_x);
	float tiempo_x = distancia_x / VEL_X;
	float tiempo_y = distancia_y / VEL_Y;
	barril->tiempo = tiempo_x + tiempo_y;
}

float minimo(float, float);

float calcularDistanciaX(escenario_t* escenario, barril_t* barril){
	
	unsigned int i;
	
	for (i = 0; i < escenario->cantidad; i++){
		if (escenario->niveles[i]->piso >= barril->y)
			continue;
		
		plataforma_t** plataformas = escenario->niveles[i]->plataformas;
		unsigned int cant = escenario->niveles[i]->cantidad;
		unsigned int j;
		for (j = 0; j < cant; j++){
			if (!plataforma_caeBarril(plataformas[j], barril->x))
				continue;
			float xi, xf;
			plataforma_obtener_x(plataformas[j], &xi, &xf);
			float distanciaDer = xf - barril->x ;
			float distanciaIzq = barril->x - xi ;
			//printf("Distancia Der: %f, DistanciaIzq: %f \n", distanciaDer, distanciaIzq);
			
			barril_t* derecha = barril_crear(NULL, xf, escenario->niveles[i]->piso);
			barril_t* izquierda = barril_crear(NULL, xi, escenario->niveles[i]->piso);
			
			distanciaDer += calcularDistanciaX(escenario, derecha);
			distanciaIzq += calcularDistanciaX(escenario, izquierda);
			return (minimo (distanciaDer, distanciaIzq));
		}
	}
	return 0;
}

float minimo (float a, float b){
	if (a < b)
		return a;
	return b;
}
