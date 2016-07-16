#include <fstream>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <stdio.h>
using namespace std;

#define CANTIDAD_DE_DISPAROS 3

struct Disparo {
    string id;
    double xi;
    double yi;
    double xd;
    double yd;
    double distancia;
    bool fueGol;
};

struct Arco {
    double x;
    double y;
    double ancho;
};

void cargarArchivoDisparos(Disparo* v) {
    ifstream file;
    file.open("disparos.txt");
    string value;
    cout << "Cargo Disparos" << endl;
    for(int i = 0; i < CANTIDAD_DE_DISPAROS; i++){
        getline(file, v[i].id, ',');
        getline(file, value, ',');
        v[i].xi = atof(value.c_str());
        getline(file, value, ',');
        v[i].yi = atof(value.c_str());
        getline(file, value, ',');
        v[i].xd = atof(value.c_str());
        getline(file, value, '\n');
        v[i].yd = atof(value.c_str());
        v[i].distancia = -1;
        v[i].fueGol = false;
        cout << v[i].id << "\t" << v[i].xi << "\t" << v[i].yi << "\t" << v[i].xd << "\t" << v[i].yd << "\t" << v[i].distancia << "\t" << endl;
    }
    file.close();
}

void cargarArchivoArco(Arco &a) {
    ifstream file;
    file.open("arco.txt");
    string value;
    cout << "Cargo el Arco" << endl;
    getline(file, value, ',');
    a.y = atof(value.c_str());
    getline(file, value, ',');
    a.x = atof(value.c_str());
    getline(file, value, '\n');
    a.ancho = atof(value.c_str());
    cout << "y:"<< a.y << "\t x:" << a.x << "\t ancho:" << a.ancho << endl;
    file.close();
}

double calcularPendiente(double x,double y,double xd, double yd) {
    return ((y-yd)/(x-xd));
}

double calcularOrdenada(double x,double y,double xd, double yd) {
    double m = calcularPendiente( x , y , xd , yd);
    return (((y+yd)-(m*(x+xd)))/2);
}

double calcularDistancia(double x,double y,double xd, double yd) {
    return sqrt(pow((x-xd),2)+pow((y-yd),2));
}

bool ordenar(const Disparo l, const Disparo r){
    return l.distancia > r.distancia;
}

int main() {    
    Arco a;
    Disparo v[CANTIDAD_DE_DISPAROS];
    cargarArchivoArco(a);
    cargarArchivoDisparos(v);

    double m, b, xInt;
    for (int i = 0; i < CANTIDAD_DE_DISPAROS; i++ ) {
        if (v[i].xi != v[i].xd){
            m = calcularPendiente( v[i].xi , v[i].yi , v[i].xd , v[i].yd);
            b = calcularOrdenada( v[i].xi , v[i].yi , v[i].xd , v[i].yd);
            xInt = ((a.y - b)/m);
        }
        else {
            xInt = v[i].xi;
        }
        v[i].distancia = calcularDistancia(v[i].xi, v[i].yi, xInt, a.y);

        if ( (a.x < xInt) && (xInt < (a.x + a.ancho))){
            v[i].fueGol = true;
        }
    }

    sort(v, v+CANTIDAD_DE_DISPAROS, ordenar); 
    
    cout << "\nResultado:" << endl;
    for (int i = 0; i < CANTIDAD_DE_DISPAROS; i++ ) {
        if(v[i].fueGol){
            cout << "GOOOOLL de " << v[i].id << " a distancia:" << v[i].distancia << "\t" << endl;
        }
    }

    for (int i = 0; i < CANTIDAD_DE_DISPAROS; i++ ) {
        if(!v[i].fueGol){
            cout << "corner de " << v[i].id << endl;
        }
    }
        

    return 0;
}