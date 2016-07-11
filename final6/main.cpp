#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <stdio.h>

#define MAX_PAREDES 50
#define MAX_DISPAROS 50
#define MAX_RESULTADOS 50

using namespace std;

struct pared{
    string id;
    float x;
    float y_inf;
    float y_sup;
    int energia;
};

struct disparo{
    string id;
    float x_original;
    float y_original;
    int energia;
    bool vivo;
};

struct resultado{
    string id;
    float distancia;
};

void verificarColision(pared pared, disparo* disparo, resultado* resultado, int &contador){
    //Chequear posicion Y
    if((disparo->y_original >= pared.y_inf) && (disparo->y_original <= pared.y_sup)){
       if(disparo->x_original <= pared.x){
            disparo->energia = disparo->energia + pared.energia;
            if(disparo->energia <= 0){
                disparo->vivo = false;
                resultado->id = disparo->id;
                resultado->distancia = pared.x - disparo->x_original;
                contador++;
            }
        }

    }
}

int comparParedes(const void* a, const void* b){
    if((*(pared*)a).x < (*(pared*)b).x) return -1;
    if((*(pared*)a).x == (*(pared*)b).x) return 0;
    if((*(pared*)a).x > (*(pared*)b).x) return 1;
}

int comparDisparos(const void* a, const void* b){
    if((*(disparo*)a).x_original < (*(disparo*)b).x_original) return -1;
    if((*(disparo*)a).x_original == (*(disparo*)b).x_original) return 0;
    if((*(disparo*)a).x_original > (*(disparo*)b).x_original) return 1;
}

int comparResultado(const void* a, const void* b){
    if((*(resultado*)a).distancia < (*(resultado*)b).distancia) return -1;
    if((*(resultado*)a).distancia == (*(resultado*)b).distancia) return 0;
    if((*(resultado*)a).distancia > (*(resultado*)b).distancia) return 1;
}

int main(){

    pared* paredes = new pared[MAX_PAREDES];
    disparo* disparos = new disparo[MAX_DISPAROS];
    resultado* resultados = new resultado[MAX_RESULTADOS];

    int cantidadParedes = 0;
    int cantidadDisparos = 0;

    ifstream archivoParedes;
    ifstream archivoDisparos;

    archivoParedes.open("paredes.txt");
    archivoDisparos.open("balas.txt");

    if(archivoParedes){
        int i = 0;
        while(!archivoParedes.eof()){
            string idpared,x,yinf,ysup,energia;
            getline(archivoParedes,idpared,',');
            paredes[i].id = idpared;
            getline(archivoParedes,x,',');
            paredes[i].x = atof(x.c_str());
            getline(archivoParedes,yinf,',');
            paredes[i].y_inf = atof(yinf.c_str());
            getline(archivoParedes,ysup,',');
            paredes[i].y_sup = atof(ysup.c_str());
            getline(archivoParedes,energia,'\n');
            paredes[i].energia = atoi(energia.c_str());
            i++;
        }
        cantidadParedes = i;
    }

    if(archivoDisparos){
        int i = 0;
        while(!archivoDisparos.eof()){
            string iddisparo,xorig,yorig,energia;
            getline(archivoDisparos,iddisparo,',');
            disparos[i].id = iddisparo;
            getline(archivoDisparos,xorig,',');
            disparos[i].x_original = atof(xorig.c_str());
            getline(archivoDisparos,yorig,',');
            disparos[i].y_original = atof(yorig.c_str());
            getline(archivoDisparos,energia,'\n');
            disparos[i].energia = atoi(energia.c_str());
            disparos[i].vivo = true;
            i++;
        }
        cantidadDisparos = i;
    }

    qsort(paredes,cantidadParedes,sizeof(pared),comparParedes);
    qsort(disparos,cantidadDisparos,sizeof(disparo),comparDisparos);

/*
    for(int i = 0; i < cantidadParedes-1; i++){
        cout << paredes[i].id << endl;
        cout << paredes[i].x << endl;
        cout << paredes[i].y_inf << endl;
        cout << paredes[i].y_sup << endl;
        cout << paredes[i].energia << endl;
        cout << "------" << endl;
    }

    for(int i = 0; i < cantidadDisparos-1; i++){
        cout << disparos[i].id << endl;
        cout << disparos[i].x_original << endl;
        cout << disparos[i].y_original << endl;
        cout << disparos[i].energia << endl;
        cout << "------" << endl;
    }
*/
    int contador = 0;
    for (int i = 0; i < cantidadParedes-1; i++){
        for(int j = 0; j < cantidadDisparos-1; j++){
            if(disparos[j].vivo){
                verificarColision(paredes[i],&disparos[j],&resultados[contador],contador);
            }
        }
    }

    qsort(resultados, contador, sizeof(resultado), comparResultado);

    for(int i = 0; i < contador; i++){
        cout << resultados[i].id << endl;
        cout << resultados[i].distancia << endl;
        cout << "------" << endl;
    }


    return 0;
}
