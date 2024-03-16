#ifndef SALIDA_PANTALLA_H
#define SALIDA_PANTALLA_H

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


void acumularPuntoRaya(char puntoRaya);

void printAnchuraPulso(int anchura);

void printLetra(char car);

void printWpm(int wpm, int wpm_pp);


#endif