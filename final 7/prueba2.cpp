// prueba2.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>

using namespace std;

struct plataforma{
	int cantidadPlataformas;
	float nivel;
	float posicionesX_iniciales[8];
	float posicionesX_finales[8];
};

struct personaje{
	string id;
	float x_inicial;
	float y_inicial;
};



float devolverRecorridoMinimo(float x_inicial,float posiciones_iniciales[],float posiciones_finales[]){
	

}

void verificarPosiciones(personaje personaje, plataforma plataforma, float *distanciaAcumulada){
	//esto solo se ejecuta si la posicion en y del personaje es mayor que el nivel
	//porque si es menor, ya paso por esa posicion en y
	if (personaje.y_inicial > plataforma.nivel){
		//asumimos que solo se pasar de un nivel al siguiente (no se saltean niveles)
		*distanciaAcumulada = *distanciaAcumulada + devolverRecorridoMinimo(personaje.x_inicial,plataforma.posicionesX_iniciales,plataforma.posicionesX_finales);
	}
}
int main(){
	int cantidadRegistros = 0;
	int cantidadPersonajes = 0;

	ifstream archivoPlataformas;
	ifstream archivoPersonajes;

	plataforma* plataformas = new plataforma[10];
	personaje* personajes = new personaje[10];

	archivoPlataformas.open("escenario.txt");
	archivoPersonajes.open("personajes.txt");
	
	//nota: la primer columna corresponde a la cantidad de niveles que tienen las 
	//plataformas. se agrego por simplicidad. sino se puede hacer algo medio
	//flashero para ordenar de a pares o sarasa. 
	if(archivoPlataformas){
		int i = 0;
		while(!archivoPlataformas.eof()){
			string cantidadPlataformasStr;
			string ynivel;
			
			getline(archivoPlataformas,cantidadPlataformasStr,',');
			plataformas[i].cantidadPlataformas = atoi(cantidadPlataformasStr.c_str());
			getline(archivoPlataformas,ynivel,',');
			plataformas[i].nivel = atof(ynivel.c_str());
			
			for (int j = 0; j < plataformas[i].cantidadPlataformas; j++){
				string xinicial,xfinal;
				char separador;
				getline(archivoPlataformas,xinicial,',');
				plataformas[i].posicionesX_iniciales[j] = atof(xinicial.c_str());
				if(j == plataformas[i].cantidadPlataformas-1){
					separador = '\n';
				} else {
					separador = ',';
				}
				getline(archivoPlataformas,xfinal,separador);
				plataformas[i].posicionesX_finales[j] = atof(xfinal.c_str());
			}
			i++;
			cantidadRegistros = i;
		}
	}

	if (archivoPersonajes){
		int i = 0;
		while(!archivoPersonajes.eof()){
			string idpersonaje,x_ini,y_ini;
			
			getline(archivoPersonajes,idpersonaje,',');
			personajes[i].id = idpersonaje;
			getline(archivoPersonajes,x_ini,',');
			personajes[i].x_inicial = atof(x_ini.c_str());
			getline(archivoPersonajes,y_ini,'\n');
			personajes[i].y_inicial = atof(y_ini.c_str());
			i++;
			cantidadPersonajes = i;
		}
	}
	
	for(int i = 0; i < cantidadRegistros; i++){
		cout << plataformas[i].nivel << endl;
		cout << plataformas[i].cantidadPlataformas << endl;
		for (int j = 0; j < plataformas[i].cantidadPlataformas; j++){
			cout << plataformas[i].posicionesX_iniciales[j] << endl;
			cout << plataformas[i].posicionesX_finales[j] << endl;
		}
	}
	
	cout << "mostrando personajes" << endl;
	for (int i = 0; i < cantidadPersonajes; i++){
		cout << personajes[i].id << endl;
		cout << personajes[i].x_inicial << endl;
		cout << personajes[i].y_inicial << endl;
	}
	
	//nota: me dio paja ordenar con qsort, los personajes estan ordenados por
	//la coordenada y_inicial y las plataformas están ordenadas por y_nivel

	for (int i = 0; i < cantidadPersonajes; i++){
		float distanciaAcumulada = 0;
		for (int j = 0; j < cantidadRegistros; j++){
			verificarPosiciones(personajes[i],plataformas[j],&distanciaAcumulada);
		}
	}

	return 0;
}