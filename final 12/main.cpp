#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <iostream>

#define MAX_DISPERSOR 20
#define MAX_TEJO 20
#define MAX_RESULTADO 20

using namespace std;

struct dispersor{
    string id;
    float x;
    float y;
    float alto;
    float ancho;
    bool chocado;
};

struct tejo{
    string id;
    float x;
    float y;
    float radio;
    bool chocado;
    float x_final;
};

struct resultado{
    string id_tejo;
    string id_dispersor;
    float distancia;
};

int comparTejos(const void* a, const void* b){
    if((*(tejo*)a).x < (*(tejo*)b).x) return -1;
    if((*(tejo*)a).x == (*(tejo*)b).x) return 0;
    if((*(tejo*)a).x > (*(tejo*)b).x) return 1;
}

int comparDispersores(const void* a, const void* b){
    if((*(dispersor*)a).x < (*(dispersor*)b).x) return -1;
    if((*(dispersor*)a).x == (*(dispersor*)b).x) return 0;
    if((*(dispersor*)a).x > (*(dispersor*)b).x) return 1;
}

int comparResultados(const void* a, const void* b){
    if((*(resultado*)a).distancia < (*(resultado*)b).distancia) return -1;
    if((*(resultado*)a).distancia == (*(resultado*)b).distancia) return 0;
    if((*(resultado*)a).distancia > (*(resultado*)b).distancia) return 1;
}

bool tocoPorArribaAlRectangulo(dispersor dispersor, tejo tejo){
    if((tejo.y - tejo.radio <= dispersor.y )&&(tejo.y >= dispersor.y + dispersor.alto)){
        return true;
    }
    return false;
}

bool tocoPorAbajoAlRectangulo(dispersor dispersor, tejo tejo){
    if((tejo.y + tejo.radio >= dispersor.y)&&(tejo.y + tejo.radio <= dispersor.y + dispersor.alto)){
        return true;
    }
    return false;
}

bool tocoUnVertice(dispersor dispersor, tejo tejo){
    if (tocoPorArribaAlRectangulo(dispersor, tejo) || tocoPorAbajoAlRectangulo(dispersor, tejo)){
        return true;
    }
    return false;
}

bool choca(dispersor dispersor, tejo tejo){
    if(tocoUnVertice(dispersor, tejo)){
        return true;
    } return false;
}

float calcularDistancia(tejo tejo, dispersor dispersor){
    float diferencia = dispersor.x - tejo.x;
    return sqrt(diferencia*diferencia);
}

int main(){
    ifstream archivoDispersores, archivoTejos;
    dispersor* dispersores = new dispersor[MAX_DISPERSOR];
    tejo* tejos = new tejo[MAX_TEJO];
    resultado* resultados = new resultado[MAX_RESULTADO];

    int cantidadTejos = 0, cantidadDispersores = 0;

    archivoTejos.open("circulos.txt");
    archivoDispersores.open("rectangulos.txt");

    if(archivoTejos){
        int i = 0;
        while (!archivoTejos.eof()){
            string idtejo,x,y,radio;
            getline(archivoTejos,idtejo,',');
            tejos[i].id = idtejo;
            getline(archivoTejos,x,',');
            tejos[i].x = atof(x.c_str());
            getline(archivoTejos,y,',');
            tejos[i].y = atof(y.c_str());
            getline(archivoTejos,radio,'\n');
            tejos[i].radio = atof(radio.c_str());
            tejos[i].x_final = tejos[i].x;
            tejos[i].chocado = false;
            i++;
        }
        cantidadTejos = i;
    }

    if(archivoDispersores){
        int i = 0;
        while (!archivoDispersores.eof()){
            string iddispersor,x,y,alto,ancho;
            getline(archivoDispersores,iddispersor,',');
            dispersores[i].id = iddispersor;
            getline(archivoDispersores,x,',');
            dispersores[i].x = atof(x.c_str());
            getline(archivoDispersores,y,',');
            dispersores[i].y = atof(y.c_str());
            getline(archivoDispersores,alto,',');
            dispersores[i].alto = atof(alto.c_str());
            getline(archivoDispersores,ancho,'\n');
            dispersores[i].ancho = atof(ancho.c_str());
            dispersores[i].chocado = false;
            i++;
        }
        cantidadDispersores = i;
    }

    qsort(tejos,cantidadTejos-1,sizeof(tejo),comparTejos);
    qsort(dispersores,cantidadDispersores-1,sizeof(dispersor),comparDispersores);

    //for (int i = 0; i < cantidadTejos-1; i++){
    //    cout << tejos[i].id << " " << tejos[i].x <<  " " << tejos[i].y <<  " " << tejos[i].radio <<  " " <<endl;
    //}

    //for (int i = 0; i < cantidadDispersores-1; i++){
    //    cout << dispersores[i].id << " " << dispersores[i].x_izq <<  " " << dispersores[i].x_der <<  " " << dispersores[i].alto <<  " " << dispersores[i].ancho <<  " " << endl;
    //}

    int cantidadResultados = 0;
    for(int i = 0; i < cantidadDispersores-1; i++){
        for(int j = 0; j < cantidadTejos-1; j++){
            if((!tejos[j].chocado)&&(!dispersores[i].chocado)){
                if(choca(dispersores[i],tejos[j])){
                    dispersores[i].chocado = true;
                    tejos[j].chocado = true;
                    tejos[j].x_final = calcularDistancia(tejos[j],dispersores[i]);
                    resultados[cantidadResultados].id_dispersor = dispersores[i].id;
                    resultados[cantidadResultados].id_tejo = tejos[j].id;
                    resultados[cantidadResultados].distancia = tejos[j].x_final;
                    cantidadResultados++;
                }
            }
        }
    }

    qsort(resultados,cantidadResultados,sizeof(resultado),comparResultados);

    for (int i = 0; i < cantidadResultados; i++){
        cout << "Id Dispersor: " <<resultados[i].id_dispersor << " Id Tejo: " << resultados[i].id_tejo <<  " Distancia: " << resultados[i].distancia << endl;
        cout << "......................" << endl;
    }

    archivoDispersores.close();
    archivoTejos.close();

    return 0;
}
