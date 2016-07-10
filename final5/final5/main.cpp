#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>

#define MAX_BALAS 4
#define MAX_CAJAS 4
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
};

struct resultado{
    string id_personaje;
    float distancia;
};

void calcularDistanciaRecorrida(caja *caja, resultado* &resultados, int &contador){
    resultados[contador].distancia = caja->posX_final - caja->posX;
    resultados[contador].id_personaje = caja->id_personaje;
    contador++;
}

void actualizarPosiciones(bala* bala, caja* caja){
    bala->posY = bala->posY - VEL_BALA * TIMESTEP;
    if(bala->posY <= 0){
        bala->existe = false;
    }

    caja->posX_final = caja->posX_final + VEL_CAJA * TIMESTEP;

}

void choca(bala* bala, caja *caja, resultado* &resultados, int &contador){
    bool choca = false;
        for (int j = 0; j < MAX_CAJAS-1 ; j++){
            actualizarPosiciones(bala,caja);
            cout << "posicion final de caja " << caja->id_personaje << ": " <<caja->posX_final << endl;
            cout << "estamos en la bala: " << bala->posX << " "<< bala->posY << endl;
            if((bala->existe)&&(bala->posY >= 2)){
                cout << "posicion final de caja " << caja->id_personaje << ": " <<caja->posX_final << endl;
                if((bala->posX <= caja->posX_final && bala->posX+1 >= caja->posX_final) || (bala->posX >= caja->posX_final && bala->posX+1 <= caja->posX_final+2) || (bala->posX >= caja->posX_final && bala->posX+1 >= caja->posX_final+2)){
                    cout << "Entro al if" << endl;
                    choca = true;
                    if(choca){
                        calcularDistanciaRecorrida(caja, resultados, contador);
                        bala->existe = false;
                    }
                }
            }
        }
}

int main(){
    bala balas[MAX_BALAS];
    caja cajas[MAX_CAJAS];
    resultado* resultados = new resultado[MAX_RESULTADOS];

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
            j++;
        }
    }

    //Mostrar arrays cargados del archivo de texto
    for(int i = 0; i < MAX_BALAS-1; i++){
        cout << "posX: " << balas[i].posX << " posY: " << balas[i].posY << endl;
    }

    for(int i = 0; i < MAX_CAJAS-1; i++){
        cout << "Id Personaje: " << cajas[i].id_personaje << " posX: " << cajas[i].posX << endl;
    }

    int contador = 0;
    for(int i = 0; i < MAX_BALAS-1; i++){
        for(int j = 0; j < MAX_CAJAS-1; j++){
            if(balas[i].existe){
                cout << "contador: " << contador << endl;
                choca(&balas[i],&cajas[j],resultados, contador);
            }
        }
    }

    //for(int i = 0; i < MAX_RESULTADOS-1; i++){
    //    cout << "Id Personaje: " << resultados[i].id_personaje << " Distancia recorrida: " << resultados[i].distancia << endl;
    //}

    return 0;
}
