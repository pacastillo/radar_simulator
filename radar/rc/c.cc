#include "sincronizador.h"

int main(void) {
  unsigned grados=0, hay_objeto=0;
  unsigned a1=0,a2=0;

  for(unsigned cont=0; cont<10; cont++){
    // leer del fichero S (lo que manda el radar) y dejarlo vacio
    sincronizador *sincr = new sincronizador();
    vector<unsigned> valores = sincr->recibir("S");

    // procesar los datos leidos
    grados = valores[0];
    hay_objeto = valores[1];
    a1=grados;
    a2=hay_objeto*7;

    vector<unsigned> acciones;
    acciones.push_back(a1); 
    acciones.push_back(a2);

    // escribir en A machacando lo que tenga el fichero
    sincr->enviar("A",acciones);


    cout << "(CTRL) recibe\t grados="<<grados<<"\t hay_objeto="<<hay_objeto<< endl;
    cout << "(CTRL) envia\t A1="<<a1<<"\t A2="<<a2<< endl << endl ;

    sleep(3);
  }

  return 0;
}
