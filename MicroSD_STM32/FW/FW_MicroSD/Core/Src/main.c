/**
**************************************************************************************
\file          main.c
\brief         Archivo principal del firmware para gestión de tarjeta Micro SD en
 	 	 	   modo 1 bit o 4 bits. La gestión de la tarjeta Micro SD se puede seguir
 	 	 	   al completo desde la estructura tMicroSD.

 	 	 	   Es posible modificar la configuración inicial desde Init_MicroSD.h
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

int main(void)
{

  Init_HW();
  Init_MicroSD();

  /** Solo para este ejemplo, borrar para otras aplicaciones */

  int 	var1 	 = 1993;
  float var2 	 = 01.09;
  char 	myData[] = {"Hola Mundo\0"};

  Crea_Archivo	  ("TEST.CSV\0");
  Escribe_Archivo ("TEST.CSV\0", &var1,   Entero);
  Escribe_Archivo ("TEST.CSV\0", &myData, Cadena_Caracteres);
  Escribe_Archivo ("TEST.CSV\0", &var2,   Decimal);

  /** 														 */

  while (1)
  {
  }
}




