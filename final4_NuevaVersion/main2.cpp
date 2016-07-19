#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "iostream"

#define CANTIDAD_DE_POZOS	4
#define CANTIDAD_DE_TANQUES	3
#define CARACTERES 15
using namespace std;
typedef struct Pozo{
	char id[CARACTERES];
	float x;
	float y;
	float radio;
}pozo_t;

typedef struct Tanque{
	char id[CARACTERES];
	float xIzq;
	float yInf;
	float xDer;
	float ySup;
	float distanciaAlPozo;
	char idPozo[CARACTERES];
}tanque_t;

void cargarValores(pozo_t* pozos[CANTIDAD_DE_POZOS],tanque_t* tanques[CANTIDAD_DE_TANQUES]){
	FILE* pFile1 = fopen("pozos.txt","r");
	FILE* pFile2 = fopen("tanques.txt","r");
	if(!pFile1 || !pFile2)
		printf("Error en la carga del archivo");

	for(int i=0;i<CANTIDAD_DE_POZOS;i++){
		pozo_t* nuevoPozo = (pozo_t*) malloc (sizeof(pozo_t));

		fscanf(pFile1,"%s",nuevoPozo->id);
		fscanf(pFile1,"%f",&(nuevoPozo->x));
		fscanf(pFile1,"%f",&(nuevoPozo->y));
		fscanf(pFile1,"%f",&(nuevoPozo->radio));

		pozos[i] = nuevoPozo;
	}

	for(int i=0;i<CANTIDAD_DE_TANQUES;i++){
		tanque_t* nuevoTanque = (tanque_t*) malloc (sizeof(tanque_t));

		fscanf(pFile2,"%s",nuevoTanque->id);
		fscanf(pFile2,"%f",&(nuevoTanque->xIzq));
		fscanf(pFile2,"%f",&(nuevoTanque->yInf));
		fscanf(pFile2,"%f",&(nuevoTanque->xDer));
		fscanf(pFile2,"%f",&(nuevoTanque->ySup));
		nuevoTanque->distanciaAlPozo = 0;
		strcpy(nuevoTanque->idPozo," ");

		tanques[i] = nuevoTanque;
	}

	fclose(pFile1);
	fclose(pFile2);
}

void mostrarValores(pozo_t* pozos[CANTIDAD_DE_POZOS],tanque_t* tanques[CANTIDAD_DE_TANQUES]){
	for(int i = 0; i<CANTIDAD_DE_POZOS; i++){
		printf("%s %f %f %f\n",pozos[i]->id,pozos[i]->x,pozos[i]->y,pozos[i]->radio);
	}

	for(int i = 0; i<CANTIDAD_DE_TANQUES; i++){
		printf("%s %f %f %f %f\n",tanques[i]->id,tanques[i]->xIzq,tanques[i]->yInf,tanques[i]->xDer,tanques[i]->ySup);
	}
}

void liberarMem(pozo_t* pozos[CANTIDAD_DE_POZOS],tanque_t* tanques[CANTIDAD_DE_TANQUES]){
	for(int i = 0; i<CANTIDAD_DE_POZOS; i++){
		free(pozos[i]);
	}

	for(int i = 0; i<CANTIDAD_DE_TANQUES; i++){
		free(tanques[i]);
	}
}

int compare1(const void* a, const void* b){
	if( (*(pozo_t**)a)->x < (*(pozo_t**)b)->x )	return -1;
	if( (*(pozo_t**)a)->x == (*(pozo_t**)b)->x )	return 0;
	if( (*(pozo_t**)a)->x > (*(pozo_t**)b)->x )	return 1;
}

bool entraEnPozo(tanque_t* tanque, pozo_t* pozo){
	//SI el tanque ya entro en un pozo, salimos
	if(strcmp(tanque->idPozo," ") != 0)
		return false;

	float anchoTanque = tanque->xDer - tanque->xIzq;
	float limiteIzq = pozo->x - anchoTanque/2;
	float limiteDer = pozo->x + anchoTanque/2;

	//Ec del circulo
	float xIzqInfPozo = - sqrt( pow(pozo->radio,2)  -  pow(tanque->yInf - pozo->y,2)) + pozo->x;
	float xDerInfPozo = sqrt( pow(pozo->radio,2)  -  pow(tanque->yInf - pozo->y,2)) + pozo->x;
	float xIzqSupPozo = - sqrt( pow(pozo->radio,2)  -  pow(tanque->ySup - pozo->y,2)) + pozo->x;
	float xDerSupPozo = sqrt( pow(pozo->radio,2)  -  pow(tanque->ySup - pozo->y,2)) + pozo->x;

	if(limiteIzq >= xIzqInfPozo	&&	limiteIzq >= xIzqSupPozo	&&	limiteDer <= xDerInfPozo	&&	limiteDer <= xDerSupPozo){
		strcpy(tanque->idPozo,pozo->id);
		return true;
	}
	return false;
}

