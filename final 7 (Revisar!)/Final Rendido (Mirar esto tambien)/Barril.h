#ifndef BARRIL_H
#define BARRIL_H

class Barril {
    private:
        float x, y;
        float dim;
        bool marca;

    public:
        Barril(float x, float y);
        ~Barril();

        void caer();
        float obtenerX();
        float obtenerY();
        float obtenerAncho();
        float obtenerAlto();
        void marcar();
        bool marcado();
};

#endif
