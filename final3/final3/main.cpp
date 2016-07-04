#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>

#define MAX_TANQUES 5
#define MAX_POZOS 3
using namespace std;
struct Tanque{
    string id;
    float x;
    float y;
    string direccion;
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

bool estaDentroDelDiametroDelPozo(Pozo pozo, Tanque tanque){
    if(tanque.direccion == "h"){
        if((tanque.y > pozo.y - pozo.radio) && (tanque.y < pozo.y + pozo.radio)){
            return true;
        }
    }else{
        if((tanque.x > pozo.x - pozo.radio) && (tanque.x < pozo.x + pozo.radio)){
            return true;
        }
    }

    return false;
}

float distancia(float destino, float origen){
    float diferencia = destino - origen;
    return sqrt(diferencia*diferencia);
}

float calcularDistancia(Pozo pozo, Tanque tanque){
    if(tanque.direccion == "h"){
        if (tanque.x <= pozo.x){
            return distancia(pozo.x-pozo.radio,tanque.x);
        } else {
            return distancia(pozo.x+pozo.radio,tanque.x);
        }
    }else{
        if (tanque.y <= pozo.y){
            return distancia(pozo.y-pozo.radio,tanque.y);
        } else{
            return distancia(pozo.y-pozo.radio,tanque.y);
        }
    }
}

bool verificarCaidas(Pozo pozo, Tanque tanque, Respuesta* &arrayResultados, int contador){
    bool estaDentro = estaDentroDelDiametroDelPozo(pozo, tanque);
    if(estaDentro){
        arrayResultados[contador].idPozo = pozo.id;
        arrayResultados[contador].idTanque = tanque.id;
        arrayResultados[contador].distancia = calcularDistancia(pozo, tanque);
        return true;
    }
    return false;
}

int compar(const void* a, const void* b){
    if((*(Respuesta*)a).distancia > (*(Respuesta*)b).distancia) return 1;
    if((*(Respuesta*)a).distancia == (*(Respuesta*)b).distancia) return 0;
    if((*(Respuesta*)a).distancia < (*(Respuesta*)b).distancia) return -1;
}

int main(){
    ifstream archivoTanques, archivoPozos;
    archivoTanques.open("tanques.txt");
    string idTanque, xTanque, yTanque, direccionTanque;
    Tanque arrayTanques[MAX_TANQUES];
    int i = 0;
    if(archivoTanques){
        while(!archivoTanques.eof()){
            getline(archivoTanques, idTanque, ',');
            arrayTanques[i].id = idTanque;
            getline(archivoTanques, xTanque, ',');
            arrayTanques[i].x = atof(xTanque.c_str());
            getline(archivoTanques, yTanque, ',');
            arrayTanques[i].y = atof(yTanque.c_str());
            getline(archivoTanques, direccionTanque, '\n');
            arrayTanques[i].direccion = direccionTanque;
            i++;
        }
    }

    for(int i = 0; i < MAX_TANQUES -1; i++){
        cout << "ID: " << arrayTanques[i].id << "X: " << arrayTanques[i].x << "Y: " << arrayTanques[i].y << "Direccion" << arrayTanques[i].direccion << endl;
    }

    archivoTanques.close();

    string idPozo, xPozo, yPozo, radioPozo;
    Pozo arrayPozos[MAX_POZOS];
    archivoPozos.open("pozos.txt");
    int j = 0;
    if(archivoPozos){
        while(!archivoPozos.eof()){
            getline(archivoPozos, idPozo, ',');
            arrayPozos[j].id = idPozo;
            getline(archivoPozos, xPozo, ',');
            arrayPozos[j].x = atof(xPozo.c_str());
            getline(archivoPozos, yPozo, ',');
            arrayPozos[j].y = atof(yPozo.c_str());
            getline(archivoPozos, radioPozo, '\n');
            arrayPozos[j].radio = atof(radioPozo.c_str());
            j++;
        }
    }

    archivoPozos.close();


    Respuesta* arrayResultados = new Respuesta[3];
    int contador = 0;
    for (int k = 0; k < MAX_POZOS-1; k++){
        for(int i = 0; i < MAX_TANQUES-1; i++){
            if (verificarCaidas(arrayPozos[k],arrayTanques[i], arrayResultados, contador)){
                contador++;
            }
        }
    }

    qsort(arrayResultados,3,sizeof(Respuesta),compar);

    ofstream archivoSalida;
    archivoSalida.open("resultadosEjer3.txt");

    for(int i = 0; i < 3; i++){
        cout << arrayResultados[i].idPozo << endl;
        archivoSalida << arrayResultados[i].idPozo << endl;
        cout << arrayResultados[i].idTanque << endl;
        archivoSalida << arrayResultados[i].idTanque << endl;
        cout << arrayResultados[i].distancia << endl;
        archivoSalida << arrayResultados[i].distancia << endl;
        cout << "-----------------------------" << endl;
        archivoSalida << "-----------------------------" << endl;
    }

    archivoSalida.close();
    delete[] arrayResultados;

    return 0;
}
