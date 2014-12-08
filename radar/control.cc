#include "sincronizador.h"

#ifndef M_PI
#define M_PI 3.14159265359
#endif /* M_PI */
#define DEG2RAD .01745329251994444444   // (M_PI/180)

int main(int argc, char** argv) {
  srand( time(0) );

  cout << "\n(c)2001 - Pedro Angel Castillo Valdivieso"<<endl;
  cout << "\nOPCIONES: "<<argv[0]<<"  [disparar_aleatoriamente] "<<endl;

  unsigned aleatorio=0;
  if( argv[1] ) aleatorio=1;

  unsigned x,y;

  for(unsigned cont=0; cont<500; cont++){
    // RECOGER valor del ángulo y objetivo (si hay)
    // APLICAR procesar
    // ENVIAR coordenada del disparo

    sincronizador *sincr = new sincronizador();
    vector<unsigned> radar = (vector<unsigned>) sincr->recibir("S");

    cout << "\n angulo="<< radar[0] << "\t radio="<< radar[1] ;

    vector<unsigned> disparo;

    if( !aleatorio ){
      if( radar[1] ) {
	double tmp_ang = (double)( (double)radar[0] * (double)DEG2RAD );
	x = (unsigned) 200 + (int) (radar[1] * cos(tmp_ang));
	y = (unsigned) 200 - (int) (radar[1] * sin(tmp_ang));
      }else{
        x=y=0;
      } 
    }else{
      x = 50+(int) (300.0*rand()/(RAND_MAX+1.0));
      y = 50+(int) (300.0*rand()/(RAND_MAX+1.0));
    }

    disparo.push_back(x);
    disparo.push_back(y);
    sincr->enviar("A", disparo);

    cout << "\t  =>  ("<<x<<","<<y<<")"<<endl;

    sleep(1);
  }

  return 0;
}


/*
    - ir recogiendo los datos de todos los objetivos detectados por el radar
    - ir guardando en fichero los datos que manda el radar y la coordenada de disparo generada por el control
    - si no dispara en esa ocasión, entonces devolver (0,0)
    - si te llega un radio igual 0, es por que no hay objetivo en ese momento en la línea del radar
*/
