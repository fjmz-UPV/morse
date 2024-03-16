
#if defined(SALIDA_POR_PANTALLA)



#include "salidaPantalla.h"



/******************* VISUALIZACION  *******************/

const char PUNTO = '.';
const char RAYA = '-';
const char ESPACIO = ' ';

char arrayPuntosRayas[MAX_PUNTOS_RAYAS+1];
int contPR = 0;

char lineaLetras[MAX_LETRAS];



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
      Serial.print(lineaLetras[i]);
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


#endif

