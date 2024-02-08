// Pantalla 320 x240 px 


#include <M5CoreS3.h>
#include <utility/M5Timer.h>

M5Timer M5T;
int nTimer;


/***************************** COLAS ********************************/
#define MAX_COLA 20

class Cola_eventos {
  private:
    char evts[MAX_COLA];
    long tiempos[MAX_COLA];
    int ptr_in  = 0;
    int ptr_out = 0;
    bool vacia = true;
    bool llena = false;
  public:
    Cola_eventos();
    bool encolar(char tipo, long tiempo);
    bool desencolar(char *tipo, long *tiempo);
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

bool Cola_eventos::encolar(char tipo, long tiempo) {
  if (llena) return false;
  evts[ptr_in]    = tipo;
  tiempos[ptr_in] = tiempo;
  ptr_in++;
  if (ptr_in==MAX_COLA) ptr_in = 0;
  if (ptr_in==ptr_out) llena = true;
  vacia = false;
  return true;
}

bool Cola_eventos::desencolar(char *tipo, long *tiempo) {
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

const int intPin = 1; 

Cola_eventos ce;




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






/****************** SETUP ******************/
void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);

  M5.begin();  
  M5.Lcd.setTextSize(2);       
  print("PacoSoft CW Decoder 2");  


  pinMode(intPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(intPin), doChange,  CHANGE);
}


/********************* GESTION DEL MANIPULADOR *********************/

void doChange() {
  if (digitalRead(intPin) == LOW) doFalling();
  else                             doRising();
}


void doFalling() {
  ce.encolar('B', micros());
}


void doRising() {
  ce.encolar('S', micros());
}


/************************* LOOP ****************************/


volatile long tiempo_ant = 0;

void loop() {

  char tipo;
  long tiempo;
  long tiempo_ant_aux;
  bool hayEvento;

  noInterrupts();
    tiempo_ant_aux = tiempo_ant;
    hayEvento = ce.desencolar(&tipo, &tiempo);
    tiempo_ant = tiempo;
  interrupts();

  hayEvento = true;
    
  if (hayEvento) {
    gotoXY(0, 20);
    print(tipo); print(": ");
    print(tiempo); print(" // ");print(tiempo_ant_aux); print(" // ");print(tiempo-tiempo_ant_aux); 
    println("");
  }



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


void print(long x) {
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


void println(long x) {
  //Serial.print(x);
  M5.Lcd.println(x);
}




