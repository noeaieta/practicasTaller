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

//funcion recursiva
float minimaD( barril_t* barril){
    //recorro todos los niveles, que ya estan ordenados descendentemente
    for( int i= 0; i<cantNiv; i++ ){
         float yNivel = nivel_obtener_y(losNiveles[i]);
         //si el nivel esta mas arriba del barril, lo descarto
         if ( yNivel >= barril_obtener_yi( barril )  )
            continue;
         //sino, obtengo todas las plataformas del nivel
         plataforma_t** plats = nivel_obtener_plataformas( losNiveles[i] );
         int cantPlats = nivel_obtener_cant( losNiveles[i] ) ;
         
         //para cada plataforma
         for (int j=0; j< cantPlats; j++ ){
             //reviso, si choco con alguna, entonces:
             if( aterrizaEnPlat(plats[j], barril_obtener_xi(barril) ) ){
                 //guardo posiciones en x del la plataforma
                 float plat_xi = plat_obtener_xi(plats[j]);
                 float plat_xf = plat_obtener_xf(plats[j]);
                 float barril_x = barril_obtener_xi(barril);
                 //creo un barril justo en la pos donde volveria a caer, por la derecha
                 barril_t* b1 = crear_barril ( NULL, plat_xi , yNivel );
                 //creo un barril justo en la pos donde volveria a caer, por la izquierda
                 barril_t* b2 = crear_barril ( NULL, plat_xf , yNivel );

                 //yaRecorrido es la distancia que seguro se recorrio, desde la pos inicial hasta el nivel actual
                 float yaRecorrido = barril_obtener_yi(barril) - yNivel;

                 //dos llamadas recursivas, de las cuales me quedo con la que se recorre el minimo
                 //tomando en cuenta lo que se recorre en x para llegar al borde de la plataforma, y como caeria desde ese borde hasta el final
                 return  yaRecorrido + minimo ( minimaD(b1) + (  barril_x - plat_xi  ) , minimaD(b2) + ( plat_xf  -  barril_x  ) );
             }
             
         }
         
    }
    
    //condicion de corte: si llego aca es que cayo desde su pos inicial hasta el piso
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
