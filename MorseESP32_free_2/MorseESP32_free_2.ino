// Pantalla 320 x240 px 

//#include <M5Core2.h>
#include <M5Unified.h>

#include <utility/M5Timer.h>

M5Timer M5T;
int nTimer;


volatile bool problema=false;


/***************************** COLAS ********************************/
#define MAX_COLA 100

class Cola_char {
  private:
    char cola[MAX_COLA];
    int ptr_in  = 0;
    int ptr_out = 0;
    bool vacia = true;
    bool llena = false;
  public:
    Cola_char();
    bool encolar(char c);
    bool desencolar(char *c);
    bool estaVacia(void);
    bool estaLlena(void);
};


Cola_char::Cola_char() {

}

bool Cola_char::estaVacia(void) {
  return vacia;
}

bool Cola_char::estaLlena(void) {
  return llena;
}

bool Cola_char::encolar(char c) {
  if (llena) return false;
  cola[ptr_in] = c;
  ptr_in++;
  if (ptr_in==MAX_COLA) ptr_in = 0;
  if (ptr_in==ptr_out) llena = true;
  vacia = false;
  return true;
}

bool Cola_char::desencolar(char *c) {
  if (vacia) return false;
  *c = cola[ptr_out];
  ptr_out++;
  if (ptr_out==MAX_COLA) ptr_out = 0;
  if (ptr_in == ptr_out) vacia = true;
  llena = false;
  return true;
}



class Cola_int {
  private:
    int cola[MAX_COLA];
    int ptr_in  = 0;
    int ptr_out = 0;
    bool vacia = true;
    bool llena = false;
  public:
    Cola_int();
    bool encolar(int c);
    bool desencolar(int *c);
    bool estaVacia(void);
    bool estaLlena(void);
};


Cola_int::Cola_int() {

}

bool Cola_int::estaVacia(void) {
  return vacia;
}

bool Cola_int::estaLlena(void) {
  return llena;
}

bool Cola_int::encolar(int c) {
  if (llena) return false;
  cola[ptr_in] = c;
  ptr_in++;
  if (ptr_in==MAX_COLA) ptr_in = 0;
  if (ptr_in==ptr_out) llena = true;
  vacia = false;
  return true;
}

bool Cola_int::desencolar(int *c) {
  if (vacia) return false;
  *c = cola[ptr_out];
  ptr_out++;
  if (ptr_out==MAX_COLA) ptr_out = 0;
  if (ptr_in == ptr_out) vacia = true;
  llena = false;
  return true;
}




/////////////


class Cola_eventos {
  private:
     String evts[MAX_COLA];
    volatile long tiempos[MAX_COLA];
    volatile int ptr_in  = 0;
    volatile int ptr_out = 0;
    volatile bool vacia = true;
    volatile bool llena = false;
  public:
    Cola_eventos();
    bool encolar(String tipo, long tiempo);
    bool desencolar(String *tipo, long *tiempo);
    bool estaVacia(void);
    bool estaLlena(void);
};


Cola_eventos::Cola_eventos() {

}

bool Cola_eventos::estaVacia(void) {
  return vacia;
}

bool Cola_eventos::estaLlena(void) {
  return llena;
}

bool Cola_eventos::encolar(String tipo, long tiempo) {
  if (llena) return false;

  if (tiempo==0) problema=true;

  evts[ptr_in] = tipo;
  tiempos[ptr_in] = tiempo;
  ptr_in++;
  if (ptr_in==MAX_COLA) ptr_in = 0;
  if (ptr_in==ptr_out) llena = true;
  vacia = false;
  return true;
}

bool Cola_eventos::desencolar(String *tipo, long *tiempo) {
  if (vacia) return false;
  *tipo = evts[ptr_out];
  *tiempo = tiempos[ptr_out];
  ptr_out++;
  if (ptr_out==MAX_COLA) ptr_out = 0;
  if (ptr_in == ptr_out) vacia = true;
  llena = false;
  return true;
}





/***********************************************************/


#define T_REBOTE 10


const int intPin = 33;

const int UMBRAL_INTERLETRAS   = 3; //2;
const int UMBRAL_INTERPALABRAS = 7; //5;

float wpm_inic = 15;
int tdi_ms;
int t_th_di_da_ms;
long t_timer_us;
int t_timer_ms;


const char PUNTO   = '.';
const char RAYA    = '-';
const char ESPACIO = ' ';


char linea1[16];


volatile long anchuraPulso = 0;
volatile long anchuraSilencio = 0;


int wpm_media = 0;



#define MAX_PUNTOS_RAYAS 10


#define COL_ANCHURA_PULSO 0
#define FILA_ANCHURA_PULSO 50

#define COL_WPM 0
#define FILA_WPM 100

#define COL_PUNTOS_RAYAS  0 
#define FILA_PUNTOS_RAYAS 150

