/**
**************************************************************************************
\file          main.c
\brief         Archivo principal del Firmware para gestión de tarjeta Micro SD modo
 	 	 	   1 bit.
\details
\author        Juan Galbis Domènech
\version       1.0
\date          03/03/2022
**************************************************************************************/

#include "main.h"
#include "Init_HW.h"
#include "Init_MicroSD.h"


/**
  * @brief  The application entry point.
  * @retval int
  */

int 	var1 = 555;
float 	var2 = 798.12;
char 	myData[] = {"HOLA\0"};

int main(void)
{

  Init_HW();
  Init_MicroSD();

  Crea_Archivo	  ("TEST.CSV\0");
  Escribe_Archivo ("TEST.CSV\0", &var1, 	 Entero);
  Escribe_Archivo ("TEST.CSV\0", &var2, 	 Decimal);
  Escribe_Archivo ("TEST.CSV\0", &myData, Cadena_Caracteres);

  while (1)
  {
  }
}




