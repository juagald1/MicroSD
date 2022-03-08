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

float var1 = 5.26;
float var2 = 7.69;

int main(void)
{

  Init_HW();
  Init_MicroSD();

  Crea_Archivo	  ("PROBA.CSV\0");
  Escribe_Archivo ("PROBA.CSV\0", &var1);
  Escribe_Archivo ("PROBA.CSV\0", &var2);

  while (1)
  {

  }
}




