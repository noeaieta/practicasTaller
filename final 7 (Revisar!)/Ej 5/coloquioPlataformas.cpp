// coloquioPlataformas.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#define MAX_CHAR 20
#define MAX_PLAT 8
#define	MAX_PERS 10
#define	MAX_NIVEL 10

typedef struct plataforma_t{
	float xo;
	float xf;
}plataforma;

typedef struct nivel_t{
	float y;
	int cantPlat;
	plataforma plataformas[MAX_PLAT];
}nivel;

typedef struct personaje_t{
	char id[MAX_CHAR];
	float x;
	float y;
}personaje;

typedef struct resultado_t{
	char id[MAX_CHAR];
	float distancia;
}resultado;



void obtenerNiveles(nivel niveles[MAX_NIVEL],int* cant){
	FILE* file = fopen("niveles.txt","r");
	float xo,xd,y;
	int i = 0;
	int cantPar = 0;

	if(!file)
		printf("no se pudo abrir el archivo de niveles");

	while(!feof(file)){
		//El primer parametro del archivo de texto se lo agregue yo ya que no se fijan en la entrada
		//se puede cambiar a gusto yo le agregue la cantidad de plataformas ya que varian para cada nivel
		//para poder usar fscanf y no tener que trabajar con fstream get line strtok y atof
		fscanf(file,"%i",&cantPar);

		niveles[i].cantPlat = cantPar;
		fscanf(file,"%f",&y);
		niveles[i].y = y;
		printf("%f ",y);
		for(int j = 0; j < cantPar;j++){
			fscanf(file,"%f %f",&xo,&xd);
			niveles[i].plataformas[j].xo = xo;
			niveles[i].plataformas[j].xf = xd;
			printf("%f %f",xo,xd);
		}
		i++;
		printf("\n");
	}


	(*cant) = i;

}

void obtenerPersonajes(personaje personajes[MAX_PERS],int* cantPers){
	FILE* file = fopen("personajes.txt","r");
	if(!file)
		printf("no se pudo abrir el archivo de niveles");
	char id[MAX_CHAR];
	float x,y;
	int i = 0;
	while(!feof(file)){
		fscanf(file,"%s %f %f",id,&x,&y);
		strcpy(personajes[i].id,id);
		personajes[i].x = x;
		personajes[i].y = y;
		printf("%s %f %f\n",id,x,y);
		i++;

	}
	(*cantPers) = i;
	fclose(file);


}


float minimo(float a,float b){
	if(a < b)
		return a;
	return b;

}




bool caeSobrePlataformaDelNivel(personaje pers, nivel niv, plataforma * plat){
	//si cae justo en el borde de la plataforma lo tomo como que no cae sobre ella.
	//en el coloquio dijeron que no iba a pasar eso.
	//tampoco se da el caso en el que el personaje aparezca con xo e yo en alguna plataforma
	//por lo cual niv.y < pers.y.
	for(int i = 0; i < niv.cantPlat; i ++){
		if(niv.y < pers.y && pers.x < niv.plataformas[i].xf && pers.x > niv.plataformas[i].xo){
			(*plat) = niv.plataformas[i];
			return true;
		}
	}

	return false;
}




//La obtener a traves de fuerza bruta y recursividad.
float obtenerDistanciaMinima(personaje pers,nivel niveles[MAX_NIVEL],int cantNiv,float distAnterior){
	plataforma plataformaAux;
	int idNivel = -1;
	for(int i = 0; i < cantNiv;i++){
		//Para cada uno de los niveles veo desde el inferior hasta el superior si el personaje cae en alguna
		if( caeSobrePlataformaDelNivel(pers,niveles[i],&plataformaAux)){
			idNivel = i;
		}
	}

	//Si no cae sobre ninguna significa que cae al piso ( nivel inferior ) por lo cual termina y devuelvo lo que cae
	//mas la distancia anterior
	if(idNivel == -1)
		return distAnterior + pers.y;
	
	//sino sumo la distancia hasta la plataforma cambio el y del personaje.
	distAnterior += pers.y - niveles[idNivel].y;
	pers.y = niveles[idNivel].y;

	//creo un personaje para el camino izquierdo
	personaje i;
	i.x = plataformaAux.xo;
	i.y = pers.y;
	float distI = distAnterior + abs(i.x-pers.x);

	//creo un personaje para el camino derecho
	personaje d;
	d.x = plataformaAux.xf;
	d.y = pers.y;
	float distD = distAnterior + abs(d.x-pers.x);

	//devuelvo el camino minimo entre el camino derecho y el izquierdo que a su vez si se dividen daran el menor como resultado.
	return minimo(obtenerDistanciaMinima(i,niveles,cantNiv,distI),obtenerDistanciaMinima(d,niveles,cantNiv,distD));
}



int compararNiveles(const void* a, const void* b){
	if((*(nivel *)a).y<(*(nivel *)b).y)
		return -1;
	if((*(nivel *)a).y==(*(nivel *)b).y)
		return 0;
	if ((*(nivel *)a).y>(*(nivel *)b).y)
		return 1;

}


int compararResultados(const void* a, const void* b){
	if((*(resultado *)a).distancia<(*(resultado *)b).distancia)
		return -1;
	if((*(resultado *)a).distancia==(*(resultado *)b).distancia)
		return 0;
	if ((*(resultado *)a).distancia>(*(resultado *)b).distancia)
		return 1;


}





int _tmain(int argc, _TCHAR* argv[])
{
	
	nivel niveles[MAX_NIVEL];
	int cantNiveles;
	obtenerNiveles(niveles,&cantNiveles);

	//ordeno los niveles de menor a mayor Y_nivel
	qsort(niveles,cantNiveles,sizeof(nivel),compararNiveles);

	personaje personajes[MAX_PERS];
	int cantPers;
	obtenerPersonajes(personajes,&cantPers);

	resultado resultados[MAX_PERS];

	for(int i = 0; i< cantPers;i++){
		
		strcpy(resultados[i].id,personajes[i].id);
		resultados[i].distancia = obtenerDistanciaMinima(personajes[i],niveles,cantNiveles,0);

	}
	//ordeno los resultados de menor a mayor distancia.
	qsort(resultados,cantPers,sizeof(resultado),compararResultados);

	printf("RESULTADOS:\n");
	for(int i = 0; i < cantPers;i++){
		printf("%s %f\n",resultados[i].id,resultados[i].distancia);
	}

	return 0;
}

