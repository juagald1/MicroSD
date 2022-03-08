/*
 * Init_MicroSD.c
 *
 *  Created on: 3 mar. 2022
 *      Author: jugaldo
 */

/*
FA_READ				Specifies read access to the file. Data can be read from the file.
FA_WRITE			Specifies write access to the file. Data can be written to the file. Combine with FA_READ for read-write access.
FA_OPEN_EXISTING	Opens a file. The function fails if the file is not existing. (Default)
FA_CREATE_NEW		Creates a new file. The function fails with FR_EXIST if the file is existing.
FA_CREATE_ALWAYS	Creates a new file. If the file is existing, it will be truncated and overwritten.
FA_OPEN_ALWAYS		Opens the file if it is existing. If not, a new file will be created.
FA_OPEN_APPEND		Same as FA_OPEN_ALWAYS except the read/write pointer is set end of the file.

FATFS
"r"					FA_READ
"r+"				FA_READ 		 | FA_WRITE
"w"					FA_CREATE_ALWAYS | FA_WRITE
"w+"				FA_CREATE_ALWAYS | FA_WRITE | FA_READ
"a"					FA_OPEN_APPEND	 | FA_WRITE
"a+"				FA_OPEN_APPEND 	 | FA_WRITE | FA_READ
"wx"				FA_CREATE_NEW 	 | FA_WRITE
"w+x"				FA_CREATE_NEW 	 | FA_WRITE | FA_READ
*/

#include "Init_MicroSD.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"

#define   LONGIUD_MAX_DATO 20
#define	  MAX_DECIMALES	   1000 			/*!< 1000 = 3 DECIMALES, 100 = 2 DECIMALES ...*/

SD_HandleTypeDef hsd;
FATFS 	 		 FAT_FS;
FIL   			 Archivo;
UINT  			 testByte;
sMicroSD		 MicroSD;

char RAIZ_SD [3] = "0:";
char datos_array[LONGIUD_MAX_DATO] = {' '} ;

int  n_bytes;
int  suma;


/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
void SDIO_SD_Init(void)
{

  hsd.Instance				    = SDIO;
  hsd.Init.ClockEdge			= SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass 			= SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave 		= SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide 				= SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl  = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv 			= 0;

}

void Init_MicroSD (void)
{
	  SDIO_SD_Init();
	  FATFS_Init();
}

void Datos_Char (void *datos)
{
	float parte_decimal;
	int parte_entera;
	int parte_decimal_entera;

	float dato = *(float *) datos;			/*!< Se pasa el numero recibido a decimal       */
	parte_entera = dato;					/*!< Obtiene parte entera del numero recibido   */
	parte_decimal = (dato-parte_entera);	/*!< Obtiene parte decimal del numero recibido  */

	/** NUMERO ENTERO */
	if(dato-parte_entera == 0)
	{
		sprintf (datos_array, "%d", parte_entera);
	}

	/** NUMERO DECIMAL */
	else{

		/*!< Si el dato es negativo, cambia a positiva la parte decimal  */
		if (dato < 0)
		{
			parte_decimal = (parte_decimal*(-1));
		}

		parte_decimal = (parte_decimal*MAX_DECIMALES);					    /*!< Tres decimales de precisión ajustar con DEFINE  */
		parte_decimal_entera = parte_decimal;								/*!< Pasa a entera la parte decimal  				 */
		sprintf (datos_array, "%d.%d", parte_entera, parte_decimal_entera);	/*!< Genera un string con la parte entera y decimal  */
	}
}

int Recorre_Array (void){

	for (int i= 0; i<=LONGIUD_MAX_DATO;i++){

		if(datos_array[i] == 0){
			suma++;
		}
	}
	return suma++;
}

/**
  * @brief  Función que permite crear una carpeta en la raiz de la tarjeta Micro SD.
  * @param  TCHAR*: "Nombre de la Carpeta a Crear".
  * @retval None
  */
