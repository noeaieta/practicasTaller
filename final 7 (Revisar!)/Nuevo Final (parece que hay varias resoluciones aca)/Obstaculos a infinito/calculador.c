#include "escenario.h"
#include "barril.h"
#include "nivel.h"
#include "plataforma.h"
#include <stdio.h>


float calcularDistanciaX(escenario_t*, barril_t*);

float calcularDistanciaXOrdenes(escenario_t*, barril_t*, bool*);

void calcular_distancia(escenario_t* escenario, barril_t* barril){
	float distancia_y = barril->y;
	
	//float distancia_x = calcularDistanciaX(escenario, barril);
	bool inf = false;
	float distancia_x = calcularDistanciaXOrdenes(escenario, barril, &inf);
	if (!inf)
		barril->distancia = (distancia_y + distancia_x);
	else
		barril->infinito = true;

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





float calcularDistanciaXOrdenes(escenario_t* es, barril_t* barril, bool* inf){
	unsigned int i;
	

	int orden;
	unsigned int sig_orden = 0;
	*inf = false;
	float distancia = 0;
	
	barril_t* aux = barril_crear(NULL, barril->x, barril->y);
	
	for (i = 0; i < es->cantidad; i++){
		if (es->niveles[i]->piso >= aux->y)
			continue;
		
		plataforma_t** plataformas = es->niveles[i]->plataformas;
		
		unsigned int j;
		bool salir = false;;
		for (j = 0; j < es->niveles[i]->cantidad && !salir; j++){
			if (!plataforma_caeBarril(plataformas[j], aux->x))
				continue;
			orden = barril->ordenes[sig_orden];
			if (sig_orden + 1 < barril->cant_ordenes)
				sig_orden++;
			
			float distAct, x;
			if (orden == DERECHA){
				x = plataformas[j]->xf;
				distAct = x - aux->x;
				if (plataforma_tiene_obstaculo(plataformas[j], aux->x, x))
					*inf = true;
			}else{
				x = plataformas[j]->xi;
				distAct =  aux->x - x;
				if (plataforma_tiene_obstaculo(plataformas[j], x ,aux->x))
					*inf = true;
			}
			if (*inf) return 0;
			
			distancia += distAct;
			
			barril_t* aux2 = barril_crear(NULL, x , es->niveles[i]->piso);
			
			barril_destruir(aux);
			aux = aux2;
			salir = true;
		}
	}
	barril_destruir(aux);
	
	return distancia;
}
