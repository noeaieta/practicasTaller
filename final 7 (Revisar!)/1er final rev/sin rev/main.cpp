#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "Barril.h"
#include "Plataforma.h"
#include "Nivel.h"

#define N 255
#define NIVS 8
#define PLATS 10
#define BARRS 100


using namespace std;

nivel_t** losNiveles;
barril_t** barriles;
int cantNiv;
int cantB;



void parsearPlataformas(){
                cantNiv = 0;
                int cantPlat = 0;
                FILE* archivo = fopen( "niveles.txt" , "r");
                char c;
                losNiveles = (nivel_t**) malloc( sizeof(nivel_t*) * NIVS );

                while( !feof(archivo) ){
                       c = fgetc(archivo);
                       char* linea = (char*) malloc ( sizeof(char) * (N+1) );
                       int i = 0;
                       while( (!feof(archivo)) && (c != '\n') ){
		                      linea[i] = c;
                              i++;
			                  c = fgetc(archivo);
                       }
                       linea[i] = '\n';
                       
                       //if( feof(archivo) )
                           //continue;
                           
                       char* y = strtok(linea, "," ) ;
                       float yNivel =  strtof(y, NULL);
                       
                       plataforma_t** plataformas = (plataforma_t**) malloc( sizeof(plataforma_t*) * PLATS );
                       
                       while ( char* xinicial = strtok(NULL, ", " ) ){
                             char* xfinal = strtok(NULL, ", " );
                             plataforma_t* nueva = crear_plat( strtof(xinicial, NULL ) , strtof(xfinal, NULL ) , yNivel );
                             plataformas[cantPlat] = nueva;
                             cantPlat++ ;
                       }
                       
                       nivel_t* nuevoNivel = crear_Nivel ( plataformas, cantPlat , yNivel );
                       losNiveles[cantNiv] = nuevoNivel;
                       cantPlat = 0;
                       cantNiv++ ;
                }
}

void parsearBarriles(){
                cantB = 0;
                FILE* archivo = fopen( "barriles.txt" , "r");
                char c;
                barriles = (barril_t**) malloc( sizeof(barril_t*) * BARRS );

                while( !feof(archivo) ){
                       c = fgetc(archivo);
                       char* linea = (char*) malloc ( sizeof(char) * (N+1) );
                       int i = 0;
                       while( (!feof(archivo)) && (c != '\n') ){
		                      linea[i] = c;
                              i++;
			                  c = fgetc(archivo);
                       }
                       linea[i] = '\n';
                       
                       //if( feof(archivo) )
                           //continue;
                           
                       char* id = strtok(linea, "," ) ;
                       char* xinicial = strtok(NULL, ", " );
                       char* yinicial = strtok(NULL, ", " );
                       barril_t* nuevo = crear_barril( id, strtof(xinicial, NULL ) , strtof(yinicial, NULL ) );
                       barriles[cantB] = nuevo;
                       cantB++ ;
                }
}


float minimo(float x, float y){
      if (x<y)
         return x;
      
      return y;
}

float minimaD( barril_t* barril){
    for( int i= 0; i<cantNiv; i++ ){
         float yNivel = nivel_obtener_y(losNiveles[i]);
         if ( yNivel >= barril_obtener_yi( barril )  )
            continue;
         plataforma_t** plats = nivel_obtener_plataformas( losNiveles[i] );
         int cantPlats = nivel_obtener_cant( losNiveles[i] ) ;
         for (int j=0; j< cantPlats; j++ ){
             
             if( aterrizaEnPlat(plats[j], barril_obtener_xi(barril) ) ){
                 barril_t* b1 = crear_barril ( NULL, plat_obtener_xi(plats[j]) , yNivel );
                 barril_t* b2 = crear_barril ( NULL, plat_obtener_xf(plats[j]) , yNivel );
                 return barril_obtener_yi(barril) - yNivel +  minimo ( minimaD(b1) + (barril_obtener_xi(barril) - plat_obtener_xi(plats[j])  ) , minimaD(b2) + (plat_obtener_xf(plats[j])  -  barril_obtener_xi(barril)  ) );
             }
             
         }
         
    }
    
    return barril_obtener_yi(barril) ;
}



int main(int argc, char *argv[])
{	
    parsearBarriles();	
    
    parsearPlataformas();
    ordenarNiveles(losNiveles, cantNiv);
    
    for ( int i = 0; i< cantB; i++ ){
        float d = minimaD( barriles[i]);
        asignarDistMin( barriles[i], d);
    }
    
    
    ordenarBarriles(barriles, cantB);
    system("PAUSE");
    return EXIT_SUCCESS;
}
