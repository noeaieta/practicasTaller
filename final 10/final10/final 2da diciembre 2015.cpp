

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

typedef struct {

	int x;
	int y;
	int alcance;
	float ataque;

} Enemigo;

typedef struct {

	char id[10];
	int x;
	int y;
	float vida;
	float escudo;
	bool vivo;
	int cantPasos;
	int ciclos;

} Tropa;

typedef struct {

	char id[10];
	int cantPasos;
	int cantCiclos;

} Muerto;

Enemigo enemigos[100];
Tropa tropas[100];
Tropa muertos[100];



int cargarEnemigos(Enemigo* e) {

	FILE* archivo = fopen("enemigos.txt", "r");

	int cantEnemigos = 0;

	for (int i = 0; !feof(archivo); i++) {

		fscanf(archivo, "%i %i %f %i", &e[i].x, &e[i].y, &e[i].ataque, &e[i].alcance);
		cantEnemigos++;
	}

	return cantEnemigos - 1;
}

int cargarTropas(Tropa* t) {

	FILE* archivo = fopen("tropas.txt", "r");

	int cantTropas = 0;

	for (int i = 0; !feof(archivo); i++) {

		cantTropas++;
		fscanf(archivo, "%s %i %i %f %f", t[i].id, &t[i].x, &t[i].y, &t[i].vida, &t[i].escudo);
		t[i].vivo = true;
		t[i].cantPasos = 0;

	}
	return cantTropas - 1;
}

bool estaEnRangoOVivo(Enemigo enemigo, Tropa tropa) {

	int distanciaX;
	int distanciaY;

	if(!tropa.vivo)
		return false;






	distanciaX = enemigo.x - tropa.x;
	if (distanciaX < 0)
		distanciaX = distanciaX * -1;

	distanciaY = enemigo.y - tropa.y;
	if (distanciaY < 0)
		distanciaY = distanciaY * -1;

	if (distanciaX<0 || distanciaY<0)
		cout<<"EXPLOTA"<<endl;


	return (distanciaX <= enemigo.alcance && distanciaY <= enemigo.alcance);

}

bool atacar(Enemigo* e, Tropa* t, Tropa* m, int cantEnemigos, int cantTropas, int* cantMuertos, int ciclos) {

	bool huboAtaque = false;

	for (int i = 0; i < cantEnemigos; i++) {
		for (int j = 0; j < cantTropas; j++) {
			if (estaEnRangoOVivo(e[i], t[j])) {
				if (e[i].ataque > t[j].escudo) {
					huboAtaque = true;
					printf("En el ciclo %d El enemigo %d le pego a la tropa %d \n",ciclos, i, j);
					printf("la tropa esta en %d %d y el enemigo en %d %d \n", t[j].x, t[j].y, e[i].x, e[i].y);
					t[j].vida = t[j].vida + t[j].escudo - e[i].ataque;

					if (t[j].vida <= 0) {
						strcpy(m[*cantMuertos].id,t[j].id);
						m[*cantMuertos].cantPasos = t[j].cantPasos;
						m[*cantMuertos].ciclos = ciclos;
						*cantMuertos = *cantMuertos + 1;
						t[j].vivo = false;
					}
				}
			}
		}
	}

	return huboAtaque;
}

bool checkearTablero(Tropa tropita, Enemigo* e, int cantEnemigos) {

	for (int i = 0; i < cantEnemigos; i++) {
		if (tropita.x + 1 == e[i].x && tropita.y == e[i].y) {
			return true;
		}
	}
	return false;
}

bool mover(Tropa* t, Enemigo* e, int cantEnemigos, int cantTropas) {

	bool huboMovimiento = false;

	for (int i = 0; i < cantTropas; i++) {
		if (t[i].x < 99) {
			bool hayAlgoAdelante = checkearTablero(t[i], e, cantEnemigos);
			if (!hayAlgoAdelante && t[i].vivo) {
				t[i].x++;
				t[i].cantPasos++;
				huboMovimiento = true;
			}
		}
	}
	return huboMovimiento;
}

int cmpMuertos(const void* a, const void* b){

	int aa = (*(Tropa*)a).ciclos;
	int bb = (*(Tropa*)b).ciclos;

	return (aa-bb);
}

int main() {

	int cantidadEnemigos;
	int cantidadTropas;
	int cantidadMuertos;
	int cantCiclos = 0;
	bool parar = false;
	bool perdioVida = false;
	bool seMovio = false;

	cantidadEnemigos = cargarEnemigos(enemigos);
//	cargarTablero(enemigos, tablero);

	for (int i = 0; i < cantidadEnemigos; i++) {
		printf("X: %i, Y: %i, Alcance: %i, Ataque: %f \n", enemigos[i].x, enemigos[i].y, enemigos[i].alcance,
				enemigos[i].ataque);
	}
	cantidadTropas = cargarTropas(tropas);

	for (int i = 0; i < cantidadTropas; i++) {
		printf("ID: %s, X: %i, Y: %i, Vida: %f, Escudo: %f \n", tropas[i].id, tropas[i].x, tropas[i].y, tropas[i].vida,
				tropas[i].escudo);
	}

	while (!parar) {
		cantCiclos++;

		perdioVida = atacar(enemigos, tropas, muertos, cantidadEnemigos, cantidadTropas, &cantidadMuertos, cantCiclos);
		seMovio = mover(tropas, enemigos, cantidadEnemigos, cantidadTropas);

		if(!seMovio && !perdioVida)
			parar = true;
	}

	qsort(muertos, cantidadMuertos, sizeof(Tropa), cmpMuertos);

	cout<<"Aca van los muertos"<<endl;

	for (int i = 0;i<cantidadMuertos;i++){

		printf("ID: %s, CantPasos: %i, CantCiclos: %i \n", muertos[i].id, muertos[i].cantPasos, muertos[i].ciclos);

	}

	cout<<"Aca van los muertos asdf"<<endl;

	return 0;
}

