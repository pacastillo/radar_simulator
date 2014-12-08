#include "radar.h"

//*****************************************************************************
//*****************************************************************************
unsigned varDURACION_OBJETIVO = 10;   // valor alto => más tiempo queda en pantalla
unsigned varDURACION_DISPARO  = 10;   // valor alto => más tiempo queda en pantalla
unsigned varVELOCIDAD = 10;           // valor alto => lentitud del simulador
unsigned varDENSIDAD_OBJETIVOS = 5;   // valor alto => más objetivos al mismo tiempo

int angulo = 0;
int incrAngulo = 1;                   // valor alto => más num.grados avanza en cada paso

bool modoGrafico = true;

unsigned num_aciertos = 0;
unsigned num_disparos = 0;
unsigned num_objetivos= 0;

//*****************************************************************************
// estructura y vector para los objetos (que aparezcan un tiempo)
struct objeto {
  unsigned x;
  unsigned y;
  unsigned tiempo;
  unsigned alcanzado;
};
vector<objeto> objetivos;
vector<objeto> disparos;


//*****************************************************************************
void texto_comunicacion();


//*****************************************************************************
MyDialog* MyDialog::_instance(0);

//*****************************************************************************
//*****************************************************************************

class Arena : public Gtk::DrawingArea {
public:
  Arena();
  virtual ~Arena();
  bool timer_callback();
  void redibujar();

protected:
  // manejadores de eventos
  virtual void on_realize();
  virtual bool on_expose_event(GdkEventExpose* event);

  Glib::RefPtr<Gdk::GC> gc_;
  Gdk::Color blue_, red_, green_, black_, white_, grey_, yellow_, lila_;
  Glib::RefPtr<Gdk::Colormap> colormap;
};


Arena::Arena() {
  // inicializar colores; el resto se hace en   on_realize()
  colormap = get_default_colormap();

  blue_  = Gdk::Color("blue");
  red_   = Gdk::Color("red");
  green_ = Gdk::Color("green");
  black_ = Gdk::Color("black");
  white_ = Gdk::Color("white");
  grey_  = Gdk::Color("grey");
  yellow_= Gdk::Color("yellow");

  lila_  = Gdk::Color("yellow");
  lila_.set_rgb(50000,0,50000);

  colormap->alloc_color(blue_);
  colormap->alloc_color(red_);
  colormap->alloc_color(green_);  
  colormap->alloc_color(black_);
  colormap->alloc_color(white_);
  colormap->alloc_color(grey_);
  colormap->alloc_color(yellow_);
  colormap->alloc_color(lila_);

  // timeout
  //Glib::signal_timeout().connect(SigC::slot(*this, &Arena::timer_callback), 500);

  add_events(Gdk::EXPOSURE_MASK);
}

Arena::~Arena() {}


void Arena::on_realize() {
  Gtk::DrawingArea::on_realize();

  // inicializar el resto de cosas
  Glib::RefPtr<Gdk::Window> window = get_window();

  gc_ = Gdk::GC::create(window);

  window->set_background(black_);
  window->clear();

  gc_->set_foreground(black_);
}


bool Arena::on_expose_event(GdkEventExpose*) {

  Glib::RefPtr<Gdk::Window> window = get_window();
  // obtener los tamaños de ventana para "repintar" en función de estos
  int winx, winy, winw, winh, wind;
  window->get_geometry(winx, winy, winw, winh, wind);
  window->clear();

  int bounding;
  if (winw>winh) bounding = winh-5; else bounding = winw-5;
  gc_->set_foreground(grey_);
  gc_->set_line_attributes(/*line_width*/2,/*LineStyle*/Gdk::LINE_ON_OFF_DASH,/*CapStyle*/Gdk::CAP_NOT_LAST,/*JoinStyle*/Gdk::JOIN_MITER);  
  window->draw_arc(gc_, 0, winw/2-(bounding/2), winh/2-(bounding/2), bounding, bounding, 0, 360*64);

  // green rotating line  
  double radius;
  double tmp_ang = (double)( (double)angulo * (double)DEG2RAD );
  int x1, y1, x2, y2;
  radius = winw/2.0;
  if (winw>winh) radius = winh/2.0; else radius = winw/2.0;
  x1 = winw/2;
  y1 = winh/2;
  x2 = x1 + (int) (radius * cos(tmp_ang));
  y2 = y1 - (int) (radius * sin(tmp_ang));
  gc_->set_foreground(green_);
  gc_->set_line_attributes(/*line_width*/2,/*LineStyle*/Gdk::LINE_SOLID,/*CapStyle*/Gdk::CAP_NOT_LAST,/*JoinStyle*/Gdk::JOIN_MITER);  
  window->draw_line(gc_, x1, y1, x2, y2);


  //pintar los OBJETIVOS que hay encolados en el vector...
  for(unsigned j=0; j<objetivos.size(); j++) {
    if( objetivos[j].alcanzado )
      gc_->set_foreground(lila_);
    else
      gc_->set_foreground(blue_);
    window->draw_arc(gc_, 1,  objetivos[j].x,objetivos[j].y,  15,15,  0,360*64);
    objetivos[j].tiempo -- ;
    if( objetivos[j].tiempo == 0 ) {
      vector<objeto>::iterator it = objetivos.begin()+j;
      objetivos.erase(it);
    }
  }

  //pintar los DISPAROS que hay encolados en el vector...
  for(unsigned j=0; j<disparos.size(); j++) {
    gc_->set_foreground(red_);
    window->draw_arc(gc_, 1,  disparos[j].x,disparos[j].y,  disparos[j].tiempo,disparos[j].tiempo,  0,360*64);
    disparos[j].tiempo -- ;
    if( disparos[j].tiempo == 0 ) {
      vector<objeto>::iterator it = disparos.begin()+j;
      disparos.erase(it);      
    }
  }

  return true;
}