void Crea_Carpeta (const TCHAR* nombre_carpeta_crear)
{
	if(f_mount(&FAT_FS, RAIZ_SD, 1) == FR_OK)
	{
		if(f_mkdir(nombre_carpeta_crear) != FR_OK)
		{
			MicroSD.Flags.Error.Crea_Carpeta = 1;
		}
	}
}

/**
  * @brief  Función que permite borrar una carpeta de la raiz de la tarjeta Micro SD.
  * @param  TCHAR*: "Nombre de la Carpeta a Borrar".
  * @retval None
  */
void Borra_Carpeta (const TCHAR* nombre_carpeta_borrar)
{
	if(f_mount(&FAT_FS, RAIZ_SD, 1) == FR_OK)
	{
		if(f_unlink(nombre_carpeta_borrar) != FR_OK)
		{
			MicroSD.Flags.Error.Borra_Carpeta = 1;
		}
	}
}

/**
  * @brief  Función que permite crear un nuevo archivo con acceso lectura y escritura en la raiz de la tarjeta Micro SD.
  * 		Si el archivo existe, la función se eliminará y sobrescribirá.
  * 		Imporante! Añadir al final del nombre del archivo su formato.
  * @param  TCHAR*: "Nombre del archivo a Crear" (ej. "DATOS.TXT").
  * @retval None
  */
void Crea_Archivo (const TCHAR* nombre_archivo_crear)
{
	if(f_mount(&FAT_FS, RAIZ_SD, 1) == FR_OK)
	{
		if(f_open(&Archivo, nombre_archivo_crear, FA_CREATE_NEW | FA_WRITE | FA_READ) != FR_OK)
		{
			MicroSD.Flags.Error.Crea_Archivo = 1;
		}
			f_close(&Archivo);
	}
}

/**
  * @brief  Función que permite borrar un archivo de la raiz de la tarjeta Micro SD.
  * 		Imporante! Añadir al final del nombre del archivo su formato.
  * @param  TCHAR*: "Nombre del Archivo a Borrar".
  * @retval None
  */
void Borra_Archivo (const TCHAR* nombre_archivo_borrar)
{
	if(f_mount(&FAT_FS, RAIZ_SD, 1) == FR_OK)
	{
		if(f_unlink(nombre_archivo_borrar) != FR_OK)
		{
			MicroSD.Flags.Error.Borra_Carpeta = 1;
		}
	}
}

/**
  * @brief  Función que permite escribir datos en un archivo de la raiz de la tarjeta Micro SD ya existente.
  * 		Imporante! Añadir al final del nombre del archivo su formato.
  * @param  TCHAR*: "Nombre del Archivo a Escribir".
  * @param  void*:  Datos a escribir en archivo
  * @retval None
  */
void Escribe_Archivo (const TCHAR* nombre_archivo, void *datos)
{
	if(f_mount(&FAT_FS, RAIZ_SD, 1) == FR_OK)
	{

	  /**  MEJORAS
		*  Se debe permitir la entrada de datos como argumento en la función
	    *  Los datos deben entrar por un *void y transformarse a char
        **/

		Datos_Char(datos);

		n_bytes = ((sizeof (datos_array))- Recorre_Array());
		datos_array[n_bytes] = 0x3B;							// 0x3B -> ';' ASCII
		n_bytes = n_bytes+1;									//

		if(f_open(&Archivo, nombre_archivo, FA_OPEN_APPEND | FA_WRITE | FA_READ) != FR_OK)
		{
			MicroSD.Flags.Error.Apertura_Archivo = 1;
		}

		if (f_write(&Archivo, &datos_array, n_bytes, &testByte) != FR_OK)
		{
			MicroSD.Flags.Error.Escritura_Archivo = 1;
		}

		if (f_close(&Archivo) != FR_OK)
		{
			MicroSD.Flags.Error.Cierre_Archivo = 1;
		}
			n_bytes=suma=0;		//JGD METER EN ESTRUCTURA DE DATOS
	}
}
