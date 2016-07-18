// ejercicio1.cpp: archivo de proyecto principal.

#include "stdafx.h"

using namespace System;
/*
int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Hola a todos");
    return 0;
}
*/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define CANTIDAD_DE_TIROS 6
#define CARACTERES_NOMBRE 15
#define CANTIDAD_DE_PARTIDAS 2
#define CANTIDAD_DE_JUGADORES 3

typedef struct Jugador{
	char nombre[CARACTERES_NOMBRE];
	int puntos[CANTIDAD_DE_PARTIDAS];
}jugador_t;

typedef struct Tiro{
	jugador_t* jugador;
	int partida;
	float x;
	float y;
}tiro_t;

typedef struct Puntaje{
	jugador_t* jugador;
	int puntaje;
}puntaje_t;

bool esJugadorNuevo(char nombre[CARACTERES_NOMBRE], jugador_t* jugadores[CANTIDAD_DE_JUGADORES]){
	for (int i=0; i<CANTIDAD_DE_JUGADORES; i++){
		if(!jugadores[i])return true;

		if(strcmp(jugadores[i]->nombre,nombre) == 0)
			return false;
	}
	return true;
}

void cargarValores(tiro_t tiros[CANTIDAD_DE_TIROS], jugador_t* jugadores[CANTIDAD_DE_JUGADORES]){
	FILE* pFile = fopen("tiros.txt","r+");
	if(pFile == NULL) return;

	int indiceJugador = 0;
	for(int i=0; i<CANTIDAD_DE_TIROS; i++){
		tiro_t tiroActual;
		char nombre[CARACTERES_NOMBRE];
		
		fscanf(pFile,"%s",nombre);
		fscanf(pFile,"%d",&(tiroActual.partida));
		fscanf(pFile,"%f",&(tiroActual.x));
		fscanf(pFile,"%f",&(tiroActual.y));

		if(i==0)
			strcpy(nombre,"pepe");

		if(esJugadorNuevo(nombre,jugadores)){
			jugador_t* nuevoJugador = (jugador_t*) malloc (sizeof(jugador_t));
			strcpy(nuevoJugador->nombre,nombre);
			
			for(int k=0;k<CANTIDAD_DE_PARTIDAS;k++)
				nuevoJugador->puntos[k] = 0;

			tiroActual.jugador = nuevoJugador;
			jugadores[indiceJugador] = nuevoJugador;
			indiceJugador++;
		}
		else{
			for(int j=0; j<CANTIDAD_DE_JUGADORES; j++){
				if(strcmp(jugadores[j]->nombre, nombre) == 0){
					tiroActual.jugador = jugadores[j];
					break;
				}
			}
		}
		tiros[i] = tiroActual;
	}
	fclose(pFile);
}

int obtenerPuntaje(float x, float y){
	if(x<(float)10 || x>(float)15){
		if(y<(float)10)
			return 25;
		else if(y<(float)15)
			return 50;
		else
			return 25;
	}
	else{
		if(y<(float)10)
			return 50;
		else if(y<(float)15)
			return 200;
		else
			return 50;
	}
}

void procesarDisparos(tiro_t tiros[CANTIDAD_DE_TIROS], jugador_t* jugadores[CANTIDAD_DE_JUGADORES]){

	for(int i=0; i<CANTIDAD_DE_TIROS; i++){
		
		int partida = tiros[i].partida;
		int puntaje = obtenerPuntaje(tiros[i].x,tiros[i].y);
		tiros[i].jugador->puntos[partida-1] += puntaje;
	}
}

void inicializarJugadores(jugador_t* jugadores[CANTIDAD_DE_JUGADORES]){
	for(int i=0;i<CANTIDAD_DE_JUGADORES;i++)
		jugadores[i] = NULL;
}

void generarPuntajes(jugador_t* jugadores[CANTIDAD_DE_JUGADORES], puntaje_t puntajes[CANTIDAD_DE_PARTIDAS][CANTIDAD_DE_JUGADORES]){
	for(int i=0; i<CANTIDAD_DE_PARTIDAS; i++){
		for(int j=0; j<CANTIDAD_DE_JUGADORES; j++){
			puntaje_t nuevoPuntaje;
			nuevoPuntaje.jugador = jugadores[j];
			nuevoPuntaje.puntaje = jugadores[j]->puntos[i];
			puntajes[i][j] = nuevoPuntaje;
		}
	}
}

int compare(const void* p1, const void* p2){
	if( ((puntaje_t*)p1)->puntaje < ((puntaje_t*)p2)->puntaje ) return 1;
	if( ((puntaje_t*)p1)->puntaje == ((puntaje_t*)p2)->puntaje ) return 0;
	if( ((puntaje_t*)p1)->puntaje > ((puntaje_t*)p2)->puntaje ) return -1;
}

void ordenarPuntajes(puntaje_t puntajes[CANTIDAD_DE_PARTIDAS][CANTIDAD_DE_JUGADORES]){
	for(int i=0; i<CANTIDAD_DE_PARTIDAS; i++){
		qsort(puntajes[i],CANTIDAD_DE_JUGADORES,sizeof(puntaje_t),compare);
	}
}

void mostrarPuntajes(puntaje_t puntajes[CANTIDAD_DE_PARTIDAS][CANTIDAD_DE_JUGADORES]){
	for(int i=0; i<CANTIDAD_DE_PARTIDAS; i++){
		for(int j=0; j<CANTIDAD_DE_JUGADORES; j++){
			printf("%s %d\t", puntajes[i][j].jugador->nombre,puntajes[i][j].puntaje);
		}
		printf("\n");
	}
	printf("\n");
}

void generarSalida(puntaje_t puntajes[CANTIDAD_DE_PARTIDAS][CANTIDAD_DE_JUGADORES]){

	FILE* pFile = fopen("resultados.txt","w");

	fprintf(pFile,"Se mostraran los ganadores de cada partida.");
	
	for(int i=0;i<CANTIDAD_DE_PARTIDAS;i++){
		fprintf(pFile,"\nPartida %d\n",i+1);
		int puntajeMax=0;
		for(int j=0;j<CANTIDAD_DE_JUGADORES;j++){
			if(puntajes[i][j].puntaje>=puntajeMax){
				puntajeMax = puntajes[i][j].puntaje;
				fprintf(pFile,"%s %d\t", puntajes[i][j].jugador->nombre,puntajes[i][j].puntaje);
			}
		}
	}
	fclose(pFile);
}

int main(){

	tiro_t tiros[CANTIDAD_DE_TIROS];
	jugador_t* jugadores[CANTIDAD_DE_JUGADORES];
	puntaje_t puntajes[CANTIDAD_DE_PARTIDAS][CANTIDAD_DE_JUGADORES];

	inicializarJugadores(jugadores);

	cargarValores(tiros, jugadores);

	procesarDisparos(tiros,jugadores);
	
	generarPuntajes(jugadores, puntajes);

	mostrarPuntajes(puntajes);

	ordenarPuntajes(puntajes);

	mostrarPuntajes(puntajes);

	generarSalida(puntajes);
	
	for(int i=0;i<CANTIDAD_DE_JUGADORES;i++)
		free(jugadores[i]);

	int a;
	scanf("%d",&a);
	return 0;
}

/*	for(int i=0;i<CANTIDAD_DE_TIROS; i++){
		printf("%s %d %f %f \n",tiros[i].jugador->nombre,tiros[i].partida,tiros[i].x,tiros[i].y);
	}

	int a;
	scanf("%d",&a);
*/
