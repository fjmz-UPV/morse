

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

#define T_REBOTE 5

const int UMBRAL_INTERLETRAS   = 3; //2;
const int UMBRAL_INTERPALABRAS = 7; //5;
const int UMBRAL_INTERSIMBOLOS = 2; 

const int WPM_INIC = 15; 
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
//////////////// ARDUINO UNO ///////////////////////////////////
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
  Serial.println("Parando temporizador");
  Timer1.stop();
}

void arrancarTemporizador() {
  Serial.println("Arrancando temporizador");
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

void print(long x) {
  lcd.print(x);
}

void print(String s) {
   lcd.print(s);
}



/************* MANIPULADOR ARDUINO UNO **************/

const int pinManipulador = 2;  // Manipulador





#endif



////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//////////////// M%STACK CORE 2 ////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#ifdef ARDUINO_M5STACK_Core2

#include <M5Unified.h>

/*************** TEMPORIZACION **************/


#include <utility/M5Timer.h>

M5Timer M5T;
int nTimer;

long t_timer_ms_M5T;

void configurarTemporizador(long t_timer_ms) {
  t_timer_ms_M5T = t_timer_ms;
  pararTemporizador();
  nTimer = M5T.setTimer(t_timer_ms_M5T, timeOut, 10);
  pararTemporizador();
}

void pararTemporizador() {
  if (M5T.isEnabled(nTimer)) {
      M5T.deleteTimer(nTimer);
  }
}

void arrancarTemporizador() {
  pararTemporizador();
  nTimer = M5T.setTimer(t_timer_ms_M5T, timeOut, 10);
}

void reajustarTemporizador(long t_timer_ms) {
  configurarTemporizador(t_timer_ms);
}



/******************* AUDIO *********************/

void inicAudio() {
  M5.Speaker.begin();
  M5.Speaker.setVolume(32);
}

void controlOscilador(int valor) {
  if (valor==LOW) {
    M5.Speaker.stop();
  } else {
    M5.Speaker.tone(1000, 1000);
  }
}


/****************** LED ************************/


void inicLED() {
  //pinrMode(ledPin, OUTPUT);
}

void controlLED(int valor) {
  if (valor!=LOW) {
      M5.Lcd.fillCircle(310, 10, 5, YELLOW);
    } else {
      M5.Lcd.fillCircle(310, 10, 5, BLACK);
}
}


/*************** LCD **************************/

// 320 x 240 px

void inicDisplay() {
  M5.begin();
  M5.Lcd.clear();
  M5.Lcd.setTextSize(2);
}

void presentacion() {
  print("PacoSoft CW Decoder\n");
  print(WPM_INIC); print(' '); print(tdi_ms); print(' '); print(UMBRAL_INTERSIMBOLOS*tdi_ms);
}


/************ CONTROL PANTALLA M5STACK CORE2 *********************/ 

#undef FILA_LETRAS    
#undef FILA_PUNTOS_RAYAS     
#undef FILA_ANCHURA_PULSO       
#undef FILA_WPM      
#undef FILA_WPM_PP        

#define FILA_LETRAS        4
#define FILA_PUNTOS_RAYAS  3
#define FILA_ANCHURA_PULSO 2
#define FILA_WPM           2
#define FILA_WPM_PP        2




#define ESCALA 50

void gotoXY(int x, int y) {
  M5.Lcd.setCursor(x*ESCALA, y*ESCALA);
}

void print(String x) {
  M5.Lcd.print(x);
}

void print(char x) {
  M5.Lcd.print(x);
}

void print(int x) {
  M5.Lcd.print(x);
}


void print(long x) {
  M5.Lcd.print(x);
}


/************* MANIPULADOR ARDUINO UNO **************/

const int pinManipulador = 33;  // Manipulador



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
    //Serial.println(puntoRaya);
  #endif  
  if (contPR==0) {
    gotoXY( COL_PUNTOS_RAYAS_INIC, FILA_PUNTOS_RAYAS );
    for (int i=0; i<MAX_PUNTOS_RAYAS; i++) {
      print(' ');
      arrayPuntosRayas[i] = '\0';
    }
  }
  arrayPuntosRayas[contPR] = puntoRaya;
  gotoXY(COL_PUNTOS_RAYAS_INIC, FILA_PUNTOS_RAYAS);
  for (int i=0; i<=contPR; i++) {
    print(arrayPuntosRayas[i]);
  }
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
      //Serial.print(lineaLetras[i]);
    #endif
  }
  lineaLetras[MAX_LETRAS-1] = car;
  print(lineaLetras[MAX_LETRAS-1]);
  #ifdef DEBUG 
    Serial.print("Nueva letra: ");
    Serial.println(lineaLetras[MAX_LETRAS-1]);
  #endif
  contPR = 0;
}


