#include <string>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#define MAX_BALAS 20
#define MAX_LADRILLOS 20
#define MAX_RESULTADOS 20

using namespace std;

struct bala{
    string id;
    float x;
    float y;
    int energia;
    float y_final;
    bool vive;
};

struct ladrillo{
    string id;
    float x_izq;
    float x_der;
    float y;
    int resistencia;
    bool vive;
};

struct resultado{
    string id_ladrillo;
    string id_bala;
    float distancia;
};

int comparLadrillos(const void* a, const void* b){
    if((*(ladrillo*)a).y > (*(ladrillo*)b).y) return 1;
    if((*(ladrillo*)a).y == (*(ladrillo*)b).y) return 0;
    if((*(ladrillo*)a).y < (*(ladrillo*)b).y) return -1;
}

int comparBalas(const void* a, const void* b){
    if((*(bala*)a).y > (*(bala*)b).y) return 1;
    if((*(bala*)a).y == (*(bala*)b).y) return 0;
    if((*(bala*)a).y < (*(bala*)b).y) return -1;
}

int comparResultados(const void* a, const void* b){
    if((*(resultado*)a).distancia > (*(resultado*)b).distancia) return 1;
    if((*(resultado*)a).distancia == (*(resultado*)b).distancia) return 0;
    if((*(resultado*)a).distancia < (*(resultado*)b).distancia) return -1;
}

bool choca(bala bala,ladrillo ladrillo){
    if((bala.y < ladrillo.y) && ((bala.x > ladrillo.x_izq)&&(bala.x < ladrillo.x_der))){
        return true;
    }
    return false;
}

float devolverDistancia(bala bala, ladrillo ladrillo){
    float diferencia = ladrillo.y - bala.y;
    return sqrt(diferencia*diferencia);
}

int main(){
    ifstream archivoBalas, archivoLadrillos;
    bala* balas = new bala[MAX_BALAS];
    ladrillo* ladrillos = new ladrillo[MAX_LADRILLOS];
    resultado* resultados = new resultado[MAX_RESULTADOS];

    int cantidadBalas = 0, cantidadLadrillos = 0;

    archivoBalas.open("balas.txt");
    archivoLadrillos.open("plataformas.txt");

    if(archivoBalas){
        int i = 0;
        while (!archivoBalas.eof()){
            string idbala,x,y,energia;
            getline(archivoBalas,idbala,',');
            balas[i].id = idbala;
            getline(archivoBalas,x,',');
            balas[i].x = atof(x.c_str());
            getline(archivoBalas,y,',');
            balas[i].y = atof(y.c_str());
            getline(archivoBalas,energia,'\n');
            balas[i].energia = atoi(energia.c_str());
            balas[i].y_final = balas[i].y;
            balas[i].vive = true;
            i++;
        }
        cantidadBalas = i;
    }

    if(archivoLadrillos){
        int i = 0;
        while (!archivoLadrillos.eof()){
            string idladrillo,xizq,xder,y,resistencia;
            getline(archivoLadrillos,idladrillo,',');
            ladrillos[i].id = idladrillo;
            getline(archivoLadrillos,xizq,',');
            ladrillos[i].x_izq = atof(xizq.c_str());
            getline(archivoLadrillos,xder,',');
            ladrillos[i].x_der = atof(xder.c_str());
            getline(archivoLadrillos,y,',');
            ladrillos[i].y = atof(y.c_str());
            getline(archivoLadrillos,resistencia,'\n');
            ladrillos[i].resistencia = atoi(resistencia.c_str());
            ladrillos[i].vive = true;
            i++;
        }
        cantidadLadrillos = i;
    }

    //Ordenamos ladrillos y balas por la coordenada y para procesar mas facil
    qsort(ladrillos,cantidadLadrillos-1,sizeof(ladrillo),comparLadrillos);
    qsort(balas,cantidadBalas-1,sizeof(bala),comparBalas);

    //for (int i = 0; i < cantidadBalas-1; i++){
    //    cout << balas[i].id << " " << balas[i].y <<  " " << endl;
    //}

    //for (int i = 0; i < cantidadLadrillos-1; i++){
    //    cout << ladrillos[i].id << " " << ladrillos[i].y <<  " " << endl;
    //}

    //Comienzo del procesamiento
    int contador = 0;
    for(int i = 0; i < cantidadBalas-1; i++){
        for (int j = 0; j < cantidadLadrillos; j++){
            cout << "Comparando " << balas[i].id << " con " << ladrillos[j].id << endl;
            if((balas[i].vive)&&(ladrillos[j].vive)){
                if (choca(balas[i],ladrillos[j])){
                    balas[i].vive = false;
                    ladrillos[j].resistencia = ladrillos[j].resistencia - balas[i].energia;

                    if(ladrillos[j].resistencia <= 0){
                        ladrillos[j].vive = false;
                        balas[i].y_final = devolverDistancia(balas[i],ladrillos[j]);

                        //Almacena datos en el array de resultados
                        resultados[contador].id_ladrillo = ladrillos[j].id;
                        resultados[contador].id_bala = balas[i].id;
                        resultados[contador].distancia = balas[i].y_final;
                        contador++;
                    }
                }
            }
        }
    }

    qsort(resultados,contador,sizeof(resultado),comparResultados);

    for (int i = 0; i < contador; i++){
        cout << "Id Ladrillo: " << resultados[i].id_ladrillo << endl;
        cout << "Id Bala: " << resultados[i].id_bala << endl;
        cout << "Distancia Recorrida: " << resultados[i].distancia << endl;
        cout << "......................" << endl;
    }

    archivoBalas.close();
    archivoLadrillos.close();
    return 0;
}