//Calcula la distancia entre el centro de masa del tanque inicial, con el centro de masa del tanque cuando ingresa al pozo
void calcularDistancia(tanque_t* tanque, pozo_t* pozo){
	float anchoTanque = tanque->xDer - tanque->xIzq;
	float xIzqInfPozo = - sqrt( pow(pozo->radio,2)  -  pow(tanque->yInf - pozo->y,2)) + pozo->x;
	float xIzqSupPozo = - sqrt( pow(pozo->radio,2)  -  pow(tanque->ySup - pozo->y,2)) + pozo->x;

	float xDondeCaeElTanque;
	if(xIzqInfPozo >= xIzqSupPozo)
		xDondeCaeElTanque = xIzqInfPozo;
	else
		xDondeCaeElTanque = xIzqSupPozo;

	float centroDeMasaInicial = anchoTanque/2 + tanque->xIzq;
	float centroDeMasaFinal = anchoTanque/2 + xDondeCaeElTanque;

	tanque->distanciaAlPozo =	centroDeMasaFinal - centroDeMasaInicial;
}

//Suponemos que no hay superposicion de pozos
void procesarDatos(pozo_t* pozos[CANTIDAD_DE_POZOS],tanque_t* tanques[CANTIDAD_DE_TANQUES]){

	//Ordeno los pozos ascendentemende de acuerdo a su coord x
	qsort(pozos,CANTIDAD_DE_POZOS,sizeof(pozo_t*),compare1);

	for(int i=0; i<CANTIDAD_DE_TANQUES; i++){
		for(int j=0; j<CANTIDAD_DE_POZOS; j++){

			if(entraEnPozo(tanques[i],pozos[j]))
				calcularDistancia(tanques[i],pozos[j]);
		}
	}
}

int compare2(const void* a, const void* b){
	if( (*(tanque_t**)a)->distanciaAlPozo < (*(tanque_t**)b)->distanciaAlPozo) return -1;
	if( (*(tanque_t**)a)->distanciaAlPozo == (*(tanque_t**)b)->distanciaAlPozo) return 0;
	if( (*(tanque_t**)a)->distanciaAlPozo > (*(tanque_t**)b)->distanciaAlPozo) return 1;
}

void ordenarSalida(tanque_t* tanques[CANTIDAD_DE_TANQUES]){
	qsort(tanques,CANTIDAD_DE_TANQUES,sizeof(tanque_t*),compare2);
}

void generarSalida(tanque_t* tanques[CANTIDAD_DE_TANQUES]){
	ordenarSalida(tanques);

    for(int i = 0; i < CANTIDAD_DE_TANQUES-1; i++){
         cout << "ID TANQUE: " << tanques[i]->id << "ID POZO: " << tanques[i]->idPozo << "DISTANCIA: " << tanques[i]->distanciaAlPozo << endl;
    }
	FILE* pFile = fopen("resultados.txt","w");

	fprintf(pFile,"Se presenta el resultado en el siguiente formato:\nIdTanque\tIdPozo\tDistanciaRecorrida\n");
	for(int i=0;i<CANTIDAD_DE_TANQUES;i++){
		//printf("%s\n",tanques[i]->id);
		if(tanques[i]->distanciaAlPozo != 0)
			fprintf(pFile,"%s \t %s \t %f\n",tanques[i]->id,tanques[i]->idPozo,tanques[i]->distanciaAlPozo);
	}

	fclose(pFile);
}

int main(){
	pozo_t* pozos[CANTIDAD_DE_POZOS];
	tanque_t* tanques[CANTIDAD_DE_TANQUES];

	cargarValores(pozos,tanques);
//	mostrarValores(pozos,tanques);

	procesarDatos(pozos,tanques);

	generarSalida(tanques);

	liberarMem(pozos,tanques);
	return 0;
}
