

#define DEBUG 


/********************* CONTROL PERIFERICOS *******************/

bool ledOn = false;
bool buzzerOn = false;

/************************* PANTALLA **********************/

#define MAX_LETRAS      13
#define COL_LETRAS_INIC 0
#define COL_LETRAS_FIN  ( COL_LETRAS_INIC + MAX_LETRAS - 1 )
#define FILA_LETRAS     1


#define MAX_PUNTOS_RAYAS      8
#define COL_PUNTOS_RAYAS_INIC 0
#define COL_PUNTOS_RAYAS_MAX  ( COL_PUNTOS_RAYAS_INIC + MAX_PUNTOS_RAYAS - 1 )
#define FILA_PUNTOS_RAYAS     0

#define MAX_ANCHURA_PULSO         9999
#define ANCHURA_PULSO_OVF_STRING  "****" 
#define MAX_DIGITOS_ANCHURA_PULSO 4
#define COL_ANCHURA_PULSO_INIC    ( COL_PUNTOS_RAYAS_MAX + 2 )
#define COL_ANCNURA_PULSO_FIN     ( COL_ANCHURA_PULSO_INIC + MAX_DIGITOS_ANCHURA_PULSO - 1 )
#define FILA_ANCHURA_PULSO        0

#define MAX_WPM         99
#define WPM_OVF_STRING  "**"
#define MAX_DIGITOS_WPM 2

#define COL_WPM_INIC    ( COL_ANCNURA_PULSO_FIN + 2 )
#define COL_WPM_FIN     ( COL_WPM_INIC + MAX_DIGITOS_WPM - 1 )
#define FILA_WPM        0

#define COL_WPM_PP_INIC    ( COL_LETRAS_FIN + 2 )
#define COL_WPM_PP_FIN     ( COL_WPMPP_INIC + MAX_DIGITOS_WPM - 1 )
#define FILA_WPM_PP        1



/**************** TEMPORIZACION *******************/

#define T_REBOTE 10

const int UMBRAL_INTERLETRAS   = 3; //2;
const int UMBRAL_INTERPALABRAS = 7; //5;
const int UMBRAL_INTERSIMBOLOS = 2; 

const float WPM_INIC = 15; 
int wpm_media = WPM_INIC;
long tdi_ms;


long anchuraPulso = 0;
long anchuraSilencio = 0;

volatile int n_TimerStart = 0;

int n_puntos = 0;
int n_rayas = 0;
int n_letras = 0;

long t_inic_palabra = 0;
bool palabra_iniciada = false;
int t_palabra;


long t_flanco_bajada = 0;
long t_flanco_subida = 0;



/******************* VISUALIZACION  *******************/

const char PUNTO = '.';
const char RAYA = '-';
const char ESPACIO = ' ';

char arrayPuntosRayas[MAX_PUNTOS_RAYAS+1];
int contPR = 0;

char lineaLetras[MAX_LETRAS];









////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#ifdef ARDUINO_AVR_UNO

/*************** TEMPORIZACION **************/
#include <TimerOne.h>
void configurarTemporizador(long t_timer_ms) {
  Timer1.stop();
  Timer1.initialize(t_timer_ms*1000);
  Timer1.attachInterrupt(timeOut);
  Timer1.stop();
}

void pararTemporizador() {
  Timer1.stop();
}

void arrancarTemporizador() {
  Timer1.start();
}

void reajustarTemporizador(long t_timer_ms) {
  Timer1.stop();
  Timer1.initialize(t_timer_ms*1000);    
}




/*************** LCD **************************/
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;


void presentacion() {
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("PacoSoft CW dec.");
  lcd.setCursor(0,1);
  lcd.print(WPM_INIC); lcd.print(' '); lcd.print(tdi_ms); lcd.print(' '); lcd.print(UMBRAL_INTERSIMBOLOS * tdi_ms); 
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

void print(String s) {
   lcd.print(s);
}



/************* PERIFERICOS ARDUINO UNO **************/

const int pinManipulador = 2;  // Manipulador
const int ledPin = 13;  // LED
const int buzzer = 4;   // Oscilador


void encenderLed(int valor) {
  if (ledOn) digitalWrite(ledPin, valor);
}

void encenderOscilador(int valor) {
  if (buzzerOn) digitalWrite(buzzer, valor);
}


#endif
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////






/********************** IMPRESION RESULTADOS *********************/


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
  #ifdef DEBUG
    Serial.println(puntoRaya);
  #endif  
  if (contPR==0) {
    gotoXY( COL_PUNTOS_RAYAS_INIC, FILA_PUNTOS_RAYAS );
    for (int i=0; i<MAX_PUNTOS_RAYAS; i++) {
      print(' ');
      arrayPuntosRayas[i] = '\0';
    }
  }
  arrayPuntosRayas[contPR] = puntoRaya;
  gotoXY(contPR + COL_PUNTOS_RAYAS_INIC, FILA_PUNTOS_RAYAS );
  print(puntoRaya);
  contPR++;
  if ( contPR == MAX_PUNTOS_RAYAS ) contPR = 0;
}


