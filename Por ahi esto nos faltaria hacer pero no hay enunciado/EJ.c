#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define TECHO 25.0f
#define PISO 0.0f
#define LIMITEY1 10.0f
#define LIMITEY2 15.0f
#define LIMITEX1 10.0f
#define LIMITEX2 15.0f

typedef struct Jugadores{
    char nombre[20];
    int IdPartido;
    int puntos;
}sJugadores;

int comparar(const void * val1, const void * val2){
    if((*(sJugadores *)val1).IdPartido < (*(sJugadores *)val2).IdPartido )
        return -1;
    else if((*(sJugadores *)val1).IdPartido > (*(sJugadores *)val2).IdPartido )
        return 1;
    else
        return 0;
}

bool estoyEnBlanco(float x, float y){
    if((x>LIMITEX1)&&(x<LIMITEX2)&&(y>LIMITEY1)&&(y<LIMITEY2))
        return true;
    return false;
}
bool estoyEnNegro(float x, float y){
    if((x>PISO)&&(x<LIMITEX1)&&(y>PISO)&&(y<LIMITEY1))
        return true;
    if((x<TECHO)&&(x>LIMITEX2)&&(y>PISO)&&(y<LIMITEY1))
        return true;
    if((x>PISO)&&(x<LIMITEX1)&&(y<TECHO)&&(y>LIMITEY2))
        return true;
    if((x<TECHO)&&(x>LIMITEX2)&&(y<TECHO)&&(y>LIMITEY2))
        return true;
    return false;
}

bool estoyEnGris(float x, float y){
    if((x>PISO)&&(x<LIMITEX1)&&(y<LIMITEY2)&&(y>LIMITEY1))
        return true;
    if((x<TECHO)&&(x>LIMITEX2)&&(y<LIMITEY2)&&(y>LIMITEY1))
        return true;
    if((x<LIMITEX2)&&(x>LIMITEX1)&&(y>PISO)&&(y<LIMITEY1))
        return true;
    if((x<LIMITEX2)&&(x>LIMITEX1)&&(y<TECHO)&&(y>LIMITEY2))
        return true;
    return false;
}
int buscar(sJugadores jugadores[100], int cantidadDeJugadores,char nombre[20],int IdPartido){
    if(cantidadDeJugadores==0)
        return -1;
    for(int i=0;i<cantidadDeJugadores;i++){
        if(((strcmp(jugadores[i].nombre,nombre))==0)&&(jugadores[i].IdPartido ==IdPartido))
            return i;
    }
    return -1;
}

int determinarPuntuacion(float x, float y){
    if(estoyEnBlanco(x,y))
        return 200;
    if(estoyEnNegro(x,y))
        return 25;
    if( estoyEnGris(x,y))
        return 50;
    return 0;
}

int determinarGanadores(sJugadores jugadores[100],int cantJug,sJugadores ganadores[100]){
    int IdLeidas[100];
    sJugadores ganadoresAux[100];
    int cantGanAux=0;
    int cantID=0;
    int cantGanadores=0;
    int id;
    for(int i=0;i<cantJug;i++){
        bool idLeida= false;
        id = jugadores[i].IdPartido;
        for(int j=0;j<cantID;j++){
            if(id == IdLeidas[j]){
                idLeida =true;
            }
        }
        if(!idLeida){
            IdLeidas[cantID]=id;
            cantID++;
            bool entre= false;
            for(int k=0;k<cantJug;k++){
                if(jugadores[k].IdPartido == id){
                    if(!entre){
                        ganadores[cantGanadores].puntos = jugadores[k].puntos;
                        ganadores[cantGanadores].IdPartido = id;
                        strcpy(ganadores[cantGanadores].nombre,jugadores[k].nombre);
                        entre=true;
                    }else if(jugadores[k].puntos>ganadores[cantGanadores].puntos){
                        ganadores[cantGanadores].puntos = jugadores[k].puntos;
                        ganadores[cantGanadores].IdPartido = id;
                        strcpy(ganadores[cantGanadores].nombre,jugadores[k].nombre);
                        cantGanAux=0;
                    }else if((jugadores[k].puntos==ganadores[cantGanadores].puntos)){
                        ganadoresAux[cantGanAux].puntos = jugadores[k].puntos;
                        ganadoresAux[cantGanAux].IdPartido = id;
                        strcpy(ganadoresAux[cantGanAux].nombre,jugadores[k].nombre);
                        cantGanAux++;
                    }
                }
            }
            if(cantGanAux>0){
                for(int i=0;i<cantGanAux;i++){
                    cantGanadores++;
                    ganadores[cantGanadores].puntos = ganadoresAux[i].puntos;
                    ganadores[cantGanadores].IdPartido = id;
                    strcpy(ganadores[cantGanadores].nombre,ganadoresAux[i].nombre);
                }
            }
            cantGanAux=0;
            cantGanadores++;
        }
    }
    return cantGanadores;
}

int main(){
    FILE * archivo;
    archivo= fopen("disparo.txt","r+");
    sJugadores jugadores[100];
    int cantidadDeJugadores=0;
    while(!feof(archivo)){
        char nombre[20];
        int IdPartido;
        float x;
        float y;
        int pos;
        fscanf(archivo,"%s",nombre);
        fscanf(archivo,"%d",&IdPartido);
        fscanf(archivo,"%f",&x);
        fscanf(archivo,"%f",&y);
        pos = buscar(jugadores,cantidadDeJugadores,nombre,IdPartido);
        if(pos==-1){
            jugadores[cantidadDeJugadores].IdPartido=IdPartido;
            strcpy(jugadores[cantidadDeJugadores].nombre,nombre);
            jugadores[cantidadDeJugadores].puntos=determinarPuntuacion(x,y);
            cantidadDeJugadores++;
        }else{
            jugadores[pos].puntos=jugadores[pos].puntos+determinarPuntuacion(x,y);
        }
    }
    sJugadores ganadores[100];
    int cantidadDeGanadores;
    cantidadDeGanadores=determinarGanadores(jugadores,cantidadDeJugadores,ganadores);
    qsort(ganadores,cantidadDeGanadores,sizeof(ganadores[0]),comparar);
    for(int i=0;i<cantidadDeGanadores;i++){
        printf(" %d %s %d \n",ganadores[i].IdPartido,ganadores[i].nombre,ganadores[i].puntos);
    }
    fclose(archivo);
    return 0;
}
