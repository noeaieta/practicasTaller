#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>

#define MAX_BALAS 25
#define MAX_CAJAS 10
#define MAX_RESULTADOS 2
#define VEL_BALA 10
#define VEL_CAJA 5
#define TIMESTEP 0.01

using namespace std;

struct bala {
    float posX;
    float posY;
    bool existe;
};

struct caja {
    string id_personaje;
    float posX;
    float posX_final;
    bool existe;
};

struct resultado{
    string id_personaje;
    float distancia;
};

int comparBalas(const void*a,const void*b){
    if((*(bala*)a).posX > (*(bala*)b).posX) return 1;
    if((*(bala*)a).posX == (*(bala*)b).posX) return 0;
    if((*(bala*)a).posX < (*(bala*)b).posX) return -1;
}

int comparCajas(const void* a, const void*b){
    if((*(caja*)a).posX > (*(caja*)b).posX) return 1;
    if((*(caja*)a).posX == (*(caja*)b).posX) return 0;
    if((*(caja*)a).posX < (*(caja*)b).posX) return -1;
}

void verificarSiColisionan(caja* caja, bala* bala){

    //cout << bala->posX << endl;
    float tiempoCaidaBala = (bala->posY)/VEL_BALA;
    //cout << "tiempo caida bala: " << tiempoCaidaBala << endl;
    float tiempoColisionMaximo = (bala->posX - caja->posX)/VEL_CAJA;
    //cout << "tiempo colision maximo: " << tiempoColisionMaximo << endl;
    float tiempoColisionMinimo = (bala->posX+1 - caja->posX+2)/VEL_CAJA;
    //cout << "tiempo colision minimo: " << tiempoColisionMinimo << endl;

    if ((tiempoCaidaBala <= tiempoColisionMinimo) && (tiempoCaidaBala >= tiempoColisionMaximo)){
        caja->existe = false;
        bala->existe = false;
        caja->posX_final = tiempoCaidaBala*VEL_CAJA - caja->posX;
        cout << caja->id_personaje << endl;
        cout << "posX_final: " << caja->posX_final << endl;
    }

}

int main(){
    bala* balas = new bala[MAX_BALAS];
    caja* cajas = new caja[MAX_CAJAS];
    resultado* resultados = new resultado[MAX_RESULTADOS];

    int cantidadBalas = 0;
    int cantidadCajas = 0;

    ifstream archivoBalas;
    ifstream archivoCajas;

    archivoBalas.open("balas.txt");
    archivoCajas.open("cajas.txt");

    if(archivoBalas){
        string posX,posY;
        int i = 0;
        while(!archivoBalas.eof()){
            getline(archivoBalas,posX,',');
            balas[i].posX = atof(posX.c_str());
            getline(archivoBalas,posY,'\n');
            balas[i].posY = atof(posY.c_str());
            balas[i].existe = true;
            i++;
        }
        cantidadBalas = i;
    }

    if(archivoCajas){
        int j = 0;
        string idPersonaje,posX;
        while(!archivoCajas.eof()){
            getline(archivoCajas,idPersonaje,',');
            cajas[j].id_personaje = idPersonaje;
            getline(archivoCajas,posX,'\n');
            cajas[j].posX = atof(posX.c_str());
            cajas[j].posX_final = cajas[j].posX;
            cajas[j].existe = true;
            j++;
        }
        cantidadCajas = j;
    }

    //Ordenar los arrays de balas y cajas
    qsort(balas,cantidadBalas,sizeof(bala),comparBalas);
    qsort(cajas,cantidadCajas,sizeof(caja),comparCajas);

    //Recorrer array de cajas
    for(int i = 0; i < cantidadCajas-1; i++){
        if(cajas[i].existe){
            for(int j = 0; j < cantidadBalas-1; j++){
                if((balas[j].existe)&&(cajas[i].existe)){
                    verificarSiColisionan(&cajas[i],&balas[j]);
                }
            }
        }
    }

    //Mostrar arrays cargados del archivo de texto
    for(int i = 0; i < cantidadBalas-1; i++){
        cout << "posX: " << balas[i].posX << " posY: " << balas[i].posY << endl;
    }

    for(int i = 0; i < cantidadCajas-1; i++){
        cout << "Id Personaje: " << cajas[i].id_personaje << " posX: " << cajas[i].posX << endl;
    }

    return 0;
}
