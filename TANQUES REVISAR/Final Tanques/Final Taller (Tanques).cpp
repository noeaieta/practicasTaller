// Final Taller (Tanques).cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "math.h"
#include <iostream>

const int MAX_POZOS = 15;

typedef struct{
	char id[20];
	float x;
	float y;
	float radio;
}Pozo;

typedef struct{
	char id[20];
	float xInfIzq;
	float yInfIzq;
	float xSupDer;
	float ySupDer;
}Tanque;

typedef struct{
	char idPozo[20];
	char idTanque[20];
	float distancia;
}Caida;

int obtenerPozos (Pozo* pozos){
	FILE* archivo;
	archivo = fopen("pozos.txt", "r");
	int i;
	for (i = 0; !feof(archivo); i++){
		fscanf(archivo, "%s %f %f %f", pozos[i].id, &pozos[i].x, &pozos[i].y, &pozos[i].radio);
	}
	fclose(archivo);
	return i;
}

int compararPozos(const void* a, const void* b){
	float x1 = ((Pozo*)a)->x;
	float x2 = ((Pozo*)b)->x;
	if (x1 > x2) return 1;
	if (x1 == x2) return 0;
	return -1;
}

void ordenarPozosDeIzqADer(Pozo* pozos, int cantidad){
	qsort(pozos, cantidad, sizeof(Pozo), compararPozos);
}

bool puntoDentroDePozo(float x, float y, Pozo pozo){
	return (sqrt(pow(x-pozo.x,2)+pow(y-pozo.y,2)) < pozo.radio);
}

bool tanqueDentroDePozo(Tanque tanque, Pozo pozo){
	if (! puntoDentroDePozo(tanque.xInfIzq, tanque.yInfIzq, pozo)) return false;
	if (! puntoDentroDePozo(tanque.xInfIzq, tanque.ySupDer, pozo)) return false;
	if (! puntoDentroDePozo(tanque.xSupDer, tanque.yInfIzq, pozo)) return false;
	if (! puntoDentroDePozo(tanque.xSupDer, tanque.ySupDer, pozo)) return false;
	return true;
}

bool verificarCaidaEntrePozoYTanque(Pozo pozo, Tanque tanque, float &distanciaRecorrida){
	bool choco = false;
	bool termino = false;
	float xInicial = tanque.xSupDer;
	if (tanque.xSupDer < (pozo.x - pozo.radio)){
		//Lo pongo al tanque en el comienzo del circulo
		tanque.xInfIzq = tanque.xInfIzq + ((pozo.x - pozo.radio) - tanque.xSupDer);
		tanque.xSupDer = pozo.x - pozo.radio;
	}
	while (! termino){
		if (tanqueDentroDePozo(tanque, pozo)) {
			termino = true;
			choco = true;
			distanciaRecorrida = tanque.xSupDer - xInicial;
		} else {
			tanque.xInfIzq = tanque.xInfIzq + 0.0001;			
			tanque.xSupDer = tanque.xSupDer + 0.0001;
		}
		if (tanque.xInfIzq > (pozo.x + pozo.radio)) termino = true; //Ya no hay chance de colisionar
	}
	return choco;
}

int procesarTanques(Pozo* pozos, int cantidad, Caida* caidas){
	FILE* archivo;
	archivo = fopen("tanques.txt", "r");
	int cantidadCaidas = 0;
	while (! feof(archivo)){
		Tanque tanque;
		fscanf(archivo, "%s %f %f %f %f", tanque.id, &tanque.xInfIzq, &tanque.yInfIzq,
			&tanque.xSupDer, &tanque.ySupDer);
		for (int i=0; i < cantidad; i++){
			float distanciaRecorrida;
			std::cout << "Verifico caida entre tanque " << tanque.id << " y pozo " << pozos[i].id << "\n";
			bool choco = verificarCaidaEntrePozoYTanque(pozos[i], tanque, distanciaRecorrida);
			if (choco){
				strcpy(caidas[cantidadCaidas].idPozo, pozos[i].id);
				strcpy(caidas[cantidadCaidas].idTanque, tanque.id);
				caidas[cantidadCaidas].distancia = distanciaRecorrida;
				cantidadCaidas++;
				break;
			}
		}
		std::cout << "\n";
	}
	return cantidadCaidas;
}

int compararCaidas(const void* a, const void* b){
	float d1 = ((Caida*)a)->distancia;
	float d2 = ((Caida*)b)->distancia;
	if (d1 > d2) return 1;
	if (d1 == d2) return 0;
	return -1;
}

void ordenarCaidasPorDistancia (Caida* caidas, int cantidad){
	qsort(caidas, cantidad, sizeof(Caida), compararCaidas);
}

void imprimirCaidas(Caida* caidas, int cantidad){
	std::cout << "ID_TANQ\t\t\tID_POZO\t\t\tDIST\n";
	for (int i = 0; i < cantidad; i++){
		std::cout << caidas[i].idTanque << "\t\t\t" << caidas[i].idPozo << "\t\t" <<
			caidas[i].distancia << "\n";
	}
	getchar();
}

int main(int argc, _TCHAR* argv[])
{
	Pozo pozos[MAX_POZOS];
	int cantidadPozos = obtenerPozos(pozos);
	ordenarPozosDeIzqADer(pozos, cantidadPozos);
	Caida caidas[MAX_POZOS];
	int cantidadCaidas = procesarTanques(pozos, cantidadPozos, caidas);
	ordenarCaidasPorDistancia(caidas, cantidadCaidas);
	imprimirCaidas(caidas, cantidadCaidas);
	return 0;
}

