#ifndef PERSONAJE_H
#define PERSONAJE_H

class Barril;

class Personaje {
    private:
        float x;
        float dim;
        char* id;
        float dist;
        bool marca;
    public:
        Personaje(char* id, float x);
        ~Personaje();

        void avanzar();
        float obtenerX();
        float obtenerDim();
        char* obtenerID();
        float obtenerDist();
        
        bool chocaConBarril(Barril* barril);

        void marcar();
        bool marcado();
};

#endif