#define COL_TEXTO 0 
#define FILA_TEXTO 200


char arrayPuntosRayas[MAX_PUNTOS_RAYAS+1];
int contPR = 0;

volatile int n_Timer1Start = 1;


volatile int n_dits = 0;
volatile int tiempo_de_pulsos = 0;
volatile int n_puntos = 0;
volatile int n_rayas = 0;
volatile int n_letras = 0;

volatile long t_inic_palabra = 0;
bool palabra_iniciada = false;

Cola_char colaEventos;
Cola_char colaPR;
Cola_int colaAnchuras;
Cola_int colaWpm;
Cola_eventos ce;

/****************** SETUP ******************/
void setup() {
    M5.begin();  
    M5.Lcd.setTextSize(2);       
    print("PacoSoft CW Decoder");  

    pinMode(intPin, INPUT_PULLUP);

    tdi_ms        = round( 60.f/(50*wpm_inic ) *1000 );
    t_th_di_da_ms = 2 * tdi_ms;
    t_timer_us    = tdi_ms*1000L;
    t_timer_ms    = tdi_ms;

    Serial.begin(115200);

    Serial.println("Hola");

    Serial.print("\nwpm_inic: ");
    Serial.println(wpm_inic);

    Serial.print("tdi_ms: ");
    Serial.println(tdi_ms);

    Serial.print("umbral: ");
    Serial.println(t_th_di_da_ms);

    Serial.print("t_timer_us: ");
    Serial.println(t_timer_us);

    M5.Speaker.begin();
    M5.Speaker.setVolume(32);

    attachInterrupt(digitalPinToInterrupt(intPin), doChange,  CHANGE);

}





/********************* GESTION DEL MANIPULADOR *********************/

void doChange() {
  if (digitalRead(intPin) == LOW) doFalling();
  else                             doRising();
}


long t_flanco_bajada = 0;
long t_flanco_subida = 0;

#define INICIAL    0
#define NIVEL_BAJO 1
#define NIVEL_ALTO 2

volatile int estado = INICIAL;


void procesar_flanco_bajada() {
  t_flanco_bajada = millis();
  anchuraSilencio = t_flanco_bajada - t_flanco_subida;
  estado = NIVEL_BAJO;

  if (M5T.isEnabled(nTimer)) {
    M5T.deleteTimer(nTimer);
  }

  if (!palabra_iniciada) {
    palabra_iniciada = true;
    t_inic_palabra = t_flanco_bajada;
  }
}




void doFalling() {

  ce.encolar("Bajada", millis());
  switch (estado) {

    case INICIAL:
      procesar_flanco_bajada();
      break;

    case NIVEL_BAJO:
      break;

    case NIVEL_ALTO:
      if ( millis() - t_flanco_subida> T_REBOTE ) {
        procesar_flanco_bajada();
        colaEventos.encolar('y');
      } else {
        colaEventos.encolar('f');
      }
      break;
  }
}




void procesar_flanco_subida() {
    char simbolo;
    t_flanco_subida = millis();
    anchuraPulso = t_flanco_subida - t_flanco_bajada;
    estado = NIVEL_ALTO;
    tiempo_de_pulsos += anchuraPulso;

    if (M5T.isEnabled(nTimer)) {
      M5T.deleteTimer(nTimer);
    }
    nTimer = M5T.setTimer(t_timer_ms, timeOut, 10);

    n_Timer1Start = 1;

    if (anchuraPulso < t_th_di_da_ms) {
      simbolo = PUNTO;
      n_puntos++;
      n_dits += 2;
    } else {
      simbolo = RAYA;
      n_rayas++;
      n_dits += 4;
    }
    colaEventos.encolar('S');
    colaPR.encolar(simbolo); 
    colaAnchuras.encolar(anchuraPulso);
}




void doRising() {
  ce.encolar("Subida", millis());

  switch (estado) {

    case INICIAL:
      procesar_flanco_subida();
      break;

    case NIVEL_BAJO: 
      if ( millis() - t_flanco_bajada > T_REBOTE ) {
        procesar_flanco_subida();      
        colaEventos.encolar('x');
      } else {
        colaEventos.encolar('r');
      }
      break;
    case NIVEL_ALTO:
      break;
    }

}







void timeOut() {
  switch(n_Timer1Start) {
    case UMBRAL_INTERLETRAS: 

            n_puntos = 0;
            n_rayas  = 0;
            tiempo_de_pulsos = 0;
            n_letras++;

            colaEventos.encolar('L');

            break;

    case UMBRAL_INTERPALABRAS: 

            n_puntos = 0;
            n_rayas  = 0;
            tiempo_de_pulsos = 0;
            
            M5T.deleteTimer(nTimer);

            colaEventos.encolar('P');

            n_dits += 3;

            long t_palabra = millis() - t_inic_palabra;

            //int t_dit = t_palabra / n_dits;
            int wpm = (int) (60000.*n_dits/(50.*t_palabra));
            colaWpm.encolar(wpm);

            n_dits = 0;
            palabra_iniciada = false;

            break;
  }
  n_Timer1Start++;
}






