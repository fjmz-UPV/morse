/********************** IMPRESION RESULTADOS *********************/

#include "morse.h"

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
