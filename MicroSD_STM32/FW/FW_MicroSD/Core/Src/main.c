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
int main(void)
{

  Init_HW();
  Init_MicroSD();

  //Test_1();

  Crea_Carpeta ("/TEST");
  Crea_Carpeta ("/AMUNT");
  Crea_Carpeta ("/VALENCIA");

  while (1)
  {

  }
}