/************** DECODIFICAION *********************/

char caracter(char arrayPuntosRayas[]) {
  if (String(arrayPuntosRayas)==".")       return 'E';
  else if (String(arrayPuntosRayas)=="-")  return 'T';

  else if (String(arrayPuntosRayas)==".-") return 'A';
  else if (String(arrayPuntosRayas)=="..") return 'I';
  else if (String(arrayPuntosRayas)=="-.") return 'N';
  else if (String(arrayPuntosRayas)=="--") return 'M';

  else if (String(arrayPuntosRayas)=="...") return 'S';
  else if (String(arrayPuntosRayas)=="..-") return 'U';
  else if (String(arrayPuntosRayas)==".-.") return 'R';
  else if (String(arrayPuntosRayas)==".--") return 'W';
  else if (String(arrayPuntosRayas)=="-..") return 'D';
  else if (String(arrayPuntosRayas)=="-.-") return 'K';
  else if (String(arrayPuntosRayas)=="--.") return 'G';
  else if (String(arrayPuntosRayas)=="---") return 'O';

  else if (String(arrayPuntosRayas)=="....") return 'H';
  else if (String(arrayPuntosRayas)=="...-") return 'V';
  else if (String(arrayPuntosRayas)=="..-.") return 'F';
  //else if (String(arrayPuntosRayas)=="..--") return 'S';
  else if (String(arrayPuntosRayas)==".-..") return 'L';
  //else if (String(arrayPuntosRayas)==".-.-") return 'S';
  else if (String(arrayPuntosRayas)==".--.") return 'P';
  else if (String(arrayPuntosRayas)==".---") return 'J';
  else if (String(arrayPuntosRayas)=="-...") return 'B';
  else if (String(arrayPuntosRayas)=="-..-") return 'X';
  else if (String(arrayPuntosRayas)=="-.-.") return 'C';
  else if (String(arrayPuntosRayas)=="-.--") return 'Y';
  else if (String(arrayPuntosRayas)=="--..") return 'Z';
  else if (String(arrayPuntosRayas)=="--.-") return 'Q';
  //else if (String(arrayPuntosRayas)=="---.") return 'S';
  //else if (String(arrayPuntosRayas)=="----") return 'S';

  else if (String(arrayPuntosRayas)==".----") return '1';
  else if (String(arrayPuntosRayas)=="..---") return '2';
  else if (String(arrayPuntosRayas)=="...--") return '3';
  else if (String(arrayPuntosRayas)=="....-") return '4';
  else if (String(arrayPuntosRayas)==".....") return '5';
  else if (String(arrayPuntosRayas)=="-....") return '6';
  else if (String(arrayPuntosRayas)=="--...") return '7';
  else if (String(arrayPuntosRayas)=="---..") return '8';
  else if (String(arrayPuntosRayas)=="----.") return '9';
  else if (String(arrayPuntosRayas)=="-----") return '0';

  else if (String(arrayPuntosRayas)==".-.-.-") return '.';
  else if (String(arrayPuntosRayas)=="--..--") return ',';
  else if (String(arrayPuntosRayas)=="..--..") return '?';
  else if (String(arrayPuntosRayas)=="-.-.--") return '!';
  else if (String(arrayPuntosRayas)==".----.") return '\'';
  else if (String(arrayPuntosRayas)==".-..-.") return '"';
  else if (String(arrayPuntosRayas)=="-.--.") return '(';
  else if (String(arrayPuntosRayas)=="-.--.-") return ')';
  else if (String(arrayPuntosRayas)==".-...") return '&';
  else if (String(arrayPuntosRayas)=="---...") return ':';
  else if (String(arrayPuntosRayas)=="-.-.-.") return ';';
  else if (String(arrayPuntosRayas)=="-..-.") return '/';
  else if (String(arrayPuntosRayas)=="..--.-") return '_';
  else if (String(arrayPuntosRayas)=="-...-") return '=';
  else if (String(arrayPuntosRayas)==".-.-.") return '+';
  else if (String(arrayPuntosRayas)=="-....-") return '-';
  else if (String(arrayPuntosRayas)=="...-..-") return '$';
  else if (String(arrayPuntosRayas)==".--.-.") return '@';

  else return '*';
}


void marcaEspacio(bool marca) {
  if (marca) {
    M5.Lcd.fillCircle(310, 10, 5, YELLOW);
  } else {
    M5.Lcd.fillCircle(310, 10, 5, BLACK);
  }
}


#define PASO 20

