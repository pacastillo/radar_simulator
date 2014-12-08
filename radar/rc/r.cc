#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;


int main(void) {
  unsigned grados=0, hay_objeto=0;
  unsigned a1=0,a2=0;

  for(unsigned cont=0; cont<10; cont++){
    // avanzar 1 grado la línea...
    grados++;

    // comprobar si hay objeto
    hay_objeto=grados%2;

    // abrir para machacar el fichero de S    
    ofstream fo;
    fo.open( "S" , ios::trunc);
    fo << grados << " " << hay_objeto << endl;
    fo.close();
    fo.clear();


    // si hay datos en A => leer los datos ; procesar los datos ; vaciar fichero A
    ifstream fi;
    fi.open( "A" );
    if( fi ) {
      unsigned hay_datos=0;
      fi >> a1;
      if( !fi.eof() ){ // puede estar vacío => ignorar...
	hay_datos=1;
      }
      fi.close();
      fi.clear();

      if(hay_datos) {
	fi.open( "A" );
	fi >> a1;
	fi >> a2;
	fi.close();
	fi.clear();
	
	ofstream tmp;
	tmp.open( "A" , ios::trunc);
	tmp.close();
	tmp.clear();  
      }    
    }

    // repintar
    cout << "(RADAR) envia\t grados="<<grados<<"\t hay_objeto="<<hay_objeto<< endl;
    cout << "(RADAR) recibe\t A1="<<a1<<"\t A2="<<a2<< endl << endl;

    // esperar
    sleep(2);
  }

  return 0;
}