void printAnchuraPulso(int anchura) {
  gotoXY( COL_ANCHURA_PULSO_INIC, FILA_ANCHURA_PULSO );
  for (int i = 0; i < MAX_DIGITOS_ANCHURA_PULSO; i++ ) {
    print(' ');
  }
  gotoXY( COL_ANCHURA_PULSO_INIC, FILA_ANCHURA_PULSO );
  if ( anchura <= MAX_ANCHURA_PULSO ) {
    print(anchura);
  } else {
    print(ANCHURA_PULSO_OVF_STRING);
  }
}


void printLetra(char car) {
  gotoXY( COL_LETRAS_INIC, FILA_LETRAS );
  for (int i=0; i<MAX_LETRAS-1; i++) {
    lineaLetras[i] = lineaLetras[i+1];
    print(lineaLetras[i]);
    #ifdef DEBUG 
      Serial.print(lineaLetras[i]);
    #endif
  }
  lineaLetras[MAX_LETRAS-1] = car;
  print(lineaLetras[MAX_LETRAS-1]);
  #ifdef DEBUG 
    Serial.print(lineaLetras[MAX_LETRAS-1]);
  #endif
  contPR = 0;
}


void printWpm(int wpm, int wpm_pp) {
  lcd.setCursor(COL_WPM_INIC, FILA_WPM);
  for (int i = 0; i < MAX_DIGITOS_WPM; i++ ) {
    print(' ');
  }
  lcd.setCursor(COL_WPM_INIC, FILA_WPM);
  if ( wpm <= MAX_WPM ) {
    print( wpm );
  } else {
    print( WPM_OVF_STRING );
  }

  lcd.setCursor(COL_WPM_PP_INIC, FILA_WPM_PP);
  for (int i = 0; i < MAX_DIGITOS_WPM; i++ ) {
    print(' ');
  }
  lcd.setCursor(COL_WPM_PP_INIC, FILA_WPM_PP);
  if ( wpm_pp <= MAX_WPM ) {
    print( wpm_pp );
  } else {
    print( WPM_OVF_STRING );
  }

}




/******************* EVENTOS MANIPULADOR **********************/

#define MAX_COLA 20

#define EVENTO_FLANCO_BAJADA 'B'
#define EVENTO_FLANCO_SUBIDA 'S'
#define EVENTO_TIMEOUT_INTERLETRAS 'L'
#define EVENTO_TIMEOUT_INTERPALABRAS 'P' 

#define ESTADO_INICIAL 0
#define NIVEL_BAJO 1
#define NIVEL_ALTO 2


int estado = ESTADO_INICIAL;

volatile bool llena = false;
volatile bool vacia = true;
volatile long cola_tiempo[MAX_COLA];
volatile int  cola_eventos[MAX_COLA];
volatile int ptr_in  = 0; 
volatile int ptr_out = 0;

void doChange() {
  encolarEvento( ( (digitalRead(pinManipulador)==LOW)? 'B' : 'S' ) );
}

void encolarEvento(char evento) {
  if (llena) return;
  cola_tiempo[ptr_in]  = millis();
  cola_eventos[ptr_in] = evento;
  ptr_in++;
  if (ptr_in == MAX_COLA) ptr_in = 0;
  if (ptr_in == ptr_out)  llena  = true;
  vacia = false;
}
  

void timeOut() {
  switch(n_TimerStart) {
    case UMBRAL_INTERLETRAS: 
      encolarEvento('L');
      break;
    case UMBRAL_INTERPALABRAS: 
      encolarEvento('P');
      break;
  }
  n_TimerStart++;
}






/********************** SETUP ***********************/

void setup(){

  pinMode(pinManipulador, INPUT_PULLUP);

  pinMode(ledPin,  OUTPUT);
  pinMode(buzzer, OUTPUT);

  tdi_ms        = round( 60.f/(50*WPM_INIC ) *1000 );
  configurarTemporizador(tdi_ms);

  #ifdef DEBUG
    Serial.begin(115200);
    Serial.print("\nWPM_INIC: ");
    Serial.println(WPM_INIC);
    Serial.print("tdi_ms: ");
    Serial.println(tdi_ms);
    Serial.print("tdi_ms: ");
    Serial.println(tdi_ms);
  #endif

  for (int i = 0; i < MAX_LETRAS; i++ ) {
    lineaLetras[i]=' ';
  }

  presentacion(); 
  
  attachInterrupt(digitalPinToInterrupt(pinManipulador), doChange,  CHANGE);

}




/************** LOOP *********************/




long arrayTiempos[MAX_PUNTOS_RAYAS*2+10];
int ptr_arrayTiempos;


