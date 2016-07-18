#include "Barril.h"

struct barril{
       char* id;
       float xi;
       float yi;
       float dMinima;     
};

float barril_obtener_xi(barril_t* barril){
      return barril->xi;
}

float barril_obtener_yi(barril_t* barril){
      return barril->yi;
}

barril_t* crear_barril ( char* id, float xi, float yi ){
              barril_t* nuevo = (barril_t*) malloc( sizeof(barril_t) );
              nuevo -> id = id;
              nuevo -> xi = xi;
              nuevo -> yi = yi;
              nuevo -> dMinima = 100000;
              return nuevo;
}

void asignarDistMin(barril_t* barril, float d){
     barril -> dMinima = d;
}

void swap( barril_t** barriles, int pos1, int pos2 ){
     barril_t* barrilAux = barriles[pos1];
     barriles[pos1] = barriles[pos2];
     barriles[pos2] = barrilAux;
}

int minimo ( barril_t** barriles , int cant,int posi){
    int minimo = posi;
    for (int i= posi; i< cant; i++ ){
        if ( barriles[i]->dMinima < barriles[minimo]->dMinima )
           minimo = i;
    }
    return minimo;
     
}


void ordenarBarriles (  barril_t** barriles , int cant ){
     for (int i = 0; i< cant; i++ ){
         int posMin = minimo( barriles, cant, i);
         swap ( barriles, i, posMin );
     }
     
     for (int i = 0; i< cant; i++ ){
         printf( "%s", barriles[i]->id  );
         printf( "%f \n" , barriles[i]->dMinima );
     }
}
