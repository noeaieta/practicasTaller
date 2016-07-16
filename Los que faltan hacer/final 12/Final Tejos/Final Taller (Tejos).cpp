// Final Taller (Tejos).cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "math.h"
#include <iostream>

const int MAX_RECTANGULOS = 20;

typedef struct{
	char id[20];
	float x;
	float y;
	float ancho;
	float alto;
}Rectangulo;

typedef struct{
	char id[20];
	float x;
	float y;
	float radio;
}Circulo;

typedef struct{
	char idRect[20];
	char idCirc[20];
	float distancia;
}Choque;

int cargarRectangulos (Rectangulo* rectangulos){
	FILE* archivo;
	archivo = fopen("rectangulos.txt", "r");
	int i;
	for (i = 0; ! feof(archivo); i++){
		fscanf(archivo, "%s %f %f %f %f", rectangulos[i].id, &rectangulos[i].x, &rectangulos[i].y,
			&rectangulos[i].alto, &rectangulos[i].ancho);
	}
	fclose(archivo);
	return i;
}

int compararRectangulos(const void* a, const void* b){
	float x1 = ((Rectangulo*)a)->x;
	float x2 = ((Rectangulo*)b)->x;
	if (x1 > x2) return 1;
	if (x1 == x2) return 0;
	return -1;
}

void ordenarRectangulosDeIzqADer(Rectangulo* rectangulos, int cantidad){
	qsort(rectangulos, cantidad, sizeof(Rectangulo), compararRectangulos);
}

bool chocoPuntoContraCirculo(float x,  float y, Circulo circulo){
	return ((sqrt(pow(x-circulo.x,2)+pow(y-circulo.y,2))) < circulo.radio);
}

bool chocoLineaContraCirculo(float lineaVertical, float inf, float sup, Circulo circulo){
	return ((circulo.x + circulo.radio > lineaVertical) && (circulo.y >= inf) && (circulo.y <= sup)); 
}

bool chocaronCirculoYRectangulo(Circulo circulo, Rectangulo rectangulo){
	if (chocoPuntoContraCirculo(rectangulo.x, rectangulo.y, circulo)) return true;
	if (chocoPuntoContraCirculo(rectangulo.x, rectangulo.y + rectangulo.alto, circulo)) return true;
	if (chocoPuntoContraCirculo(rectangulo.x + rectangulo.ancho, rectangulo.y, circulo)) return true;
	if (chocoPuntoContraCirculo(rectangulo.x + rectangulo.ancho, rectangulo.y + rectangulo.alto, circulo)) return true;
	if (chocoLineaContraCirculo(rectangulo.x, rectangulo.y, rectangulo.y + rectangulo.alto, circulo)) return true;
	return false;
}

bool verificarChoqueEntreCirculoYRectangulo(Circulo circulo, Rectangulo rectangulo, float &distanciaRecorrida){
	bool termino = false;
	bool choco = false;
	float xInicial = circulo.x;
	if ((circulo.x - circulo.radio) < rectangulo.x){
		circulo.x = rectangulo.x - circulo.radio;
	}
	while (! termino){
		if (chocaronCirculoYRectangulo(circulo, rectangulo)){
			termino = true;
			choco = true;
			distanciaRecorrida = circulo.x - xInicial;
		} else {
			circulo.x = circulo.x + 0.001;
		}
		if (circulo.x > (rectangulo.x + rectangulo.ancho)) termino = true;
	}
	return choco;
}

int procesarChoques(Rectangulo* rectangulos, int cantidadRectangulos, Choque* choques){
	FILE* archivo;
	archivo = fopen("circulos.txt", "r");
	int cantidadChoques = 0;
	while (! feof(archivo)){
		Circulo circulo;
		fscanf(archivo, "%s %f %f %f", circulo.id, &circulo.x, &circulo.y, &circulo.radio);
		for (int i = 0; i < cantidadRectangulos; i++){
			float distanciaRecorrida;
			if (verificarChoqueEntreCirculoYRectangulo(circulo, rectangulos[i], distanciaRecorrida)){
				strcpy(choques[cantidadChoques].idRect, rectangulos[i].id);
				strcpy(choques[cantidadChoques].idCirc, circulo.id);
				choques[cantidadChoques].distancia = distanciaRecorrida;
				cantidadChoques++;
				break;
			}
		}
	}
	return cantidadChoques;
}

int compararChoques(const void* a, const void* b){
	float d1 = ((Choque*)a)->distancia;
	float d2 = ((Choque*)b)->distancia;
	if (d1 > d2) return 1;
	if (d1 == d2) return 0;
	return -1;
}

void ordenarChoquesPorDistancia(Choque* choques, int cantidad){
	qsort(choques, cantidad,sizeof(Choque),compararChoques);
}

void imprimirChoques(Choque* choques, int cantidadChoques){
	std::cout << "ID_CIRC\t\t\tID_RECT\t\t\tDIST\n";
	for (int i = 0; i < cantidadChoques; i++){
		std::cout << choques[i].idCirc << "\t\t\t" << choques[i].idRect << "\t\t\t" <<
			choques[i].distancia << "\n";
	}
	getchar();
}

int main(int argc, _TCHAR* argv[])
{
	Rectangulo rectangulos[MAX_RECTANGULOS];
	int cantidadRectangulos = cargarRectangulos(rectangulos);
	ordenarRectangulosDeIzqADer(rectangulos, cantidadRectangulos);
	Choque choques[MAX_RECTANGULOS];
	int cantidadChoques = procesarChoques(rectangulos, cantidadRectangulos, choques);
	ordenarChoquesPorDistancia(choques, cantidadChoques);
	imprimirChoques(choques, cantidadChoques);
	return 0;
}