void printWpm(int wpm, int wpm_pp) {
  gotoXY(COL_WPM_INIC, FILA_WPM);
  for (int i = 0; i < MAX_DIGITOS_WPM; i++ ) {
    print(' ');
  }
  gotoXY(COL_WPM_INIC, FILA_WPM);
  if ( wpm <= MAX_WPM ) {
    print( wpm );
  } else {
    print( WPM_OVF_STRING );
  }

  gotoXY(COL_WPM_PP_INIC, FILA_WPM_PP);
  for (int i = 0; i < MAX_DIGITOS_WPM; i++ ) {
    print(' ');
  }
  gotoXY(COL_WPM_PP_INIC, FILA_WPM_PP);
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

#define ESTADO_INICIAL 2
#define NIVEL_BAJO 0
#define NIVEL_ALTO 1


int estado = ESTADO_INICIAL;

volatile bool llena = false;
volatile bool vacia = true;
volatile long cola_tiempo[MAX_COLA];
volatile int  cola_eventos[MAX_COLA];
volatile int ptr_in  = 0; 
volatile int ptr_out = 0;


void inicManipulador() {
  pinMode(pinManipulador, INPUT_PULLUP);
}


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


  inicManipulador();
  inicDisplay();
  inicAudio();
  inicLED();


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
  noInterrupts();
    pararTemporizador();
  interrupts();
  controlLED(HIGH);
  controlOscilador(HIGH);
  if (!palabra_iniciada) {
    palabra_iniciada = true;
    t_inic_palabra = t_flanco_bajada;
  }
}

void falsoFlancoBajada(long tiempo) {
  #ifdef DEBUG
    Serial.println("Falso flanco de bajada");
  #endif  
  flancoBajada(tiempo);
}

char falsoFlancoSubida(long tiempo) {
  #ifdef DEBUG
    Serial.println("Falso flanco de subida");
  #endif
  return flancoSubida(tiempo);
}

char flancoSubida(long tiempo) {
    char simbolo;
    t_flanco_subida = tiempo;
    anchuraPulso = t_flanco_subida - t_flanco_bajada;
    t_palabra = tiempo - t_inic_palabra;
    Serial.print("FS. "); Serial.print("t_flanco_subida = "); Serial.print(t_flanco_subida); Serial.print(" anchuraPulso = "); Serial.println(anchuraPulso);
    noInterrupts();
      n_TimerStart = 0;
      arrancarTemporizador();
    interrupts();
    controlLED(LOW);
    controlOscilador(LOW);
    if (anchuraPulso < UMBRAL_INTERSIMBOLOS * tdi_ms) {
      simbolo = PUNTO;
      n_puntos++;
    } else {
      simbolo = RAYA;
      n_rayas++;
    }

    return simbolo;
}





#ifdef DEBUG
String strEstado(int estado) {
  switch(estado) {
    case ESTADO_INICIAL: return "ESTADO_INICIAL"; 
    case NIVEL_BAJO: return "NIVEL_BAJO";
    case NIVEL_ALTO: return "NIVEL_ALTO";
  }
}

String strEvento(char evento) {
  switch(evento) {
    case 'B': return "Flanco de BAJADA";
    case 'S': return "Flanco de SUBIDA";
    case 'L': return "Timeout por LETRA";
    case 'P': return "Timeout por PALABRA";
  }
}
#endif

void loop() {

  #ifdef ARDUINO_M5STACK_Core2
    M5T.run();
  #endif

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

    #ifdef DEBUG 
      Serial.print("\nevento: "); Serial.print(strEvento(evento)); 
      Serial.print(", estado: "); Serial.print(strEstado(estado));
      Serial.print(", tiempo: "); Serial.print(tiempo);
      Serial.print(", t_flanco_bajada: "); Serial.print(t_flanco_bajada);
      Serial.print(", t_flanco_subida: "); Serial.print(t_flanco_subida);
      Serial.println();
    #endif

    switch(evento) {

      case EVENTO_FLANCO_BAJADA:
        nuevo_simbolo = false;
        switch(estado) {
          case ESTADO_INICIAL:
            estado = NIVEL_BAJO;
            t_flanco_bajada = tiempo;
            break;
          case NIVEL_BAJO:
            if ( tiempo - t_flanco_bajada > T_REBOTE ) {
              simbolo = falsoFlancoSubida(tiempo);
              nuevo_simbolo = true;  
              estado = NIVEL_ALTO;
            }
            break;
          case NIVEL_ALTO:
            if ( tiempo - t_flanco_subida > T_REBOTE ) {
              flancoBajada(tiempo);
              estado = NIVEL_BAJO;
            }
            break;
        } // switch estado
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
      
      case EVENTO_FLANCO_SUBIDA:
        nuevo_simbolo = false;
        switch (estado) {
          case ESTADO_INICIAL:
            estado = NIVEL_ALTO;
            t_flanco_subida = tiempo;
            break;
          case NIVEL_BAJO: 
            if ( tiempo - t_flanco_bajada > T_REBOTE ) {
              simbolo = flancoSubida(tiempo); 
              nuevo_simbolo = true;     
              estado = NIVEL_ALTO;
            }
            break;
          case NIVEL_ALTO:
              if ( tiempo - t_flanco_subida > T_REBOTE ) {
                falsoFlancoBajada(tiempo);
                estado = NIVEL_BAJO;
              }
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
          //reajustarTemporizador(tdi_ms);
        interrupts();

        printLetra(ESPACIO);
        printWpm(wpm_media, wpm_pp);

        n_puntos = 0;
        n_rayas  = 0;
        n_letras = 0;
        palabra_iniciada = false;
        break;
    }

    #ifdef DEBUG
      Serial.print("Tras proceso, nivel: "); Serial.println(strEstado(estado));
    #endif

  } // hayEvento

} // loop








