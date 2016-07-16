#include "stdafx.h"
#include <iostream>
#include <vector>

const int MAX_PLATAFORMAS = 50;

typedef struct{
	char id[20];
}ID;

typedef struct{
	ID idPlataforma;
	float xIzq;
	float xDer;
	float y;
	int resistencia;
	std::vector<ID> choques;
}Plataforma;

typedef struct{
	ID idBala;
	float x;
	float y;
	int energia;
}Bala;

typedef struct{
	ID idBala;
	ID idPlataforma;
	float distancia;
	int resistenciaFinal;
	std::vector<ID> choques;
}Choque;

int cargarPlataformas (Plataforma* plataformas){
	FILE* archivo;
	archivo = fopen("plataformas.txt", "r");
	int i;
	for (i=0; ! feof(archivo); i++){
		fscanf(archivo, "%s %f %f %f %i", plataformas[i].idPlataforma.id, &plataformas[i].xIzq, &plataformas[i].xDer, &plataformas[i].y,
			&plataformas[i].resistencia);
	}
	fclose(archivo);
	return i;
}

int compararPlataformas(const void *a, const void *b){
	Plataforma* p = (Plataforma*) a;
	Plataforma* q = (Plataforma*) b;
	if (p->y > q->y) return 1;
	if (p->y == q->y) return 0;
	if (p->y < q->y) return -1;
}

void ordenarPlataformasPorAltura(Plataforma* plataformas, int cantidad){
	qsort(plataformas, cantidad, sizeof(Plataforma), compararPlataformas);
}

int leerBalasYProcesar(Plataforma* plataformas, Choque* choques, int cantidadPlataformas){
	FILE* archivo;
	archivo = fopen("balas.txt", "r");
	int cantidadDeChoques = 0;
	while (! feof(archivo)){
		Bala bala;
		fscanf(archivo, "%s %f %f %i", bala.idBala.id, &bala.x, &bala.y, &bala.energia);
		for (int i=0; i < cantidadPlataformas; i++){
			if (bala.y > plataformas[i].y) continue;
			if ((bala.x < plataformas[i].xDer) && (bala.x > plataformas[i].xIzq) && (plataformas[i].resistencia > 0)){
				plataformas[i].resistencia = plataformas[i].resistencia - bala.energia;
				ID idBala = bala.idBala;
				plataformas[i].choques.push_back(idBala);
				if (plataformas[i].resistencia <= 0){
					choques[cantidadDeChoques].idBala = bala.idBala;
					choques[cantidadDeChoques].idPlataforma = plataformas[i].idPlataforma;
					choques[cantidadDeChoques].distancia = (plataformas[i].y - bala.y);
					choques[cantidadDeChoques].resistenciaFinal = plataformas[i].resistencia;
					choques[cantidadDeChoques].choques = plataformas[i].choques;
					cantidadDeChoques++;
				}
				break;
			}
		}
	}
	fclose(archivo);
	return cantidadDeChoques;
}

int compararChoques(const void *a, const void *b){
	Choque* p = (Choque*) a;
	Choque* q = (Choque*) b;
	if (p->distancia > q->distancia) return 1;
	if (p->distancia == q->distancia) return 0;
	if (p->distancia < q->distancia) return -1;
}

void ordenarChoquesPorDistancia(Choque* choques, int cantidad){
	qsort(choques, cantidad, sizeof(Choque), compararChoques);
}

void imprimirChoquesPorPantalla(Choque* choques, int cantidad){
	std::cout << "ID_BALA\t\tID_PLAT\t\tDIST\t\tRESIST\t\tBALAS\n";
	for (int i = 0; i < cantidad; i++){
		std::cout << choques[i].idBala.id << "\t\t" << choques[i].idPlataforma.id << "\t\t" << choques[i].distancia << "\t\t" << choques[i].resistenciaFinal << "\t\t";
		int cant = choques[i].choques.size();
		for (int j = 0; j < cant; j++){
			std::cout << choques[i].choques[j].id << " ";
		}
		std::cout << "\n";
	}
	getchar();
}

int main(int argc, _TCHAR* argv[])
{
	Plataforma plataformas[MAX_PLATAFORMAS];
	int cantidadDePlataformas = cargarPlataformas(plataformas);
	ordenarPlataformasPorAltura(plataformas, cantidadDePlataformas);
	Choque choques[MAX_PLATAFORMAS];
	int cantidadDeChoques = leerBalasYProcesar(plataformas, choques, cantidadDePlataformas);
	ordenarChoquesPorDistancia(choques, cantidadDeChoques);
	imprimirChoquesPorPantalla(choques, cantidadDeChoques);
	return 0;
}

