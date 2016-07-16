#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <math.h>
#define MAX_ENEMIGOS 10
#define MAX_TROPA 10

using namespace std;

struct enemigo{
	int x;
	int y;
	float ataque;
	int alcance;
};

struct tropa{
	string id;
	int x_inicial;
	int y_inicial;
	float energia;
	float escudo;
	bool vivo;
	int x_acum;
};

struct resultado{
	string id_personaje;
	int celdas_recorridas;
	float tiempo;
};

void mostrarMuertos(int ciclos, tropa* tropa){
	cout << "ID TROPA: " << tropa->id << endl;
	cout << "CELDAS RECORRIDAS: " << tropa->x_acum - tropa->x_inicial << endl;
	cout << "CICLOS: " << ciclos << endl;
}

bool seVaDeRango(tropa* tropa){
	if(tropa->x_acum > 100){
		tropa->vivo = false;
		return true;
	}
	return false;
}

bool chocaConEnemigo(tropa* tropa, enemigo* enemigos, int cantidadEnemigos){
	for(int i = 0; i < cantidadEnemigos; i++){
		if(enemigos[i].x == tropa->x_acum + 1){
			return true;
		}
	}
	return false;
}

void actualizarPosiciones(tropa* tropa, enemigo* enemigos, int cantidadEnemigos){
	if(seVaDeRango(tropa)){
		tropa->x_acum = tropa->x_acum; 
	}
	if(!chocaConEnemigo(tropa, enemigos, cantidadEnemigos)){
		tropa->x_acum = tropa->x_acum + 1;
	}
}

void modificarEnergia(enemigo* enemigo, tropa* tropa, int ciclos){
	float efectividad = enemigo->ataque - tropa->escudo;
	if(tropa->escudo < enemigo->ataque){
		tropa->energia = tropa->energia - efectividad;
	}
	if(tropa->energia <= 0){
		tropa->vivo = false;
		mostrarMuertos(ciclos,tropa);
	}
}

bool ataca(enemigo enemigo, tropa tropa){
	if((enemigo.x + enemigo.alcance > tropa.x_inicial) || (enemigo.y + enemigo.alcance > tropa.y_inicial)){
		return true;
	}
	return false;
}
int main(){
	ifstream archivoEnemigos, archivoTropas;
	enemigo* enemigos = new enemigo[MAX_ENEMIGOS];
	tropa* tropas = new tropa[MAX_TROPA];
	int cantidadEnemigos = 0, cantidadTropa = 0;
	archivoEnemigos.open("enemigos.txt");
	if(archivoEnemigos){
		int i = 0;
		string x,y,ataque,alcance;
		while(!archivoEnemigos.eof()){
			getline(archivoEnemigos,x,',');
			enemigos[i].x = atoi(x.c_str());
			getline(archivoEnemigos,y,',');
			enemigos[i].y = atoi(y.c_str());
			getline(archivoEnemigos,ataque,',');
			enemigos[i].ataque = atof(ataque.c_str());
			getline(archivoEnemigos,alcance,'\n');
			enemigos[i].alcance = atoi(alcance.c_str());
			i++;
		}
		cantidadEnemigos = i;
	}
	archivoTropas.open("tropa.txt");
	if(archivoTropas){
		int j = 0;
		string id,x,y,energia,escudo;
		while(!archivoTropas.eof()){
			getline(archivoTropas,id,',');
			tropas[j].id = id;
			getline(archivoTropas,x,',');
			tropas[j].x_inicial = atoi(x.c_str());
			getline(archivoTropas,y,',');
			tropas[j].y_inicial = atoi(y.c_str());
			getline(archivoTropas,energia,',');
			tropas[j].energia = atof(energia.c_str());
			getline(archivoTropas,escudo,'\n');
			tropas[j].escudo = atof(escudo.c_str());
			tropas[j].vivo = true;
			tropas[j].x_acum = tropas[j].x_inicial;
			j++;
			}
		cantidadTropa = j;
		}
	//for(int i = 0; i < cantidadEnemigos; i++){
	//	cout << enemigos[i].x << enemigos[i].y << enemigos[i].ataque << enemigos[i].alcance << endl; 
	
//}
//	for(int i = 0; i < cantidadTropa-1; i++){
//		cout << tropas[i].id << tropas[i].x_inicial << tropas[i].y_inicial << tropas[i].energia << tropas[i].escudo << endl; 
//	}

	bool parar = false;
	int ciclos = 1;
	while(!parar){
		for(int i = 0; i < cantidadEnemigos; i++){
			for(int j = 0; j < cantidadTropa-1; j++){
				if (ataca(enemigos[i], tropas[j]) && (tropas[j].vivo)){
					modificarEnergia(&enemigos[i], &tropas[j], ciclos);
				}
				if(tropas[j].vivo){
					actualizarPosiciones(&tropas[j], enemigos, cantidadEnemigos);
				}
				if(ciclos == 100){
					parar = true;
				}
			}
		}
		ciclos++;
	}

	
	return 0;
}

