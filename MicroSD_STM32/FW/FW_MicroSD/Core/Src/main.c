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

int i =1;
int main(void)
{

  Init_HW();
  Init_MicroSD();

  //Crea_Archivo("AMUNT.TXT");
  Escribe_Archivo("AMUNT.TXT");


  while (1)
  {

  }
}




