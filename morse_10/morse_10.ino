


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





#define DEBUG 

#define T_REBOTE 10

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




const int intPin2 = 2;
const int ledPin = 13;



const int UMBRAL_INTERLETRAS   = 2;
const int UMBRAL_INTERPALABRAS = 5;

float wpm_inic = 15;
int tdi_ms;
int t_th_di_da_ms;
long t_timer_us;


const char PUNTO = '.';
const char RAYA = '-';
const char ESPACIO = '_';



volatile char simbolo;
volatile long startPulso = 0;
volatile long anchuraPulso = 0;
volatile bool flancoBajada = false;

int wpm_media = 0;

char arrayPuntosRayas[MAX_PUNTOS_RAYAS+1];
int contPR = 0;


volatile int n_Timer1Start = 0;

volatile bool nuevo_simbolo = false;
volatile bool nueva_letra   = false;
volatile bool nueva_palabra = false;

const char BLANCO = ' ';
const char BLANCO2 = ' ';
const char OTRO=' ';


char linea0[16]={BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, BLANCO, OTRO};
char linea1[16]={BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, BLANCO2, OTRO};

const int PRIMERA = 0;
int cursorC_PR = PRIMERA;
int cursorC_letras = 0;

volatile int tiempo_de_pulsos = 0;
volatile int tiempo_de_pulsos_copia = 0;
volatile int n_puntos = 0;
volatile int n_rayas = 0;
volatile int n_puntos_copia = 0;
volatile int n_rayas_copia = 0;


Cola_char colaEventos;
Cola_char colaPR;

void setup(){

  pinMode(intPin2, INPUT_PULLUP);
  pinMode(ledPin,  OUTPUT);

  Serial.begin(115200);

  tdi_ms        = round( 60.f/(50*wpm_inic ) *1000 );
  t_th_di_da_ms = 2 * tdi_ms;
  t_timer_us    = tdi_ms*1000L;

  Timer1.initialize(t_timer_us);
  Timer1.attachInterrupt(timeOut);
  Timer1.stop();
  

  attachInterrupt(digitalPinToInterrupt(intPin2), doChange,  CHANGE);

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
  lcd.print("PacSoft CW dec.");
  lcd.setCursor(0,1);
  lcd.print(wpm_inic); lcd.print(' '); lcd.print(tdi_ms); lcd.print(' '); lcd.print(t_th_di_da_ms); 
 // lcd.print(" t_th: "); lcd.print(t_th_di_da_ms);


  

  delay(4000);

  lcd.setCursor(0,0);
  for (int i=0; i<16; i++) {
    lcd.print(linea0[i]);
  }

  lcd.setCursor(0,1);
  for (int i=0; i<16; i++) {
    lcd.print(linea1[i]);
  }
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
  lcd.setCursor(MAX_PUNTOS_RAYAS+1, 0);
  lcd.print("    ");
  lcd.setCursor(MAX_PUNTOS_RAYAS+1, 0);
  lcd.print(anchura);
}





void printWpm() {
  //Serial.print("("); Serial.print(wpm_media); Serial.print(" wpm)");
}




void loop() {

  char simbolo_aux = '\0';
  bool nuevo_simbolo_aux = false;
  bool nueva_letra_aux   = false;
  bool nueva_palabra_aux = false;
  int  anchuraPulso_aux  = 0;
  int tiempo_de_pulsos_aux;
  int n_puntos_aux;
  int n_rayas_aux;

  bool hay;
  char evento;
  char simbolo;


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
    } else if (evento=='L') {
      printLetra(caracter(arrayPuntosRayas));
    } else if (evento=='P') {
      printLetra(ESPACIO);
    }

  }

/*
    if (nuevo_simbolo) {
      simbolo_aux       = simbolo;
      nuevo_simbolo_aux = true;
      anchuraPulso_aux  = anchuraPulso;
      nuevo_simbolo     = false;
    }
    if (nueva_letra) {
      nueva_letra_aux = true;
      nueva_letra     = false;
      tiempo_de_pulsos_aux = tiempo_de_pulsos_copia;
      n_puntos_aux         = n_puntos_copia;
      n_rayas_aux          = n_rayas_copia;
    }
    if (nueva_palabra) {
      nueva_palabra_aux = true;
      nueva_palabra     = false;
    }
    
  interrupts();
  */

/*

  if (nuevo_simbolo_aux) {
    Serial.print("[NUEVO_SIMBOLO]");
    acumularPuntoRaya(simbolo_aux);
    printAnchuraPulso(anchuraPulso_aux);
  }

  if (nueva_letra_aux) {
    Serial.print("[NUEVO_LETRA]");
    printLetra(caracter(arrayPuntosRayas));
    Serial.print("{");
    for (int i=0; i<MAX_PUNTOS_RAYAS || arrayPuntosRayas[i]!='\0'; i++) {
      Serial.print(arrayPuntosRayas[i]);
    }
    Serial.print("}");
    int t_dit_medio = ( tiempo_de_pulsos_aux / (n_puntos_aux + 3*n_rayas_aux) );
    wpm_media =  (int)(  (wpm_media + 6000./(5.*t_dit_medio)) / 2.   );
    printWpm();
  }
  
  if (nueva_palabra_aux) {
    Serial.println("[NUEVA_PALABRA]");
    printLetra(ESPACIO);
  }

  */

}


void doChange() {
  if (digitalRead(intPin2) == LOW) doFalling();
  else                             doRising();
}


void doFalling() {
  if (!flancoBajada) {
    anchuraPulso = millis() - startPulso;
    if (anchuraPulso > T_REBOTE) {
      flancoBajada = true;
      startPulso = millis();
      Timer1.stop();
    }
  }
}



void doRising() {
  if (flancoBajada) {
    anchuraPulso = millis() - startPulso;
    if (anchuraPulso > T_REBOTE) {
      tiempo_de_pulsos += anchuraPulso;
      startPulso = millis();
      n_Timer1Start = 0;
      Timer1.start();
      flancoBajada = false;
      if (anchuraPulso < t_th_di_da_ms) {
        simbolo = PUNTO;
        n_puntos++;
       } else {
        simbolo = RAYA;
        n_rayas++;
       }
       colaEventos.encolar('S');
       colaPR.encolar(simbolo);
      nuevo_simbolo = true;
    }
  }
}



void timeOut() {
  switch(n_Timer1Start) {
    case UMBRAL_INTERLETRAS: 

            nueva_letra   = true; 

            tiempo_de_pulsos_copia = tiempo_de_pulsos;
            n_puntos_copia         = n_puntos;
            n_rayas_copia          = n_rayas;

            n_puntos = 0;
            n_rayas  = 0;
            tiempo_de_pulsos = 0;

            colaEventos.encolar('L');

            break;

    case UMBRAL_INTERPALABRAS: 
            nueva_palabra = true; 

            n_puntos = 0;
            n_rayas  = 0;
            tiempo_de_pulsos = 0;
            
            Timer1.stop(); 

            colaEventos.encolar('P');

            break;
  }
  n_Timer1Start++;
}


