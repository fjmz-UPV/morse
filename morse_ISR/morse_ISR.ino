


#define MAX_COLA 20

class Cola_char {
  private:
    char cola[MAX_COLA];
    int ptr_in  = 0;
    int ptr_out = 0;
    bool vacia = true;
    bool llena = false;
  public:
    Cola(void);
    bool encolar(char c);
    bool desencolar(char *c);
    bool estaVacia(void);
    bool estaLlena(void);
};


Cola_char::Cola() {

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
    Cola(void);
    bool encolar(int c);
    bool desencolar(int *c);
    bool estaVacia(void);
    bool estaLlena(void);
};


Cola_int::Cola() {

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



#define DEBUG 

#define T_REBOTE 5

#include <TimerOne.h>





#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

int cursorF = 0;
int cursorC = 0;

#define MAX_PUNTOS_RAYAS 8
#define COL_ANCHURA_PULSO (MAX_PUNTOS_RAYAS+1)
#define COL_WPM (COL_ANCHURA_PULSO+5)


const int intPin2 = 2;
const int ledPin = 13;
const int buzzer = 4;



const int UMBRAL_INTERLETRAS   = 3; //2;
const int UMBRAL_INTERPALABRAS = 7; //5;

float wpm_inic = 15;
int tdi_ms;
int t_th_di_da_ms;
long t_timer_us;


const char PUNTO = '.';
const char RAYA = '-';
const char ESPACIO = ' ';



volatile char simbolo;
volatile long anchuraPulso = 0;
volatile long anchuraSilencio = 0;


int wpm_media = 0;

char arrayPuntosRayas[MAX_PUNTOS_RAYAS+1];
int contPR = 0;


volatile int n_Timer1Start = 0;



const char BLANCO = ' ';
const char BLANCO2 = ' ';
const char OTRO=' ';


char linea0[16]={BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, OTRO};
char linea1[16]={BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, OTRO};

const int PRIMERA = 0;
int cursorC_PR = PRIMERA;
int cursorC_letras = 0;

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

void setup(){

  pinMode(intPin2, INPUT_PULLUP);
  pinMode(ledPin,  OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(115200);

  tdi_ms        = round( 60.f/(50*wpm_inic ) *1000 );
  t_th_di_da_ms = 2 * tdi_ms;
  t_timer_us    = tdi_ms*1000L;

  //////////////Timer1.initialize(t_timer_us);
  ////////////////////Timer1.attachInterrupt(timeOut);
  ///////////////////Timer1.stop();
  


  #ifdef DEBUG
  Serial.print("\nwpm_inic: ");
  Serial.println(wpm_inic);

  Serial.print("tdi_ms: ");
  Serial.println(tdi_ms);

  Serial.print("t_timer_us: ");
  Serial.println(t_timer_us);
  #endif




  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("PacoSoft CW dec.");
  lcd.setCursor(0,1);
  lcd.print(wpm_inic); lcd.print(' '); lcd.print(tdi_ms); lcd.print(' '); lcd.print(t_th_di_da_ms); 


  

  delay(4000);

  lcd.clear();

  
  attachInterrupt(digitalPinToInterrupt(intPin2), doChange,  CHANGE);

/*
  lcd.setCursor(0,0);
  for (int i=0; i<16; i++) {
    lcd.print(linea0[i]);
  }

  lcd.setCursor(0,1);
  for (int i=0; i<16; i++) {
    lcd.print(linea1[i]);
  }
 */

}


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




void acumularPuntoRaya(char puntoRaya) {
  Serial.print(puntoRaya);
  if (contPR==0) {
    lcd.setCursor(0,0);
    for (int i=0; i<MAX_PUNTOS_RAYAS; i++) {
      lcd.print(' ');
      arrayPuntosRayas[i] = '\0';
    }
  }
  arrayPuntosRayas[contPR] = puntoRaya;
  lcd.setCursor(contPR, 0);
  lcd.print(puntoRaya);
  contPR++;
  if (contPR==MAX_PUNTOS_RAYAS) contPR = 0;
}

void printLetra(char car) {
  lcd.setCursor(0,1);
  for (int i=0; i<15; i++) {
    linea1[i] = linea1[i+1];
    lcd.print(linea1[i]);
    Serial.print(linea1[i]);
  }
  linea1[15] = car;
  lcd.print(linea1[15]);
  Serial.print(linea1[15]);
  contPR = 0;
}

void printAnchuraPulso(int anchura) {
  lcd.setCursor(COL_ANCHURA_PULSO, 0);
  lcd.print("    ");
  lcd.setCursor(COL_ANCHURA_PULSO, 0);
  lcd.print(anchura);
}





void printWpm(int wpm) {
  //Serial.print("("); Serial.print(wpm_media); Serial.print(" wpm)");
  lcd.setCursor(COL_WPM, 0);
  lcd.print("    ");
  lcd.setCursor(COL_WPM, 0);
  lcd.print(wpm);
}




void loopxx() {

  char evento;
  char simbolo;
  int anchura;
  int wpm;

  noInterrupts();
    bool hayEvento = colaEventos.desencolar(&evento);
  interrupts();
    
  if (hayEvento) {
    if (evento=='S') {
      noInterrupts();
      colaPR.desencolar(&simbolo);
      interrupts();
      Serial.print("\nsimbolo leido: "); Serial.println(simbolo);
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


void doChange() {

  flanco(digitalRead(intPin2));

/*
  if (digitalRead(intPin2) == LOW) doFalling();
  else                            doRising();
  */
}



volatile bool llena = false;
volatile bool vacia = true;
volatile long cola_tiempo[MAX_COLA];
volatile int  cola_flanco[MAX_COLA];
volatile int ptr_in  = 0; 
volatile int ptr_out = 0;

void flanco(int nivel) {
  if (llena) return;
  cola_tiempo[ptr_in] = millis();
  cola_flanco[ptr_in] = nivel;
  ptr_in++;
  if (ptr_in == MAX_COLA) ptr_in = 0;
  if (ptr_in == ptr_out)  llena  = true;
  vacia = false;
}



#define FLANCO_BAJADA 0
#define FLANCO_SUBIDA 1

#define ESTADO_INICIAL 0
#define NIVEL_BAJO 1
#define NIVEL_ALTO 2

int estado = ESTADO_INICIAL;



void flancoBajada(long tiempo) {
  t_flanco_bajada = tiempo;
  anchuraSilencio = t_flanco_bajada - t_flanco_subida;
  //Timer1.stop();
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzer, HIGH);
  if (!palabra_iniciada) {
    palabra_iniciada = true;
    t_inic_palabra = t_flanco_bajada;
  }
}


void flancoSubida(long tiempo) {
    t_flanco_subida = tiempo;
    anchuraPulso = t_flanco_subida - t_flanco_bajada;
    tiempo_de_pulsos += anchuraPulso;
    n_Timer1Start = 0;
    //Timer1.start();
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzer, LOW);
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

void loop() {

  bool hayFlanco = false;
  long int tiempo;
  int flanco;

  noInterrupts();
    if (!vacia) {
      hayFlanco = true;
      tiempo = cola_tiempo[ptr_out];
      flanco = cola_flanco[ptr_out];
      ptr_out++;
      if (ptr_out==MAX_COLA) ptr_out = 0;
      if (ptr_in == ptr_out) vacia = true;
      llena = false;    
    }
  interrupts();

  if (hayFlanco) {
    Serial.print("flanco: "); Serial.print(flanco); 
    Serial.print(", tiempo: "); Serial.println(tiempo);

    if (flanco == FLANCO_BAJADA) {
      switch(estado) {
        ESTADO_INICIAL:
          flancoBajada(tiempo);
          estado = NIVEL_BAJO;
          break;
        NIVEL_BAJO:
          break;
        NIVEL_ALTO:
          if ( tiempo - t_flanco_subida > T_REBOTE ) {
            flancoBajada(tiempo);
            estado = NIVEL_BAJO;
          }
          break;
      } // case
    } else if (flanco == FLANCO_SUBIDA) {
      switch (estado) {
        case ESTADO_INICIAL:
          flancoSubida(tiempo);
          estado = NIVEL_ALTO;
          break;
        case NIVEL_BAJO: 
          if ( tiempo - t_flanco_bajada > T_REBOTE ) {
            flancoSubida(tiempo);      
            estado = NIVEL_ALTO;
          }
          break;
        case NIVEL_ALTO:
          break;
      } // case
    } // else



  }

}







long t_flanco_bajada = 0;
long t_flanco_subida = 0;

#define INICIAL    0
#define NIVEL_BAJO 1
#define NIVEL_ALTO 2

int estado = INICIAL;


void procesar_flanco_bajada() {
  t_flanco_bajada = millis();
  anchuraSilencio = t_flanco_bajada - t_flanco_subida;
  estado = NIVEL_BAJO;
  Timer1.stop();
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzer, HIGH);
  if (!palabra_iniciada) {
    palabra_iniciada = true;
    t_inic_palabra = t_flanco_bajada;
  }
}

void doFalling() {
  switch (estado) {

    case INICIAL:
      procesar_flanco_bajada();
      break;

    case NIVEL_BAJO:
      break;

    case NIVEL_ALTO:
      if ( millis() - t_flanco_subida> T_REBOTE ) {
        procesar_flanco_bajada();
      }
      break;
  }
}




void procesar_flanco_subida() {
    t_flanco_subida = millis();
    anchuraPulso = t_flanco_subida - t_flanco_bajada;
    estado = NIVEL_ALTO;
    tiempo_de_pulsos += anchuraPulso;
    n_Timer1Start = 0;
    Timer1.start();
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzer, LOW);
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

  switch (estado) {

    case INICIAL:
      procesar_flanco_subida();
      break;

    case NIVEL_BAJO: 
      if ( millis() - t_flanco_bajada > T_REBOTE ) {
        procesar_flanco_subida();      
      }
      break;
    case NIVEL_ALTO:
      break;
    }

}







void timeOut() {
  switch(n_Timer1Start) {
    case UMBRAL_INTERLETRAS: 

            //nueva_letra   = true; 

/*
            tiempo_de_pulsos_copia = tiempo_de_pulsos;
            n_puntos_copia         = n_puntos;
            n_rayas_copia          = n_rayas;
*/
            n_puntos = 0;
            n_rayas  = 0;
            tiempo_de_pulsos = 0;
            n_letras++;

            colaEventos.encolar('L');

           
            break;

    case UMBRAL_INTERPALABRAS: 
            //nueva_palabra = true; 

            n_puntos = 0;
            n_rayas  = 0;
            tiempo_de_pulsos = 0;
            
            Timer1.stop(); 

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


