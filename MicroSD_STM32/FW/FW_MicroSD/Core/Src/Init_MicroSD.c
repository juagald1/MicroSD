/*
 * Init_MicroSD.c
 *
 *  Created on: 3 mar. 2022
 *      Author: jugaldo
 */

#include "fatfs.h"
#include "Init_MicroSD.h"

SD_HandleTypeDef hsd;

FATFS FAT_FS;
FIL   Archivo;
UINT  testByte;

sMicroSD MicroSD;

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
void SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;

}

void Init_MicroSD (void)
{
	  SDIO_SD_Init();
	  FATFS_Init();
}


void Test_1 (void){

	if(f_mount(&FAT_FS, SDPath, 1) == FR_OK)
	{
		char myPath[] = "WRITE3.TXT\0";
		f_open(&Archivo, myPath, FA_WRITE | FA_CREATE_ALWAYS); //JGD escribe en archivo o lo crea
		char myData[] = "GOLAZO DE GUEDES\0";
		f_write(&Archivo, myData, sizeof(myData), &testByte);
		f_close(&Archivo);
	}
}


/**
  * @brief  Función que permite crear una carpeta en una ruta concreta de la tarjeta Micro SD.
  * @param  TCHAR*: "/RUTA/Nombre de la Carpeta/".
  * @retval None
  */
void Crea_Carpeta (const TCHAR* nombre_carpeta){

	if(f_mount(&FAT_FS, SDPath, 1) == FR_OK)
	{
		if(f_mkdir(nombre_carpeta) != FR_OK)
		{
			MicroSD.Flags.Error.Crea_Carpeta = 1;
		}
	}
}
