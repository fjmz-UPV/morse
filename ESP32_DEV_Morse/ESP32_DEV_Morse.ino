#define DEBUG 
#define VERSION "v3.0"
#define SERIAL
#define USE_PIN




/************* INIC MANIPULADOR  **************/

int pinManipuladorVertical;

void inicManipulador() {

  pinManipuladorVertical = 32;
  pinMode(pinManipuladorVertical, INPUT);

  attachInterrupt(digitalPinToInterrupt(pinManipuladorVertical), doChange,  CHANGE);

}


/****************** LED ************************/

// GPIO LED
const int ledPuntoRaya = 25;
const int ledPunto     = 26;
const int ledRaya      = 27;


bool ledPuntoRayaOn = true;
//bool ledPuntoOn     = true;
//bool ledRayaOn      = true;
//bool buzzerOn = false;


void inicLED() {
  pinMode(ledPuntoRaya, OUTPUT);
  //pinMode(ledPunto, OUTPUT);
  //pinMode(ledRaya,  OUTPUT);
}


void controlLED(int valor) {
  if (ledPuntoRayaOn) { 
    digitalWrite(ledPuntoRaya, valor);
  }
}



/********************* INIC ENTRADA-SALIDA: BLUETOOTH *************************/

#include "BluetoothSerial.h"

String NOMBRE_DISPOSITIVO = "LlaveMorse";


#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *PIN = "3141592"; // Change this to more secure PIN.


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void inicEntradaSalida() {
  SerialBT.begin(NOMBRE_DISPOSITIVO);
  #ifdef USE_PIN
    SerialBT.setPin(PIN);
    #ifdef SERIAL
      Serial.printf("Para emparejar con este dispositivo llamado %s, se necesita PIN: %s\n", NOMBRE_DISPOSITIVO.c_str(), PIN );
    #endif
  #endif
}

#define MAX_CAR_MENSAJE 256
char buffer[MAX_CAR_MENSAJE];

void enviarPorBluetooth(String mensaje) {
  #ifdef SERIAL
    Serial.printf("mensaje a enviar: %s", mensaje.c_str());
  #endif

  mensaje.toCharArray(buffer, MAX_CAR_MENSAJE);
  SerialBT.write((uint8_t*)buffer, mensaje.length());
}


/**************** TEMPORIZACION *******************/


#define T_REBOTE 5


// umbrales en dits
const int UMBRAL_INTERLETRAS   = 3; 
const int UMBRAL_INTERPALABRAS = 7; 

int wpm = 15; 
int tdi_ms = 60./50./wpm*1000;
long tdi_us = (long)tdi_ms*1000;


/********************* TEMPORIZACION ESP32 *************************/


hw_timer_t *temporizador = NULL;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


volatile int n_TimerStart = 0;

volatile int kkk=0;

void IRAM_ATTR ISR_temporizador() {

  portENTER_CRITICAL_ISR(&timerMux);

    kkk++;
    switch(n_TimerStart) {
      case UMBRAL_INTERLETRAS: 
        encolarEvento('L');
        n_TimerStart++;
        break;
      case UMBRAL_INTERPALABRAS: 
        encolarEvento('P');
        timerStop(temporizador);
        //timerAlarmDisable(temporizador);
        break;
      default:
        n_TimerStart++;  
    }

  portEXIT_CRITICAL_ISR(&timerMux);

  
}




void inicTemporizador() {
  temporizador = timerBegin(0, 80, true);
  timerAttachInterrupt(temporizador, &ISR_temporizador, true);
  timerAlarmWrite(temporizador, tdi_us, true);
  //timerAlarmEnable(temporizador);
  Serial.println("inicTemporizador!!!");
}







/******************* EVENTOS Y COLAS DEL MANIPULADOR **********************/


#define EVENTO_FLANCO_BAJADA 'B'
#define EVENTO_FLANCO_SUBIDA 'S'
#define EVENTO_TIMEOUT_INTERLETRAS 'L'
#define EVENTO_TIMEOUT_INTERPALABRAS 'P' 

#define MAX_COLA 128

volatile bool llena = false;
volatile bool vacia = true;
volatile long cola_tiempo[MAX_COLA];
volatile int  cola_eventos[MAX_COLA];
volatile int ptr_in  = 0; 
volatile int ptr_out = 0;






/************** PROCESO ******************/

volatile int jjj=0;


extern portMUX_TYPE colaMux;

long tiempoAnterior = 0;

void gestionEventos() {

  bool hayEvento = false;
  long tiempo;
  char evento;
  int mi_jjj;
  int mi_kkk;
  portENTER_CRITICAL_ISR(&timerMux);
    if (!vacia) {
      hayEvento = true;
      tiempo = cola_tiempo[ptr_out];
      evento = cola_eventos[ptr_out];
      ptr_out++;
      if (ptr_out==MAX_COLA) ptr_out = 0;
      if (ptr_in == ptr_out) vacia = true;
      llena = false;    
      mi_jjj=jjj;
      mi_kkk=kkk;
    }
  portEXIT_CRITICAL_ISR(&timerMux);

  if (hayEvento) {
    if (tiempo-tiempoAnterior>T_REBOTE) {
      String mensaje = "{e:" + String(evento) + ", t:" + String(tiempo) +"}\n";
      enviarPorBluetooth(mensaje);
      Serial.print("mi_jjj: "); Serial.println(mi_jjj);
      Serial.print("n_TimerStart: "); Serial.println(n_TimerStart);
      Serial.print("mi_kkk: "); Serial.println(kkk);

    }
    tiempoAnterior = tiempo;
  }
}





void proceso() {

  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }

  gestionEventos();

}








/*****************+ EVENTOS **************************/

portMUX_TYPE colaMux = portMUX_INITIALIZER_UNLOCKED;



void doChange() {
  portENTER_CRITICAL_ISR(&colaMux);  
    int nivel = digitalRead(pinManipuladorVertical);
    encolarEvento( ( (nivel==LOW)? 'S' : 'B' ) );  // Lógica invertida por trigger Shcmitt
    controlLED(nivel);
    if (nivel==HIGH) { 
      n_TimerStart = 0;
      //timerRestart(temporizador);
      timerRestart(temporizador);
      timerAlarmWrite(temporizador, tdi_us, true);
      timerAlarmEnable(temporizador);
      timerStart(temporizador);
      jjj++;
    }
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
  

/*********************************/




void setup() {
  Serial.begin(115200);
  #ifdef DEBUG
    Serial.print("tdi_ms: "); Serial.print(tdi_ms); Serial.print(", tdi_us: "); Serial.println(tdi_us);
  #endif
  inicManipulador();
  inicLED();
  inicEntradaSalida();
  inicTemporizador();

}


void loop() {
  proceso();
  delay(10);
}
