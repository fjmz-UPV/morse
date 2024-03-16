
#ifdef CODIGO_M5STACK_CORE2

#include "comunes.h"


//#include <M5Unified.h>
#include <M5CoreS3.h>




/***************** INICIALILZACION ******************/

void inicPlaca() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  return;
}


/*************** TEMPORIZACION **************/

int nTimer=0;

long t_timer_ms_M5T;

void configurarTemporizador(long t_timer_us) {
  
  return;
  
 
}

void pararTemporizador() {

  
  return;
  
}

void arrancarTemporizador() {
  
  return;  
  
}


void reajustarTemporizador(long t_timer_ms) {
  
  return;

}


/*
#include <utility/M5Timer.h>

M5Timer M5T;
int nTimer=0;

long t_timer_ms_M5T;

void configurarTemporizador(long t_timer_us) {
  t_timer_ms_M5T = t_timer_us/1000;
  #ifdef DEBUG
    Serial.print("t_timer_ms_M5T: "); Serial.println(t_timer_ms_M5T);
  #endif
  pararTemporizador();

  return;
  
  //nTimer = M5T.setTimer(t_timer_ms_M5T, timeOut, 10);
  nTimer = M5T.setInterval(t_timer_ms_M5T, timeOut);
  pararTemporizador();
}

void pararTemporizador() {

  #ifdef DEBUG
    Serial.print("Parando temporizador "); Serial.println(nTimer);
  #endif
  return;
  if (M5T.isEnabled(nTimer)) {
      M5T.deleteTimer(nTimer);
      #ifdef DEBUG
        Serial.print("Parado temporizador!!! ");  Serial.println(nTimer);
      #endif
  }
}

void arrancarTemporizador() {
  #ifdef DEBUG
    Serial.print("Arrancando temporizador "); Serial.println(nTimer);
  #endif
  return;  
  pararTemporizador();
  //nTimer = M5T.setTimer(t_timer_ms_M5T, timeOut, 10);
  ////nTimer = M5T.setInterval(t_timer_ms_M5T, timeOut);
  #ifdef DEBUG
    Serial.print("Arrancado temporizador!!! "); Serial.println(nTimer);
  #endif
}


void reajustarTemporizador(long t_timer_ms) {
  #ifdef DEBUG
    Serial.print("Reajustando temporizador "); Serial.println(nTimer);
  #endif

  return;

  configurarTemporizador(t_timer_ms*1000);
  #ifdef DEBUG
    Serial.print("Reajustado temporizador!!! "); Serial.println(nTimer);
  #endif
}

*/



/******************* AUDIO *********************/

/*
void inicAudio() {
  M5.Speaker.begin();
  M5.Speaker.setVolume(32);
}
*/

void inicAudio() {
  /// I2S Custom configurations are available if you desire.
  auto spk_cfg = CoreS3.Speaker.config();

  if (spk_cfg.use_dac || spk_cfg.buzzer) {
      /// Increasing the sample_rate will improve the sound quality
      /// instead of increasing the CPU load.
      spk_cfg.sample_rate =
          192000;  // default:64000 (64kHz)  e.g. 48000 , 50000 , 80000 ,
                    // 96000 , 100000 , 128000 , 144000 , 192000 , 200000
  }
  /*
      spk_cfg.pin_data_out=8;
      spk_cfg.pin_bck=7;
      spk_cfg.pin_ws=10;     // LRCK

      /// use single gpio buzzer, ( need only pin_data_out )
      spk_cfg.buzzer = false;

      /// use DAC speaker, ( need only pin_data_out ) ( only GPIO_NUM_25
  or GPIO_NUM_26 ) spk_cfg.use_dac = false;
      // spk_cfg.dac_zero_level = 64; // for Unit buzzer with DAC.

      /// Volume Multiplier
      spk_cfg.magnification = 16;
    //*/
    CoreS3.Speaker.config(spk_cfg);
    CoreS3.Speaker.begin();

}




void controlOscilador(int valor) {
  return;
  if (buzzerOn) {
    if (valor==LOW) {
      CoreS3.Speaker.stop();
    } else {
      CoreS3.Speaker.tone(1000, 1000);
    }
  }
}


/****************** LED ************************/


void inicLED() {
  //pinrMode(ledPin, OUTPUT);
}

void controlLED(int valor) {
  return;
  if (ledOn) {
    if (valor!=LOW) {
        M5.Lcd.fillCircle(310, 10, 5, YELLOW);
    } else {
        M5.Lcd.fillCircle(310, 10, 5, BLACK);
    }
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
  print("PacoSoft CW Decoder! ");
  print(VERSION); print("\n");
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


************* MANIPULADOR  **************/

extern int pinManipulador;  // pin Manipulador

void inicManipulador() {
  pinManipulador = 1; // 33 Core2; 1 Core S3
  pinMode(pinManipulador, INPUT_PULLUP);
}


#endif