void flancoBajada(long tiempo) {
  t_flanco_bajada = tiempo;
  anchuraSilencio = t_flanco_bajada - t_flanco_subida;
  Serial.print("FB. "); Serial.print("t_flanco_bajada = "); Serial.print(t_flanco_bajada); Serial.print(" anchuraSilencio = "); Serial.println(anchuraSilencio);
  pararTemporizador();
  encenderLed(HIGH);
  encenderOscilador(HIGH);
  if (!palabra_iniciada) {
    palabra_iniciada = true;
    t_inic_palabra = t_flanco_bajada;
  }
}



char flancoSubida(long tiempo) {
    char simbolo;
    t_flanco_subida = tiempo;
    anchuraPulso = t_flanco_subida - t_flanco_bajada;
    t_palabra = tiempo - t_inic_palabra;
    Serial.print("FS. "); Serial.print("t_flanco_subida = "); Serial.print(t_flanco_subida); Serial.print(" anchuraPulso = "); Serial.println(anchuraPulso);
    n_TimerStart = 0;
    arrancarTemporizador();
    encenderLed(LOW);
    encenderOscilador(LOW);
    if (anchuraPulso < UMBRAL_INTERSIMBOLOS * tdi_ms) {
      simbolo = PUNTO;
      n_puntos++;
    } else {
      simbolo = RAYA;
      n_rayas++;
    }

    return simbolo;
}



void loop() {

  bool hayEvento = false;
  long int tiempo;
  char evento;
  bool nuevo_simbolo;
  char simbolo;
  int n_dits;
  int wpm = 0;

  noInterrupts();
    if (!vacia) {
      hayEvento = true;
      tiempo = cola_tiempo[ptr_out];
      evento = cola_eventos[ptr_out];
      ptr_out++;
      if (ptr_out==MAX_COLA) ptr_out = 0;
      if (ptr_in == ptr_out) vacia = true;
      llena = false;    
    }
  interrupts();

  if (hayEvento) {

    Serial.print("evento: "); Serial.print(evento); 
    Serial.print(", tiempo: "); Serial.println(tiempo);

    switch(evento) {

      case EVENTO_FLANCO_BAJADA:
        Serial.println("evento==EVENTO_FLANCO_BAJADA");
        switch(estado) {
          case ESTADO_INICIAL:
            estado = NIVEL_BAJO;
            break;
          case NIVEL_BAJO:
            break;
          case NIVEL_ALTO:
            if ( tiempo - t_flanco_subida > T_REBOTE ) {
              flancoBajada(tiempo);
              estado = NIVEL_BAJO;
            }
            break;
        } // switch estado
        break;
      
      case EVENTO_FLANCO_SUBIDA:
        nuevo_simbolo = false;
        Serial.println("evento==EVENTO_FLANCO_SUBIDA");
        switch (estado) {
          case ESTADO_INICIAL:
            estado = NIVEL_ALTO;
            break;
          case NIVEL_BAJO: 
            if ( tiempo - t_flanco_bajada > T_REBOTE ) {
              simbolo = flancoSubida(tiempo); 
              nuevo_simbolo = true;     
              estado = NIVEL_ALTO;
            }
            break;
          case NIVEL_ALTO:
            break;
        } // case
        if (nuevo_simbolo) {
          acumularPuntoRaya(simbolo);
          nuevo_simbolo = false;
          printAnchuraPulso(anchuraPulso);

          if (ptr_arrayTiempos==0) {
            arrayTiempos[ptr_arrayTiempos++] = anchuraPulso;
          } else {
            arrayTiempos[ptr_arrayTiempos++] = anchuraSilencio;
            arrayTiempos[ptr_arrayTiempos++] = anchuraPulso;
          }

        }
        break;

      case EVENTO_TIMEOUT_INTERLETRAS:
        printLetra(caracter(arrayPuntosRayas));
        n_letras++;

        #ifdef DEBUG
          Serial.print("\nTemporiaciones de "); Serial.print(caracter(arrayPuntosRayas)); Serial.print(": ");
          for (int i = 0; i<ptr_arrayTiempos; i++) {
            Serial.print(arrayTiempos[i]); Serial.print(' ');
          }
          Serial.println();
        #endif
        ptr_arrayTiempos=0;
        break;

      case EVENTO_TIMEOUT_INTERPALABRAS:
        int n_dits = (n_puntos+n_rayas*3)+(n_puntos+n_rayas-1)*1 + 3*(n_letras-1)+3;
        int wpm_pp = (int) (60000.*n_dits/(50.*t_palabra));

        if (wpm_pp<1 || wpm_pp>MAX_WPM) wpm_pp = WPM_INIC;

        wpm_media = 0.9*wpm_media + 0.1*wpm_pp;

        if (wpm_media<1 || wpm_media>MAX_WPM) wpm_media = WPM_INIC;

        tdi_ms = 60000/(50*wpm_media);
        noInterrupts();
          reajustarTemporizador(tdi_ms);
        interrupts();

        printLetra(ESPACIO);
        printWpm(wpm_media, wpm_pp);

        n_puntos = 0;
        n_rayas  = 0;
        n_letras = 0;
        palabra_iniciada = false;
        break;
    }


  } // hayEvento

} // loop








