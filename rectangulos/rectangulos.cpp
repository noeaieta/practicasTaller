#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>

#define MAX_RECTANGULOS 20
#define MAX_RESULTADOS 20

using namespace std;

struct rectangulo{
	string id;
	float x1;
	float y1;
	float x2;
	float y2;
};

struct resultado{
	float distancia;
	float area;
	string id1;
	string id2;
	string id3;
};

bool hayInterseccion(rectangulo a, rectangulo b){
	if((a.x1 < b.x2) && (a.x2 > b.x1) && (a.y1 < b.y2) && (a.y2 > b.y1)){
		return true;
	}
	return false;
}

bool interseca(rectangulo r1, rectangulo r2, rectangulo r3){
	//Verifico que intersequen 
	if ((hayInterseccion(r1,r2)) && (hayInterseccion(r2,r3)) && (hayInterseccion(r3,r1))){
		return true;
	}
	return false;
}

float buscarMaximo(float num1, float num2, float num3){
	float maximo = 0;
	float numeros[3];

	numeros[0] = num1;
	numeros[1] = num2;
	numeros[2] = num3;

	for (int i = 0; i < 3; i++){
		if(numeros[i] >= maximo){
			maximo = numeros[i];
		} 
	}

	return maximo;
}

float buscarMinimo(float num1, float num2, float num3){
	float minimo = 1000;
	float numeros[3];

	numeros[0] = num1;
	numeros[1] = num2;
	numeros[2] = num3;

	for (int i = 0; i < 3; i++){
		if(numeros[i] <= minimo){
			minimo = numeros[i];
		} 
	}

	return minimo;
}

float calcularArea(rectangulo r1,rectangulo r2,rectangulo r3){
	float x1 = buscarMaximo(r1.x1,r2.x1,r3.x1);
	float x2 = buscarMinimo(r1.x2,r2.x2,r3.x2);
	float y1 = buscarMaximo(r1.y1,r2.y1,r3.y1);
	float y2 = buscarMinimo(r1.y2,r2.y2,r3.y2);

	float base = abs(x2-x1);
	float altura = abs(y2-y1);

	return base*altura;
}

float calcularDistancia(rectangulo r1, rectangulo r2, rectangulo r3){
	float x1 = buscarMaximo(r1.x1,r2.x1,r3.x1);
	float y2 = buscarMinimo(r1.y2,r2.y2,r3.y2);

	return sqrt(x1*x1+y2*y2);
}

int compar(const void* a, const void* b){
	if((*(resultado*)a).distancia > (*(resultado*)b).distancia) return 1;
	if((*(resultado*)a).distancia == (*(resultado*)b).distancia) return 0;
	if((*(resultado*)a).distancia < (*(resultado*)b).distancia) return -1;
}

int main(){
	ifstream aRectangulos;
	aRectangulos.open("rectangulos.txt");

	rectangulo* rectangulos = new rectangulo[MAX_RECTANGULOS];
	resultado* resultados = new resultado[MAX_RESULTADOS];

	int cantidadRectangulos = 0;

	if(aRectangulos){
		int i = 0;
		while(!aRectangulos.eof()){
			string id,x1,x2,y1,y2;
			getline(aRectangulos,id,',');
			rectangulos[i].id = id;
			getline(aRectangulos,x1,',');
			rectangulos[i].x1 = atof(x1.c_str());
			getline(aRectangulos,y1,',');
			rectangulos[i].y1 = atof(y1.c_str());
			getline(aRectangulos,x2,',');
			rectangulos[i].x2 = atof(x2.c_str());
			getline(aRectangulos,y2,'\n');
			rectangulos[i].y2 = atof(y2.c_str());
			i++;
		}
		cantidadRectangulos = i;
	}
	
	int cantidadResultados = 0;

	for(int i = 0; i < cantidadRectangulos; i++){
		for(int j = 0; j < cantidadRectangulos; j++){
			for(int k = 0; k < cantidadRectangulos; k++){
				if((i!=j) && (j!=k) && (i!=k)){
					if (interseca(rectangulos[i],rectangulos[j],rectangulos[k])){
						resultados[cantidadResultados].id1 = rectangulos[i].id;
						resultados[cantidadResultados].id2 = rectangulos[j].id;
						resultados[cantidadResultados].id3 = rectangulos[k].id;
						resultados[cantidadResultados].area = calcularArea(rectangulos[i],rectangulos[j],rectangulos[k]);
						resultados[cantidadResultados].distancia = calcularDistancia(rectangulos[i],rectangulos[j],rectangulos[k]);
						cantidadResultados++;
					}
				}
			}
		}
	}

	qsort(resultados,cantidadResultados,sizeof(resultado),compar);

	for(int i = 0; i < cantidadResultados; i++){
		//Este filtro para los repetidos es bastante croto pero es lo que se nos ocurrio... jeje
		float distancia = resultados[i].distancia;
		if(resultados[i+1].distancia != distancia){
			cout << "Intersecan: " << resultados[i].id1 << " " << resultados[i].id2 << " " << resultados[i].id3 << " " << resultados[i].area << " " << resultados[i].distancia << endl;
		}
	}

	return 0;
}
