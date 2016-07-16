#include "Plataforma.h"

struct plataforma{
       float xi;
       float xf;
       float yNivel;
};

plataforma_t* crear_plat ( float xi, float xf, float yNivel ){
              plataforma_t* nueva = (plataforma_t*) malloc( sizeof(plataforma_t) );
              nueva -> xi = xi;
              nueva -> xf = xf;
              nueva -> yNivel = yNivel;
              return nueva;
}

bool aterrizaEnPlat(plataforma_t* p, float x){
     if( x<  p->xi )
         return false;
     if( x>  p->xf )
         return false;
     return true;
}

float plat_obtener_xi(plataforma_t* plat){
      return plat->xi;
}

float plat_obtener_xf(plataforma_t* plat){
      return plat->xf;
}
