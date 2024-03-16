#ifdef CODIGO_ESPRESSIF_GENERICO

#define USE_PIN

#include "comunes.h"


/************* INIC PLACA  **************/

void inicPlaca() {
  return;
}


/************* INIC MANIPULADOR  **************/

void subida();
void bajada();

extern int pinManipulador;  // pin Manipulador

void inicManipulador() {
  pinManipulador = 25;
  pinMode(pinManipulador, INPUT_PULLUP);
  int pinManipulador2 = 33;
  pinMode(pinManipulador2, INPUT_PULLUP);
  

attachInterrupt(digitalPinToInterrupt(pinManipulador), doChange,  CHANGE);
//attachInterrupt(digitalPinToInterrupt(pinManipulador), subida,  RISING);
//attachInterrupt(digitalPinToInterrupt(pinManipulador2), bajada,  FALLING);



}



/********************* INIC ENTRADA-SALIDA *************************/

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

#define MAX_CAR_MENSAJE 128

void inicEntradaSalida() {
  SerialBT.begin(NOMBRE_DISPOSITIVO);
  #ifdef USE_PIN
    SerialBT.setPin(PIN);
    #ifdef SERIAL
      Serial.printf("Para emparejar con este dispositivo llamado %s, se necesita PIN: %s\n", NOMBRE_DISPOSITIVO.c_str(), PIN );
    #endif
  #endif
}


void enviarPorBluetooth(String mensaje) {
  #ifdef SERIAL
    Serial.printf("mensaje a enviar: %s", mensaje.c_str());
  #endif
  char buffer[MAX_CAR_MENSAJE];
  mensaje.toCharArray(buffer, MAX_CAR_MENSAJE);
  SerialBT.write((uint8_t*)buffer, mensaje.length());
}




/********************* TEMPORIZACION ESP32 *************************/


hw_timer_t *temporizador = NULL;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;





void encolarEvento(char evento);

void doChange2(bool valor) {
  portENTER_CRITICAL_ISR(&timerMux);  
    encolarEvento( ( (valor)? 'B' : 'S' ) );
  portEXIT_CRITICAL_ISR(&timerMux);
}


volatile bool valor = false;

void IRAM_ATTR ISR_temporizador() {
  /*
  portENTER_CRITICAL_ISR(&timerMux);
    valor = !valor;
  portEXIT_CRITICAL_ISR(&timerMux);
  */
  valor = !valor;
//  doChange2(valor);
  
}




void inicTemporizador() {
  //temporizador = timerBegin(0, 80, true);
  //timerAttachInterrupt(temporizador, &ISR_temporizador, true);
  //timerAlarmWrite(temporizador, 1000000, true);
  //timerAlarmEnable(temporizador);
}



/************** PROCESO ******************/


extern volatile bool llena;
extern volatile bool vacia;
extern volatile long cola_tiempo[];
extern volatile int  cola_eventos[];
extern volatile int ptr_in; 
extern volatile int ptr_out;




  extern portMUX_TYPE colaMux;

  long tiempoAnterior = 0;

  int nbbb=0;
  int nsss=0;

  void gestionEventos() {

    bool hayEvento = false;
    long tiempo;
    char evento;
    portENTER_CRITICAL_ISR(&timerMux);
      if (!vacia) {
        hayEvento = true;
        tiempo = cola_tiempo[ptr_out];
        evento = cola_eventos[ptr_out];
        ptr_out++;
        if (ptr_out==MAX_COLA) ptr_out = 0;
        if (ptr_in == ptr_out) vacia = true;
        llena = false;    
      }
    portEXIT_CRITICAL_ISR(&timerMux);

    if (hayEvento) {
      if (tiempo-tiempoAnterior>T_REBOTE) {
        if (evento=='S') nsss++; else nbbb++;
        Serial.printf("nsss: %d, nbbb=%d\n", nsss, nbbb);
        String mensaje = "{e:" + String(evento) + ", t:" + String(tiempo) +"}\n";
        enviarPorBluetooth(mensaje);
      }
      tiempoAnterior = tiempo;
    }
}




bool valor_leido = valor;
bool valor_anterior;



void proceso() {
  /*
  valor_anterior = valor_leido;
  portENTER_CRITICAL_ISR(&timerMux);
    valor_leido = valor;
  portEXIT_CRITICAL_ISR(&timerMux);

  if (valor_leido != valor_anterior) {
    #ifdef SERIAL
      Serial.println("leido: "+String(valor_leido));
    #endif
    String mensaje = "{mensaje: \"hola\", valor: " + String(valor_leido) + "}\n";
    enviarPorBluetooth(mensaje);

  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
*/
  gestionEventos();

}







#endif
