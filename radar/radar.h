#include <math.h>
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/colormap.h>
#include <gdkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/dialog.h>
#include <cmath>
#include <sstream>
#include <vector>
#include <pangomm/fontdescription.h>
#include <gtkmm/statusbar.h>
#include "sincronizador.h"

//*****************************************************************************
#define def_NOMBRE_VERSION  "Simulador de Radar V0.2 R31m04"

#ifndef M_PI
#define M_PI 3.14159265359
#endif /* M_PI */
#define DEG2RAD .01745329251994444444   // (M_PI/180)

#define def_ANCHO_ARENA  400
#define def_ALTO_ARENA   400


#define COLOR_NEGRO    0
#define COLOR_BLANCO   1
#define COLOR_AZUL     2
#define COLOR_ROJO     3
#define COLOR_VERDE    4
#define COLOR_AMARILLO 5
#define COLOR_GRIS     6
#define COLOR_LILA     7
#define COLOR_OBSTACULO COLOR_AZUL
#define COLOR_RESIDUO   COLOR_ROJO
#define COLOR_LIMPIO    COLOR_GRIS
#define COLOR_RASTRO    COLOR_LILA

//*****************************************************************************

class MyDialog : public Gtk::Dialog {
public:
  // create one dialog only
  static MyDialog* instance() {
    if( _instance == 0 ) {
      _instance = new MyDialog;
    }
    return _instance;
  }

protected:
  MyDialog() {
    //set_size_request(300, 200);
    set_resizable(false);
    set_title("Acerca de...");

    Gtk::Label *label1 = manage(new Gtk::Label( def_NOMBRE_VERSION , true ) );
    get_vbox()->pack_start(*label1);
    label1->show();

    Gtk::Label *label3 = manage(new Gtk::Label(" ",true));
    get_vbox()->pack_start(*label3);
    label3->show();
    Gtk::Label *label4 = manage(new Gtk::Label("(c) Pedro A. Castillo Valdivieso",true));
    get_vbox()->pack_start(*label4);
    label4->show();
    Gtk::Label *label5 = manage(new Gtk::Label(" ",true));
    get_vbox()->pack_start(*label5);
    label5->show();

    Gtk::Label *label6 = manage(new Gtk::Label("Universidad de Granada",true));
    get_vbox()->pack_start(*label6);
    label6->show();
    Gtk::Label *label7 = manage(new Gtk::Label(" ",true));
    get_vbox()->pack_start(*label7);
    label7->show();

    Gtk::Button *button=manage(new Gtk::Button("Cerrar"));
    get_action_area()->pack_start(*button);
    button->signal_clicked().connect(SigC::slot(*this, &Widget::hide));
    button->show();
  }

private:
  static MyDialog* _instance;
};


//*****************************************************************************
