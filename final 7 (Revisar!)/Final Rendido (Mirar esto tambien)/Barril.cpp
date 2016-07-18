#include "Barril.h"

Barril::Barril(float x, float y) {
    this->x = x;
    this->y = y;
    this->dim = 1;
    this->marca = false;
}

Barril::~Barril() {}

void Barril::caer() {
    y -= 0.001f;
}

float Barril::obtenerX() {
    return x;
}

float Barril::obtenerY() {
    return y;
}

float Barril::obtenerAncho() {
    return dim;
}

float Barril::obtenerAlto() {
    return dim;
}

bool Barril::marcado() {
    return marca;
}

void Barril::marcar() {
    marca = true;
}