void acumularPuntoRaya(char puntoRaya) {
  //Serial.print(puntoRaya);
  if (contPR==0) {
    for (int i=0; i<MAX_PUNTOS_RAYAS; i++) {
      gotoXY(COL_PUNTOS_RAYAS+i*PASO, FILA_PUNTOS_RAYAS);
      print(' ');
      arrayPuntosRayas[i] = '\0';
    }
  }
  arrayPuntosRayas[contPR] = puntoRaya;
  gotoXY(COL_PUNTOS_RAYAS+contPR*PASO, FILA_PUNTOS_RAYAS);
  print(puntoRaya);
  contPR++;
  if (contPR==MAX_PUNTOS_RAYAS) contPR = 0;
}

void printLetra(char car) {
  gotoXY(COL_TEXTO, FILA_TEXTO);
  for (int i=0; i<15; i++) {
    linea1[i] = linea1[i+1];
    print(linea1[i]);
    //Serial.print(linea1[i]);
  }
  linea1[15] = car;
  print(linea1[15]);
  //Serial.print(linea1[15]);
  contPR = 0;
}

void printAnchuraPulso(int anchura) {
  gotoXY(COL_ANCHURA_PULSO, FILA_ANCHURA_PULSO);
  print("    ");
  gotoXY(COL_ANCHURA_PULSO, FILA_ANCHURA_PULSO);
  print(anchura);
}





void printWpm(int wpm) {
  gotoXY(COL_WPM, FILA_WPM);
  print("    ");
  gotoXY(COL_WPM, FILA_WPM);
  print(wpm);
}




void gotoXY(int x, int y) {
  //Serial.print("Goto XY: "); Serial.print(x); Serial.print(", "); Serial.println(y);
  M5.Lcd.setCursor(x, y);
}

void print(String x) {
  //Serial.print(x);
  M5.Lcd.print(x);
}

void print(char x) {
  //Serial.print(x);
  M5.Lcd.print(x);
}

void print(int x) {
  //Serial.print(x);
  M5.Lcd.print(x);
}


void println(String x) {
  //Serial.println(x);
  M5.Lcd.println(x);
}

void println(char x) {
  //Serial.println(x);
  M5.Lcd.println(x);
}

void println(int x) {
  //Serial.println(x);
  M5.Lcd.println(x);
}


volatile long tiempo_ant=0;
long vez =0;

void loop() {

  char evento;
  char simbolo;
  int anchura;
  int wpm;
  int mi_estado;
  bool tela = false;

  String tipo, mi_tipo;
  long tiempo=0;
  volatile long mi_tiempo;
  volatile long tiempo_ant_aux;
  M5.update();

  M5T.run();

  vez++;

  bool problema_aux = false;

  noInterrupts();

    tiempo_ant_aux = tiempo_ant;
    bool hayEvento = ce.desencolar(&tipo, &tiempo);
    mi_estado = estado;
    mi_tipo = tipo;
    mi_tiempo = tiempo;
    tiempo_ant = tiempo;
    problema_aux = problema;
    if (problema) problema = false;

  interrupts();
    
  if (hayEvento) {
    Serial.print(mi_tiempo-tiempo_ant_aux); Serial.print("  ");Serial.print(tiempo_ant_aux); Serial.print("      ");Serial.print(mi_tiempo); Serial.print(". Tipo: "); Serial.print(tipo); 
    Serial.print(". vez: "); Serial.print(vez);
    Serial.println();

    if (problema_aux) {
      Serial.println(".....................................................");
      problema_aux= false;
    }
  }



}



void loop2() {

  char evento;
  char simbolo;
  int anchura;
  int wpm;
  int mi_estado;

  M5.update();

  M5T.run();

  noInterrupts();
    bool hayEvento = colaEventos.desencolar(&evento);
    mi_estado = estado;
  interrupts();
    
  if (mi_estado == NIVEL_BAJO) {
    //Serial.println("NIVEL_BAJO");
    marcaEspacio(true);
    M5.Speaker.tone(1000, 1000);
  } else {
    //Serial.println("NIVEL_ALTO");
    marcaEspacio(false);
    M5.Speaker.stop();
  }

  if (hayEvento) {
    Serial.print("Evento: "); Serial.println(evento);
    if (evento=='S') {
      noInterrupts();
        colaPR.desencolar(&simbolo);
      interrupts();
      Serial.print("simbolo leido: "); Serial.println(simbolo);
      acumularPuntoRaya(simbolo);
      colaAnchuras.desencolar(&anchura);
      printAnchuraPulso(anchura);
    } else if (evento=='L') {
      printLetra(caracter(arrayPuntosRayas));
    } else if (evento=='P') {
      printLetra(ESPACIO);
      colaWpm.desencolar(&wpm);
      printWpm(wpm);
    }

  }

}
