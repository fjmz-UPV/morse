#ifdef CODIGO_ARDUINO

#include "comunes.h"


/***************** INICIALILZACION ******************/

void inicPlaca() {
  return;
}

/************* MANIPULADOR  **************/

extern int pinManipulador;  // pin Manipulador

void inicManipulador() {
  pinManipulador = 2;
  pinMode(pinManipulador, INPUT_PULLUP);
}


/************** ENTRADA/SALIDA **************/

void inicEntradaSalida() {
  inicDisplay();
  inicAudio();
  inicLED();
}


/*************** TEMPORIZACION **************/

#include <TimerOne.h>
void configurarTemporizador(long t_timer_us) {
  pararTemporizador();
  #ifdef DEBUG
      Serial.print("config. temporizador (us): "); Serial.println(t_timer_us);
  #endif
  Timer1.initialize(t_timer_us);
  Timer1.attachInterrupt(timeOut);
  pararTemporizador();
}

void pararTemporizador() {
  #ifdef DEBUG
    Serial.println("Parando temporizador");
  #endif
  Timer1.stop();
}

void arrancarTemporizador() {
  #ifdef DEBUG
    Serial.println("Arrancando temporizador");
  #endif
  Timer1.start();
}


void reajustarTemporizador(long t_timer_ms) {
  Timer1.stop();
  Timer1.initialize(t_timer_ms*1000);    
}






/***************  AUDIO **********************/

const int buzzer = 4;   // Oscilador

void inicAudio() {
  pinMode(buzzer, OUTPUT);
}

void controlOscilador(int valor) {
  if (buzzerOn) digitalWrite(buzzer, valor);
}


/****************** LED ************************/

const int ledPin = 13;  // LED

void inicLED() {
  pinMode(ledPin, OUTPUT);
}

void controlLED(int valor) {
  if (ledOn) digitalWrite(ledPin, valor);
}


/*************** LCD **************************/
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;



void inicDisplay() {
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
}

void presentacion() {
  lcd.setCursor(0,0);
  lcd.print("PacoSoft CW dec.");
  lcd.setCursor(0,1);
  lcd.print(WPM_INIC); lcd.print(' '); lcd.print(tdi_ms); lcd.print(' '); lcd.print(UMBRAL_INTERSIMBOLOS * tdi_ms);  lcd.print(" "); lcd.print(VERSION);
  delay(2000);
  lcd.clear();
}


/************ CONTROL PANTALLA ARDUINO UNO *********************/ 

void gotoXY(int x, int y) {
  lcd.setCursor(x, y);
}

void print(char c) {
  lcd.print(c);
}

void print(int x) {
  lcd.print(x);
}

void print(long x) {
  lcd.print(x);
}

void print(String s) {
   lcd.print(s);
}





#endif


