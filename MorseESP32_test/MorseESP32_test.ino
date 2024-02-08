// Pantalla 320 x240 px 

//#include <M5Core2.h>
#include <M5Unified.h>




/***************************** COLAS ********************************/
#define MAX_COLA 100

class Cola_eventos {
  private:
    String evts[MAX_COLA];
    long tiempos[MAX_COLA];
    int ptr_in  = 0;
    int ptr_out = 0;
    bool vacia = true;
    bool llena = false;
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
  evts[ptr_in]    = tipo;
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



const int intPin = 33;


Cola_eventos ce;

/****************** SETUP ******************/
void setup() {
    M5.begin();  
    M5.Lcd.setTextSize(2);       
    print("PacoSoft CW Decoder");  
    pinMode(intPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(intPin), doChange,  CHANGE);
}


/********************* GESTION DEL MANIPULADOR *********************/

void doChange() {
  if (digitalRead(intPin) == LOW) doFalling();
  else                             doRising();
}


void doFalling() {
  ce.encolar("Bajada", micros());
}


void doRising() {
  ce.encolar("Subida", micros());
}


/************************* LOOP ****************************/


volatile long tiempo_ant = 0;

void loop() {

  String tipo;
  long tiempo;
  long tiempo_ant_aux;
  bool hayEvento;

  M5.update();
  noInterrupts();
    tiempo_ant_aux = tiempo_ant;
    hayEvento = ce.desencolar(&tipo, &tiempo);
    tiempo_ant = tiempo;
  interrupts();
    
  if (hayEvento) {
    Serial.print(tipo); Serial.print(": ");
    Serial.print(tiempo); Serial.print(" // ");Serial.print(tiempo_ant_aux); Serial.print(" // ");Serial.print(tiempo-tiempo_ant_aux); 
    Serial.println();
  }



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



