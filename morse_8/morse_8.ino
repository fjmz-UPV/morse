


#define DEBUG false

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
const char ESPACIO = ' ';



volatile char simbolo;
volatile long startPulso = 0;
volatile long anchuraPulso = 0;
volatile bool flancoBajada = false;

int wpm_media = 0;

char letra[10];
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

  Serial.print("wpm_inic: ");
  Serial.println(wpm_inic);

  Serial.print("tdi_ms: ");
  Serial.println(tdi_ms);

  Serial.print("t_timer_us: ");
  Serial.println(t_timer_us);





  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);
  lcd.print("hello, world!");

  delay(1000);

  lcd.setCursor(0,0);
  for (int i=0; i<16; i++) {
    lcd.print(linea0[i]);
  }

  lcd.setCursor(0,1);
  for (int i=0; i<16; i++) {
    lcd.print(linea1[i]);
  }
}


char caracter(char letra[]) {
  if (String(letra)==".")       return 'E';
  else if (String(letra)=="-")  return 'T';

  else if (String(letra)==".-") return 'A';
  else if (String(letra)=="..") return 'I';
  else if (String(letra)=="-.") return 'N';
  else if (String(letra)=="--") return 'M';

  else if (String(letra)=="...") return 'S';
  else if (String(letra)=="..-") return 'U';
  else if (String(letra)==".-.") return 'R';
  else if (String(letra)==".--") return 'W';
  else if (String(letra)=="-..") return 'D';
  else if (String(letra)=="-.-") return 'K';
  else if (String(letra)=="--.") return 'G';
  else if (String(letra)=="---") return 'O';

  else if (String(letra)=="....") return 'H';
  else if (String(letra)=="...-") return 'V';
  else if (String(letra)=="..-.") return 'F';
  //else if (String(letra)=="..--") return 'S';
  else if (String(letra)==".-..") return 'L';
  //else if (String(letra)==".-.-") return 'S';
  else if (String(letra)==".--.") return 'P';
  else if (String(letra)==".---") return 'J';
  else if (String(letra)=="-...") return 'B';
  else if (String(letra)=="-..-") return 'X';
  else if (String(letra)=="-.-.") return 'C';
  else if (String(letra)=="-.--") return 'Y';
  else if (String(letra)=="--..") return 'Z';
  else if (String(letra)=="--.-") return 'Q';
  //else if (String(letra)=="---.") return 'S';
  //else if (String(letra)=="----") return 'S';

  else if (String(letra)==".----") return '1';
  else if (String(letra)=="..---") return '2';
  else if (String(letra)=="...--") return '3';
  else if (String(letra)=="....-") return '4';
  else if (String(letra)==".....") return '5';
  else if (String(letra)=="-....") return '6';
  else if (String(letra)=="--...") return '7';
  else if (String(letra)=="---..") return '8';
  else if (String(letra)=="----.") return '9';
  else if (String(letra)=="-----") return '0';

  else return '*';
}


void printCursorCoords(char control, int columna, int fila) {
  Serial.print("["); Serial.print(control); Serial.print(";"); Serial.print(columna); Serial.print(","); Serial.print(fila); Serial.print("]"); 
}




void printSimbolo(char simbolo) {
  Serial.print(simbolo);
  linea0[cursorC_PR++] = simbolo;
  lcd.setCursor(0,0);
  for (int i=0; i<16; i++) {
    lcd.print(linea0[i]);
  }


}

void printLetra(char letra[]) {
  Serial.print(caracter(letra));
  
  lcd.setCursor(0,1);
  for (int i=0; i<15; i++) {
    linea1[i] = linea1[i+1];
    lcd.print(linea1[i]);
  }
  linea1[15] = caracter(letra);
  lcd.print(linea1[15]);
  
  for (int i=0; i<16; i++) {
    linea0[i]=' ';
    cursorC_PR=0;
  }

}



void printEspacio() {
  return;
  Serial.print(' ');
  lcd.setCursor(cursorC_letras++,1);
  lcd.print(ESPACIO);
  cursorC_PR=PRIMERA;
} 

void printWpm() {
  //Serial.print("("); Serial.print(wpm_media); Serial.print(" wpm)");
}

void printAnchuraPulso(int ancho) {
  //Serial.print("["); Serial.print(ancho); Serial.print(" ms]");
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

  noInterrupts();
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

  if (nuevo_simbolo_aux) {
    letra[contPR++] = simbolo_aux;
    letra[contPR]   = '\0';
    printSimbolo(simbolo_aux);
    printAnchuraPulso(anchuraPulso_aux);
  }

  if (nueva_letra_aux) {
    contPR = 0;
    printLetra(letra);
    int t_dit_medio = ( tiempo_de_pulsos_aux / (n_puntos_aux + 3*n_rayas_aux) );
    wpm_media =  (int)(  (wpm_media + 6000./(5.*t_dit_medio)) / 2.   );
    printWpm();
  }
  
  if (nueva_palabra_aux) {
    contPR = 0;
    printEspacio();
  }

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
            break;

    case UMBRAL_INTERPALABRAS: 
            nueva_palabra = true; 

            n_puntos = 0;
            n_rayas  = 0;
            tiempo_de_pulsos = 0;
            
            Timer1.stop(); 
            break;
  }
  n_Timer1Start++;
}


