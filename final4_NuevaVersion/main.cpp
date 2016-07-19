#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>

#define MAX_TANQUES 20
#define MAX_POZOS 20
#define MAX_RESPUESTAS 20

using namespace std;
struct Tanque{
    string id;
    float xInfIzq;
    float yInfIzq;
    float xSupDer;
    float ySupDer;
    float xInfIzqAcum;
    float xSupDerAcum;
    bool cayo;
    bool procesado;
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


int compar(const void* a, const void* b){
    if((*(Respuesta*)a).distancia > (*(Respuesta*)b).distancia) return 1;
    if((*(Respuesta*)a).distancia == (*(Respuesta*)b).distancia) return 0;
    if((*(Respuesta*)a).distancia < (*(Respuesta*)b).distancia) return -1;
}

int comparPozos(const void* a, const void* b){
    if((*(Pozo*)a).x > (*(Pozo*)b).x) return 1;
    if((*(Pozo*)a).x == (*(Pozo*)b).x) return 0;
    if((*(Pozo*)a).x < (*(Pozo*)b).x) return -1;
}

int comparTanques(const void* a, const void* b){
    if((*(Tanque*)a).xInfIzq > (*(Tanque*)b).xInfIzq) return 1;
    if((*(Tanque*)a).xInfIzq == (*(Tanque*)b).xInfIzq) return 0;
    if((*(Tanque*)a).xInfIzq < (*(Tanque*)b).xInfIzq) return -1;
}

bool verificarEcuacionCirculo(float xTanque, float yTanque, Pozo pozo){
    float difX = xTanque - pozo.x;
    float difY = yTanque - pozo.y;
    if(((difX*difX) + (difY*difY)) < (pozo.radio * pozo.radio)){
        return true;
    }
    return false;
}

bool cae(Tanque tanque, Pozo pozo){
    float x1 = tanque.xInfIzqAcum;
    float x2 = tanque.xSupDerAcum;
    float y1 = tanque.yInfIzq;
    float y2 = tanque.ySupDer;

    if(!verificarEcuacionCirculo(x1,y1,pozo)) return false;
    if(!verificarEcuacionCirculo(x1,y2,pozo)) return false;
    if(!verificarEcuacionCirculo(x2,y1,pozo)) return false;
    if(!verificarEcuacionCirculo(x2,y2,pozo)) return false;

    return true;
}

int main(){
    ifstream archivoTanques, archivoPozos;
    archivoTanques.open("tanques.txt");
    string idTanque, xInfIzq, yInfIzq, xSupDer, ySupDer;
    Tanque tanques[MAX_TANQUES];
    int i = 0, cantidadTanques = 0;
    if(archivoTanques){
        while(!archivoTanques.eof()){
            getline(archivoTanques, idTanque, ',');
            tanques[i].id = idTanque;
            getline(archivoTanques, xInfIzq, ',');
            tanques[i].xInfIzq = atof(xInfIzq.c_str());
            getline(archivoTanques, yInfIzq, ',');
            tanques[i].yInfIzq = atof(yInfIzq.c_str());
            getline(archivoTanques, xSupDer, ',');
            tanques[i].xSupDer = atof(xSupDer.c_str());
            getline(archivoTanques, ySupDer, '\n');
            tanques[i].ySupDer = atof(ySupDer.c_str());
            tanques[i].xInfIzqAcum = tanques[i].xInfIzq;
            tanques[i].xSupDerAcum = tanques[i].xSupDer;
            tanques[i].cayo = false;
            tanques[i].procesado = false;
            i++;
        }
        cantidadTanques = i;
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

    //Se ordenan
    qsort(pozos, cantidadPozos-1, sizeof(Pozo), comparPozos);
    qsort(tanques, cantidadTanques-1, sizeof(Tanque), comparTanques);


    Respuesta* respuestas = new Respuesta[MAX_RESPUESTAS];
    int contadorProcesados = 0;
    int contadorRtas = 0;


    for(int j = 0; j < cantidadTanques-1; j++){
        for(int i = 0; i < cantidadPozos-1; i++){
            bool termino = false;
            bool choco = false;
            //cout << "Comparando: " << tanques[j].id << "con pozo: " << pozos[i].id << endl;
            if (tanques[j].xSupDer < (pozos[i].x - pozos[i].radio)){
                //Lo pongo al tanque en el comienzo del circulo
                tanques[j].xInfIzqAcum = tanques[j].xInfIzq + ((pozos[i].x - pozos[i].radio) - tanques[j].xSupDer);
                tanques[j].xSupDerAcum = pozos[i].x - pozos[i].radio;
            }
            while((!termino)&&(!tanques[j].cayo)){

                    bool caeRta = cae(tanques[j], pozos[i]);
                    if(caeRta){
                        //cout << "cae el tanque: " << tanques[j].id << " en pozo: " << pozos[i].id << endl;
                        tanques[j].cayo = true;
                        respuestas[contadorRtas].idTanque = tanques[j].id;
                        respuestas[contadorRtas].idPozo = pozos[i].id;
                        respuestas[contadorRtas].distancia = tanques[j].xSupDerAcum - tanques[j].xSupDer;
                        //cout << "Distancia: " << respuestas[contadorRtas].distancia << endl;
                        contadorRtas++;
                        choco = true;
                        termino = true;
                    }else{
                            tanques[j].xInfIzqAcum = tanques[j].xInfIzqAcum + 0.0001;
                            tanques[j].xSupDerAcum = tanques[j].xSupDerAcum + 0.0001;
                    }
                    if((tanques[j].xInfIzqAcum > pozos[i].x + pozos[i].radio)){
                        termino = true;
                    }
            }
        }
    }

    qsort(respuestas, contadorRtas, sizeof(Respuesta), compar);

    for(int i = 0; i < contadorRtas; i++){
        cout << respuestas[i].idPozo << " " << respuestas[i].idTanque << " " << respuestas[i].distancia << endl;
    }

    return 0;
}
