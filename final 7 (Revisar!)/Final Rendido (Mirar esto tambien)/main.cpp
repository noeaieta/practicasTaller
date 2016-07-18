#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstring>
#include <vector>
#include "Barril.h"
#include "Personaje.h"

using namespace std;

vector<Barril*> barriles;
vector<Personaje*> personajes;
vector<Personaje*> chocados;

void leerBarriles(char* archivo) {
     ifstream file(archivo);
     
     char* cstr;
     string linea;
     
     while(getline(file, linea)) {
        cstr = new char [linea.size() +1];
        strcpy(cstr, linea.c_str());

        char* x = strtok(cstr, " ");
        char* y = strtok(NULL, " ");

        barriles.push_back(new Barril(strtof(x, NULL),strtof(y, NULL)));
        
        delete cstr;
     }

    file.close();
}

void leerPersonajes(char* archivo) {
     ifstream file(archivo);
     
     char* cstr;
     string linea;
     
     while(getline(file, linea)) {
        cstr = new char [linea.size() +1];
        strcpy(cstr, linea.c_str());

        char* id = strtok(cstr, " ");
        char* x = strtok(NULL, " ");

        personajes.push_back(new Personaje(strdup(id),strtof(x, NULL)));
        
        delete cstr;
     }

    file.close();
}

void imprimirResultados() {
    for (unsigned int i = 0; i < chocados.size(); ++i) {
        cout << chocados.at(i)->obtenerID() << " - " << chocados.at(i)->obtenerDist() << endl;
    }
}

bool mayorDistancia(Personaje* p1, Personaje* p2) {
    return p1->obtenerDist() < p2->obtenerDist();
}

void moverPersonajes() {
    for (unsigned int i = 0; i < personajes.size(); ++i) {
        // Si fue marcado, lo ignoro:
        if (personajes.at(i)->marcado())
            continue;
        personajes.at(i)->avanzar();
    }
}

void moverBarriles() {
    for (unsigned int j = 0; j < barriles.size(); ++j) {
       // Si fue marcado, lo ignoro:
       if (barriles.at(j)->marcado()) 
           continue;
       barriles.at(j)->caer();
    }
}

void verificarColisiones() {
    // Verifico colisiones:
    for (unsigned int i = 0; i < personajes.size(); ++i) {
        // Si fue marcado, lo ignoro:
        if (personajes.at(i)->marcado())
            continue;
        for (unsigned int j = 0; j < barriles.size(); ++j) {
            if (barriles.at(j)->obtenerY() <= 0) {
                barriles.at(j)->marcar();
//                cout << "CHAU BARRIL: " << j << endl;
            }

            // Si fue marcado, lo ignoro:
            if (barriles.at(j)->marcado()) 
                continue;

            // Si choque, los marco y agrego a la lista de chocados:
            if (personajes.at(i)->chocaConBarril(barriles.at(j))) {
                personajes.at(i)->marcar();
                barriles.at(j)->marcar();
                chocados.push_back(personajes.at(i));
//                cout << "CHOQUE" << personajes.at(i)->obtenerID() << " - " << personajes.at(i)->obtenerDist() << endl;
                break;
            }
        }
    }
}

void actuarTodo() {
    bool terminar1 = false;
    bool terminar2 = false;

    int t = 0;
    int ciclos = 0;
    while (!terminar1 && !terminar2) {
        while (t <= 1000) {
//            cout << "B: " << t << endl;
            moverBarriles();
            if (t % 2 == 0) {
//                cout << "P: " << t << endl;
                moverPersonajes();
            }
            verificarColisiones();
            t += 1;
        }
        t = 0;

        // Verifico si tengo que terminar:
        for (unsigned int i = 0; i < personajes.size(); ++i) {
            terminar1 = true;
/*            if (personajes.at(i)->marcado())
                cout << "Marcado: " << personajes.at(i)->obtenerID() << endl;*/
            if (!personajes.at(i)->marcado()) {
                terminar1 = false;
                break;
            }
        }
    
        for (unsigned int j = 0; j < barriles.size(); ++j) {
            terminar2 = true;
/*            if (barriles.at(j)->marcado())
                cout << "Marcado: " << j << endl;*/
            if (!barriles.at(j)->marcado()) {
                terminar2 = false;
                break;
            }
        }
        ciclos++;
    }
//    cout << ciclos << endl;
}

int main(int argc, char *argv[])
{
    leerBarriles(argv[1]);
    leerPersonajes(argv[2]);
    
    actuarTodo();
    
    sort(chocados.begin(), chocados.end(), mayorDistancia);
    imprimirResultados();

    system("PAUSE");
    return EXIT_SUCCESS;
}
