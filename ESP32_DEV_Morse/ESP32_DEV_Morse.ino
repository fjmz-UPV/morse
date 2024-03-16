#define DEBUG 
#define VERSION "v2.0"

#if defined(ARDUINO_AVR_UNO)
#define CODIGO_ARDUINO
#define SALIDA_POR_PANTALLA
#elif defined(ARDUINO_M5STACK_CORE2)
#define CODIGO_M5STACK_CORE2
#define SALIDA_POR_PANTALLA
#elif defined(ARDUINO_ESP32S3_DEV)
#define CODIGO_M5STACK_S3
#define SALIDA_POR_PANTALLA
#else
#define CODIGO_ESPRESSIF_GENERICO
#endif

#define SERIAL



#include "codigoEspecifico.h"
#include "morse.h"
#include "comunes.h"



/************** PIN MANIPULADOR: variable extern ************************/

int pinManipulador;


/********************* CONTROL POSIBLES PERIFERICOS *******************/

bool ledOn    = true;
bool buzzerOn = false;

/******************* EVENTOS Y COLAS DEL MANIPULADOR **********************/


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



/**************** TEMPORIZACION *******************/

#define T_REBOTE 5

const int UMBRAL_INTERLETRAS   = 3; // 1 < 2 < 3;
const int UMBRAL_INTERPALABRAS = 7; // 3 < 5 < 7;
const int UMBRAL_INTERSIMBOLOS = 2; // 1 puntos, 3 raya

const int WPM_INIC = 15; 
int wpm_media = WPM_INIC;
int tdi_ms;


long anchuraPulso = 0;
long anchuraSilencio = 0;

volatile int n_TimerStart = 0;

int n_puntos = 0;
int n_rayas = 0;
int n_letras = 0;

long t_inic_palabra = 0;
bool palabra_iniciada = false;
long t_palabra;

long t_flanco_bajada = 0;
long t_flanco_subida = 0;



volatile int ns=0;
volatile int nb=0;
/*****************+ EVENTOS **************************/

void subida() {
  portENTER_CRITICAL_ISR(&colaMux);  
    ns++;
    encolarEvento('S');
  portEXIT_CRITICAL_ISR(&colaMux);
}

void bajada() {
  portENTER_CRITICAL_ISR(&colaMux);  
    nb++;
    encolarEvento('B');
  portEXIT_CRITICAL_ISR(&colaMux);
}



portMUX_TYPE colaMux = portMUX_INITIALIZER_UNLOCKED;



void doChange() {
  //int valor = digitalRead(pinManipulador);
  //Serial.println(valor);
  portENTER_CRITICAL_ISR(&colaMux);  
    encolarEvento( ( (digitalRead(pinManipulador)==LOW)? 'B' : 'S' ) );
  //encolarEvento( ( (valor==LOW)? 'B' : 'S' ) );
  portEXIT_CRITICAL_ISR(&colaMux);

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
  
volatile int jjj=0;
void timeOut() {
  jjj++;
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

/*********************************/




void setup() {
  Serial.begin(115200);
  inicPlaca();
  inicManipulador();
  inicEntradaSalida();
  inicTemporizador();
}


void loop() {
  proceso();
  int mi_ns;
  int mi_nb;
    portENTER_CRITICAL_ISR(&colaMux);  
      mi_ns=ns;
      mi_nb=nb;
    portEXIT_CRITICAL_ISR(&colaMux);
  //Serial.printf("ns=%d, nb=%d\n", mi_ns, mi_nb);

  delay(10);
}
