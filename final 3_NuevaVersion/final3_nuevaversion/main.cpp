#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>

#define MAX_TANQUES 20
#define MAX_POZOS 20
#define MAX_RESULTADOS 20
using namespace std;
struct Tanque{
    string id;
    float x;
    float y;
    string direccion;
    float distanciaRecorrida;
    bool cayo;
};

struct Pozo{
    string id;
    float x;
    float y;
    float radio;
};

struct Respuesta{
    string idTanque;
    string idPozo;
    float distancia;
};


bool cae(Tanque tanque, Pozo pozo){
    if(tanque.direccion == "h"){
        cout << "Es horizontal" << endl;
        if((tanque.x <= pozo.x - pozo.radio) && ((tanque.y >= pozo.y - pozo.radio) && (tanque.y <= pozo.y + pozo.radio))){
            return true;
        }
        if((tanque.x >= pozo.x + pozo.radio) && ((tanque.y >= pozo.y - pozo.radio) && (tanque.y <= pozo.y + pozo.radio))){
            return true;
        }
    }else{
        cout << "es vertical" << endl;
        if((tanque.y <= pozo.y - pozo.radio) && ((tanque.x >= pozo.x - pozo.radio) && (tanque.x <= pozo.x + pozo.radio))){
            return true;
        }
        if((tanque.y >= pozo.y + pozo.radio) && ((tanque.x >= pozo.x - pozo.radio) && (tanque.x <= pozo.x + pozo.radio))){
            return true;
        }

    }

    return false;
}

int main(){
    ifstream archivoTanques, archivoPozos;
    archivoTanques.open("tanques.txt");
    string idTanque, xTanque, yTanque, direccionTanque;
    Tanque tanques[MAX_TANQUES];
    int i = 0, cantidadTanques = 0;
    if(archivoTanques){
        while(!archivoTanques.eof()){
            getline(archivoTanques, idTanque, ',');
            tanques[i].id = idTanque;
            getline(archivoTanques, xTanque, ',');
            tanques[i].x = atof(xTanque.c_str());
            getline(archivoTanques, yTanque, ',');
            tanques[i].y = atof(yTanque.c_str());
            getline(archivoTanques, direccionTanque, '\n');
            tanques[i].direccion = direccionTanque;
            tanques[i].distanciaRecorrida = 0;
            tanques[i].cayo = false;
            i++;
        }
        cantidadTanques = i;
    }

    for(int i = 0; i < cantidadTanques; i++){
        cout << "ID: " << tanques[i].id << "X: " << tanques[i].x << "Y: " << tanques[i].y << "Direccion" << tanques[i].direccion << endl;
    }

    archivoTanques.close();

    string idPozo, xPozo, yPozo, radioPozo;
    Pozo pozos[MAX_POZOS];
    archivoPozos.open("pozos.txt");
    int j = 0, cantidadPozos = 0;
    if(archivoPozos){
        while(!archivoPozos.eof()){
            getline(archivoPozos, idPozo, ',');
            pozos[j].id = idPozo;
            getline(archivoPozos, xPozo, ',');
            pozos[j].x = atof(xPozo.c_str());
            getline(archivoPozos, yPozo, ',');
            pozos[j].y = atof(yPozo.c_str());
            getline(archivoPozos, radioPozo, '\n');
            pozos[j].radio = atof(radioPozo.c_str());
            j++;
        }
        cantidadPozos = j;
    }

    archivoPozos.close();

    /*for(int i = 0; i < cantidadPozos; i++){
        cout << "ID: " << pozos[i].id << "X: " << pozos[i].x << "Y: " << pozos[i].y << "Radio" << pozos[i].radio << endl;
    }*/

    Respuesta* respuestas = new Respuesta[MAX_RESULTADOS];
    int contadorRtas = 0;
    for(int i = 0; i < cantidadTanques; i++){
        for(int j  = 0; j < cantidadPozos; j++){
            //cout << "Comparando: " << tanques[i].id << " " << pozos[j].id << endl;
            if(!tanques[i].cayo){
                if(cae(tanques[i], pozos[j])){
                    tanques[i].cayo = true;
                    cout << "Cayo: " << tanques[i].id << " en el pozo: " << pozos[j].id << endl;
                    respuestas[contadorRtas].idPozo = pozos[j].id;
                    respuestas[contadorRtas].idTanque = tanques[i].id;
                    if(tanques[i].direccion == "h"){
                        if(tanques[i].x <= pozos[j].x){
                            respuestas[contadorRtas].distancia = sqrt(((pozos[j].x - pozos[j].radio) - tanques[i].x)*((pozos[j].x - pozos[j].radio)- tanques[i].x));
                        }else{
                            respuestas[contadorRtas].distancia = sqrt(((pozos[j].x + pozos[j].radio) - tanques[i].x)*((pozos[j].x + pozos[j].radio)- tanques[i].x));
                        }

                    }else{
                        if(tanques[i].y <= pozos[j].y){
                            respuestas[contadorRtas].distancia = sqrt(((pozos[j].y - pozos[j].radio) - tanques[i].y)*((pozos[j].y - pozos[j].radio) - tanques[i].y));
                        }else{
                            respuestas[contadorRtas].distancia = sqrt(((pozos[j].y + pozos[j].radio) - tanques[i].y)*((pozos[j].y + pozos[j].radio) - tanques[i].y));
                        }

                    }
                    cout << "Distancia: " << respuestas[contadorRtas].distancia << endl;
                    contadorRtas++;
                }
            }
        }
    }




    return 0;
}
