#include "Personaje.h"
#include "Barril.h"

#include <cstdlib>
using namespace std;

Personaje::Personaje(char* id, float x) {
    this->x = x;
    this->dim = 2;
    this->id = id;
    this->dist = 0;
    this->marca = false;
}

Personaje::~Personaje() {
    if (id)
        free(id);
}

void Personaje::avanzar() {
    x += 0.001f;
    dist += 0.001f;
}

float Personaje::obtenerX() {
    return x;
}

char* Personaje::obtenerID() {
    return id;
}

float Personaje::obtenerDim() {
    return dim;
}

float Personaje::obtenerDist() {
    return dist;
}

bool Personaje::chocaConBarril(Barril* barril) {
    if (x > (barril->obtenerX() + barril->obtenerAncho()) || barril->obtenerX() > (x + dim))
        return false;
    if ((0 + dim) < barril->obtenerY() || (barril->obtenerY() + barril->obtenerAlto()) < 0)
        return false;
    return true;
}


bool Personaje::marcado() {
    return marca;
}

void Personaje::marcar() {
    marca = true;
}