bool Arena::timer_callback() {
 queue_draw();
 // si dev=1 -> seguir el callback ; si dev=0 -> no se llama más
 //return 1;
 return 0;
}

void  Arena::redibujar() {
  queue_draw();
}


//*****************************************************************************


class Ventana : public Gtk::Window {
public:
  Ventana();
  virtual ~Ventana();

  void actualizar_status();
  void parar();
  void continuo();
  void girar();
  bool callback();

  void obstaculo_movil();
  void ctrl_reiniciar();

  
protected:
  // Child widgets
  Gtk::VBox m_box0;
  Gtk::HBox m_box1;
  Gtk::VBox m_box2;
  Gtk::HBox m_box3; //empty box
  Gtk::HBox m_box4; //empty box

  Gtk::Button m_button1, m_button2, m_button3, m_button4, m_button5, m_button6;

  Gtk::Statusbar m_status_bar;
  unsigned int m_context_id;

  Arena m_area;

  bool moverse;
  unsigned ctrl_velocidad;
};


Ventana::Ventana()
  : m_box0(/*homogeneous*/false, /*spacing*/5), m_box1(false, 5), m_box2(false, 5), m_box3(false, 5), m_box4(false, 5),
    m_button1("Salir"), m_button2("Parar"), 
    m_button3("Continuo"), m_button4("Obst. movil"), 
    m_button5("Ayuda"), m_button6("Reiniciar"), m_area()
{

  set_title( def_NOMBRE_VERSION );
  set_resizable(false);

  m_context_id = m_status_bar.get_context_id( def_NOMBRE_VERSION );

  // box2
  m_button1.signal_clicked().connect(SigC::slot(*this, &Gtk::Widget::hide));  // conectar con SALIR
  m_button2.signal_clicked().connect(SigC::slot(*this, &Ventana::parar));  // conectar con PASOaPASO
  m_button3.signal_clicked().connect(SigC::slot(*this, &Ventana::continuo));  // conectar con CONTINUO
  //  m_button4.signal_clicked().connect(SigC::slot(*this, &Ventana::obstaculo_movil));  // conectar con OBSTACULO_MOVIL
  m_button6.signal_clicked().connect(SigC::slot(*this, &Ventana::ctrl_reiniciar));  // conectar con CTRL_REINICIAR
  //m_button5.signal_clicked().connect(SigC::slot(*this, &Ventana::about));  // conectar con ABOUT    
  MyDialog* dialog = MyDialog::instance();
  m_button5.signal_clicked().connect(SigC::slot(*dialog, &Widget::show));  // cuadro de ABOUT  


  m_box2.pack_start(m_box3, /*Gtk::PackOptions*/Gtk::PACK_EXPAND_WIDGET, /*padding*/5);
  m_box2.pack_start(m_button2, Gtk::PACK_SHRINK, 5);
  m_box2.pack_start(m_button3, Gtk::PACK_SHRINK, 5);
  //  m_box2.pack_start(m_button4, Gtk::PACK_SHRINK, 5);
  m_box2.pack_start(m_button6, Gtk::PACK_SHRINK, 5);


  m_box2.pack_start(m_box4, /*Gtk::PackOptions*/Gtk::PACK_EXPAND_WIDGET, /*padding*/5);
  m_box2.pack_start(m_button5, Gtk::PACK_SHRINK, 5);  // ACERCA DE
  m_box2.pack_start(m_button1, Gtk::PACK_SHRINK, 5);  // SALIR

  
  // box1
  m_area.set_size_request(def_ANCHO_ARENA, def_ALTO_ARENA);
  m_box1.pack_start(m_area, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box1.pack_start(m_box2, Gtk::PACK_SHRINK, 5);
    
  // box0
  m_box0.pack_start(m_box1, Gtk::PACK_EXPAND_WIDGET, 5);  
  m_box0.pack_start(m_status_bar);
  
  set_border_width(10);
  add(m_box0);
  show_all();

  Glib::signal_timeout().connect(slot(*this,&Ventana::callback), 50);

  moverse = false;
  ctrl_velocidad=0;
}

Ventana::~Ventana() {}

void Ventana::actualizar_status() {
  m_status_bar.pop(m_context_id);

  char buff[150];
  g_snprintf(buff, 80, "   a=%d   num.objetivos=%d  disparos=%d  aciertos=%d " , angulo, num_objetivos, num_disparos, num_aciertos );
  m_status_bar.push(buff, m_context_id);
}

void Ventana::parar() {
  moverse = false;
}

void Ventana::continuo() {
  moverse = true;
}

void Ventana::girar() {
  ctrl_velocidad++;
  if( ctrl_velocidad > varVELOCIDAD ) {
    ctrl_velocidad=0;
    angulo = angulo - incrAngulo ;
    if( angulo < 0 )   angulo = 360 + angulo;
    if( angulo > 360 ) angulo = 360 - angulo;
    // INICIO : llamar a la función que hace el procesamiento
    texto_comunicacion();
    // FIN : llamar a la función que hace el procesamiento
  }  
  actualizar_status();
  m_area.redibujar();
}

bool Ventana::callback() {
  if( moverse )
    girar();

  return (true); // TRUE->sigue  ; FALSE->para
}

void Ventana::obstaculo_movil() {
  cout << "\n AÚN NO ESTÁ IMPLEMENTADO" << endl;
}

void Ventana::ctrl_reiniciar() {

  cout << "\n reiniciando contadores..."<<endl;
  angulo = 0;
  num_aciertos = 0;
  num_disparos = 0;
  num_objetivos= 0;
  moverse = false;

  cout << " eliminando disparos..."<<endl;
  disparos.clear();

  cout << " eliminando objetivos..."<<endl;
  objetivos.clear();

  actualizar_status();
  m_area.redibujar();
}


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

void texto_comunicacion() {
  int rand_radio=0;

  // añadir algún objetivo de cuando en cuando SOBRE la línea del radar
  double r = (double) (100.0*rand()/(RAND_MAX+1.0));
  if( r < varDENSIDAD_OBJETIVOS ) {

    double tmp_ang = (double)( (double)angulo * (double)DEG2RAD );
    rand_radio = 5+(int) (190.0*rand()/(RAND_MAX+1.0));
    unsigned x = (unsigned) (def_ANCHO_ARENA/2) + (int) (rand_radio * cos(tmp_ang));
    unsigned y = (unsigned) (def_ALTO_ARENA/2)  - (int) (rand_radio * sin(tmp_ang));

    objeto o;
    o.x = x;
    o.y = y;
    o.tiempo = varDURACION_OBJETIVO ;
    o.alcanzado = 0;
    objetivos.push_back(o);

    num_objetivos++;
  }


  unsigned a1=0,a2=0;
  
  // abrir para machacar el fichero de S    
  ofstream fo;
  fo.open( "S" , ios::trunc);
  fo << angulo << " " << rand_radio << endl;
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

      // si habia algo que hacer => praparar para pintar la acción, sino, nada
      if( a1 && a2 ) {
	objeto c;
	c.x = a1;
	c.y = a2;
	c.tiempo = varDURACION_DISPARO;
	disparos.push_back(c);
	
	num_disparos++;
      }

    }
  }

  // si el último disparo da en un blanco, decirlo y cambiar el tiempo
  for(unsigned j=0; j<objetivos.size(); j++) {
    if( ( std::abs((int)(objetivos[j].x - a1)) < 20 )&&( std::abs((int)(objetivos[j].y - a2)) < 20 ) ) {
      objetivos[j].alcanzado = 1;
      num_aciertos ++;
      break;
    }
  }  

}

void leer_fichero_conf() {
  string nombre_fich = "radar.conf";
  string tmp;

  ifstream f;
  f.open( nombre_fich.c_str() );

  f >> tmp >> incrAngulo;

  f >> tmp >> varDURACION_OBJETIVO;
  f >> tmp >> varDURACION_DISPARO;

  f >> tmp >> varVELOCIDAD;

  f >> tmp >> varDENSIDAD_OBJETIVOS;

  bool b;
  f >> tmp >> b;
  modoGrafico = (b==1)?true:false;

  f.close();
}


//*****************************************************************************

int main(int argc, char** argv) {
  srand( time(0) );

  cout << "\n(c)2001 - Pedro Angel Castillo Valdivieso"<<endl;
  //cout << "\nOPCIONES: "<<argv[0]<<"  [fich_escenario] "<<endl;

  leer_fichero_conf();

  if(modoGrafico) {
    cout << "\n\tejecutando en modo gráfico..."<<endl;
    Gtk::Main main_instance (argc, argv);

    Ventana ventana;
    Gtk::Main::run( ventana );
  }else{
    cout << "\n\t MODO TEXTO NO IMPLEMENTADO AUN..."<<endl;
  }

  return 0;
}
