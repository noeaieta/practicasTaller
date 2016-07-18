#include "Nivel.h"

struct nivel{
       plataforma_t** plataformas;
       int cant;
       float yNivel;
};

nivel_t* crear_Nivel ( plataforma_t** plataformas, int cant, float yNivel ){
       nivel_t* nuevo = (nivel_t*) malloc( sizeof(nivel_t) );
       nuevo-> plataformas= plataformas;
       nuevo->cant = cant;
       nuevo->yNivel = yNivel;
       return nuevo;
}

float nivel_obtener_y(nivel_t* nivel){
      return nivel->yNivel;
}

int nivel_obtener_cant(nivel_t* nivel){
      return nivel->cant;
}

plataforma_t** nivel_obtener_plataformas(nivel_t* nivel){
      return nivel->plataformas;
}


void swap( nivel_t** niveles, int pos1, int pos2 ){
     nivel_t* nivelAux = niveles[pos1];
     niveles[pos1] = niveles[pos2];
     niveles[pos2] = nivelAux;
}

int minimo ( nivel_t** niveles , int cant,int posi){
    int minimo = posi;
    for (int i= posi - 1; i >= 0 ; i-- ){
        if ( niveles[i]->yNivel < niveles[minimo]->yNivel )
           minimo = i;
    }
    return minimo;
     
}


void ordenarNiveles (  nivel_t** niveles , int cant ){
     for (int i = cant - 1 ; i> 0; i-- ){
         int posMin = minimo( niveles, cant, i);
         swap ( niveles, i, posMin );
     }
     
     //for (int i = 0; i< cant; i++ ){
         //printf( "%f \n" , niveles[i]->yNivel );
     //}
}
