#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

#define CARACTERES			15
#define CANTIDAD_DE_TIROS	4
#define	PALO_IZQ			5
#define PALO_DER			9
#define PALO_SUP			3.5

typedef struct Tiro{
	char id[CARACTERES];
	float xi;
	float yi;
	float xf;
	float yf;
	bool esGol;
	float distanciaAlArco;
}tiro_t;

typedef struct Gol{
	char idTiro[CARACTERES];
	float distancia;
}gol_t;

void cargarValores(tiro_t* tiros[CANTIDAD_DE_TIROS]){
	FILE* pFile = fopen("tiros.txt","r");

	for(int i=0;i<CANTIDAD_DE_TIROS;i++){
		tiro_t* nuevoTiro = (tiro_t*) malloc (sizeof(tiro_t));

		fscanf(pFile,"%s",nuevoTiro->id);
		fscanf(pFile,"%f",&(nuevoTiro->xi));
		fscanf(pFile,"%f",&(nuevoTiro->yi));
		fscanf(pFile,"%f",&(nuevoTiro->xf));
		fscanf(pFile,"%f",&(nuevoTiro->yf));

		nuevoTiro->esGol = false;
		nuevoTiro->distanciaAlArco = 0;

		tiros[i] = nuevoTiro;
	}
	fclose(pFile);
}

void mostrarValores(tiro_t* tiros[CANTIDAD_DE_TIROS]){
	for(int i=0;i<CANTIDAD_DE_TIROS;i++){
		printf("%s %f %f %f %f\n", tiros[i]->id,tiros[i]->xi,tiros[i]->yi,tiros[i]->xf,tiros[i]->yf);
	}
}

void liberarMem(tiro_t* tiros[CANTIDAD_DE_TIROS]){
	for(int i=0;i<CANTIDAD_DE_TIROS;i++){
		free(tiros[i]);
	}
}

bool esGol(tiro_t* tiro){
	//Calculamos la ec de la recta del tiro
	float a = (tiro->yf - tiro->yi) / (tiro->xf - tiro->xi);
	float b = tiro->yf - a * tiro->xf;

	//Vemos si esta antes del palo izq o luego del palo derecho
	float yInterseccion;
	if(tiro->xi < PALO_IZQ)
		yInterseccion = a	*	PALO_IZQ +	b;
	else
		yInterseccion = a	*	PALO_DER +	b;

	if(yInterseccion < PALO_SUP && yInterseccion > 0){
		tiro->esGol = true;
		return true;
	}
	else
		return false;
}

void calcularDistanciaAlArco(tiro_t* tiro){
	//Calculamos la ec de la recta del tiro
	float a = (tiro->yf - tiro->yi) / (tiro->xf - tiro->xi);
	float b = tiro->yf - a * tiro->xf;
	float yInterseccion;
	if(tiro->xi < PALO_IZQ)
		yInterseccion = a	*	PALO_IZQ +	b;
	else
		yInterseccion = a	*	PALO_DER +	b;

	float distancia;
	if(tiro->xi < PALO_IZQ)
		distancia = sqrt(	pow(PALO_IZQ - tiro->xi,2) + pow(yInterseccion	-	tiro->yi,2)	);
	else
		distancia = sqrt(	pow(PALO_DER - tiro->xi,2) + pow(yInterseccion	-	tiro->yi,2)	);

	tiro->distanciaAlArco = distancia;
}

int procesarTiros(tiro_t* tiros[CANTIDAD_DE_TIROS]){
	int cantidadDeGoles = 0;
	for(int i=0;i<CANTIDAD_DE_TIROS;i++){
		if(esGol(tiros[i])){
			calcularDistanciaAlArco(tiros[i]);
			cantidadDeGoles ++;
		}
	}
	return cantidadDeGoles;
}

void cargarGoles(gol_t* goles, tiro_t* tiros[CANTIDAD_DE_TIROS]){
	int index = 0;
	for(int i=0;i<CANTIDAD_DE_TIROS;i++){
		if(tiros[i]->esGol){
			gol_t gol;
			strcpy(gol.idTiro,tiros[i]->id);
			gol.distancia = tiros[i]->distanciaAlArco;
			goles[index] =	gol;
			index++;
		}
	}
}

int compare(const void* a, const void* b){
	if(	(*(gol_t*)a).distancia >	(*(gol_t*)b).distancia	) return 1;
	if(	(*(gol_t*)a).distancia ==	(*(gol_t*)b).distancia	) return 0;
	if(	(*(gol_t*)a).distancia <	(*(gol_t*)b).distancia	) return -1;
}

void ordenarGoles(gol_t* goles, int cantidad){
	qsort(goles,cantidad,sizeof(gol_t),compare);
}

void generarSalida(gol_t* goles, int cantidad){
	FILE* pFile = fopen("resultado.txt","w");
	fprintf(pFile, "Id del tiro\tdistancia al arco\n");
	for(int i=0;i<cantidad;i++){
		fprintf(pFile,"%s %f\n", goles[i].idTiro, goles[i].distancia);
	}
	fclose(pFile);
}

int main(){
	tiro_t* tiros[CANTIDAD_DE_TIROS];
	int tirosQueSonGoles;

	cargarValores(tiros);
	//mostrarValores(tiros);

	tirosQueSonGoles = procesarTiros(tiros);

	gol_t goles[tirosQueSonGoles];

	cargarGoles(goles, tiros);

	ordenarGoles(goles, tirosQueSonGoles);

	generarSalida(goles, tirosQueSonGoles);

	liberarMem(tiros);
	return 0;
}
