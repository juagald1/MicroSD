/**
**************************************************************************************
\file          Init_MicroSD.c
\brief         Módulo que contiene las funciones desarrolladas para gestionar una
			   tarjeta micro SD
\details	   Codificacion ASCII UTF8
\author        Juan Galbis Domènech
\version       1.0
\date          03/03/2022
**************************************************************************************/

#include "Init_MicroSD.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//* FATFS						*/
SD_HandleTypeDef hsd;
FATFS 	 		 FAT_FS;
FIL   			 Archivo;
UINT  			 testByte;

//* GESTION MICRO SD			*/
tMicroSD		 MicroSD;

int x;


/**
  * @brief Función que permite inicializar el modulo SDIO
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

  if (HAL_SD_Init(&hsd) != HAL_OK)
  {
	 MicroSD.Flags.Error.SD_Init = 1;
  }
  if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
  {
	 MicroSD.Flags.Error.Config_Bus_Wide = 1;
  }

}

/**
  * @brief Función que permite inicializar y configurar el modulo SDIO
  * @param None
  * @retval None
  */
void Init_MicroSD (void)
{
	  SDIO_SD_Init();
	  FATFS_Init();

	  strcpy(MicroSD.Raiz_SD, 	  "0:");																		/*!< Inicializa nombre de la raiz de la tarjeta micro SD	    	*/
	  strcpy(MicroSD.Datos_String, " ");																		/*!< Inicializa string donde se alamacenara los datos conbertidos	*/
}

/**
  * @brief  Función que permite convertir a string datos de tipo entero, decimal y cadena de caracteres
  * @param void*: Datos a convertir a string
  * @param Tipo_Dato: Tipo de dato a escribir en memoria micro SD
  * @retval '1' Conversion Correcta '0' Conversion Incorrecta
  */
uint8_t Datos_String 	 (void *datos, tTipoDatos Tipo_Dato)
{
	/*!< Filtro para comprobra si el tipo de dato introducido es admisible					*/
	if((Tipo_Dato != Entero) && (Tipo_Dato != Decimal) && (Tipo_Dato != Cadena_Caracteres))
	{
		Tipo_Dato = Desconocido;
	}
		switch (Tipo_Dato)
		{

		case Entero:
			MicroSD.CS.Dato_Entero = *(int *) datos;															/*!< Se pasa el numero recibido a decimal      				    */
			sprintf (MicroSD.Datos_String, "%d", MicroSD.CS.Dato_Entero);										/*!< Genera un string con la parte entera			 			*/
			strcat  (MicroSD.Datos_String, ";");
			MicroSD.CS.estado = 1;
			break;

		case Decimal:
			MicroSD.CS.Dato_Decimal   = *(float *) datos;														/*!< Se pasa el numero recibido a decimal      				    */
			MicroSD.CS.Parte_Entera   = MicroSD.CS.Dato_Decimal;												/*!< Obtiene parte entera del numero recibido       			*/
			MicroSD.CS.Parte_Decimal  = MicroSD.CS.Dato_Decimal - MicroSD.CS.Parte_Entera;						/*!< Obtiene parte decimal del numero recibido  				*/

			if (MicroSD.CS.Dato_Decimal  < 0)																	/*!< Si el dato es negativo, cambia a positiva la parte decimal */
			{
				MicroSD.CS.Parte_Decimal 	 = (MicroSD.CS.Parte_Decimal*(-1));
			}

			MicroSD.CS.Parte_Decimal 		 = (MicroSD.CS.Parte_Decimal*MAX_DECIMALES);						/*!< Tres decimales de precisión ajustar con DEFINE  			 */
			MicroSD.CS.Parte_Decimal_Entera  = MicroSD.CS.Parte_Decimal;										/*!< Pasa a entera la parte decimal  							 */
			sprintf (MicroSD.Datos_String, "%d.%d", MicroSD.CS.Parte_Entera, MicroSD.CS.Parte_Decimal_Entera);	/*!< Genera un string con la parte entera y decimal 			 */
			strcat  (MicroSD.Datos_String, ";");
			MicroSD.CS.estado = 1;
			break;

		case Cadena_Caracteres:
			strcpy	(MicroSD.Datos_String, datos);
			strcat  (MicroSD.Datos_String, ";");
			MicroSD.CS.estado = 1;
			break;

		case Desconocido:
			MicroSD.CS.estado = 0;
			break;
		}

	if(MicroSD.CS.estado == 1)
	{
		for(x=0; x<=sizeof(MicroSD.Datos_String); x++)
		{
			if(MicroSD.Datos_String[x] != ';')
			{
				MicroSD.CS.n++;
			}else{
				MicroSD.CS.n_bytes = MicroSD.CS.n;
			}
		}
	}

	return MicroSD.CS.estado;
}

/**
  * @brief  Función que permite crear una carpeta en la raiz de la tarjeta Micro SD.
  * @param  TCHAR*: "Nombre de la Carpeta a Crear".
  * @retval None
  */
void Crea_Carpeta (const TCHAR* nombre_carpeta_crear)
{
	if(f_mount(&FAT_FS, MicroSD.Raiz_SD, 1) == FR_OK)
	{
		if(f_mkdir(nombre_carpeta_crear) != FR_OK)
		{
			MicroSD.Flags.Error.Crea_Carpeta = 1;
		}
	}else{
			MicroSD.Flags.Error.Monta_Memoria = 1;
	}
}

/**
  * @brief  Función que permite borrar una carpeta de la raiz de la tarjeta Micro SD.
  * @param  TCHAR*: "Nombre de la Carpeta a Borrar".
  * @retval None
  */
void Borra_Carpeta (const TCHAR* nombre_carpeta_borrar)
{
	if(f_mount(&FAT_FS, MicroSD.Raiz_SD, 1) == FR_OK)
	{
		if(f_unlink(nombre_carpeta_borrar) != FR_OK)
		{
			MicroSD.Flags.Error.Borra_Carpeta = 1;
		}
	}else{
			MicroSD.Flags.Error.Monta_Memoria = 1;
	}
}

/**
  * @brief  Función que permite crear un nuevo archivo con acceso lectura y escritura en la raiz de la tarjeta Micro SD
  * 		Si el archivo existe, la función se eliminará y sobrescribirá.
  * 		Imporante! Añadir al final del nombre del archivo su formato.
  * @param  TCHAR*: "Nombre del archivo a Crear" (ej. "DATOS.TXT").
  * @retval None
  */
void Crea_Archivo (const TCHAR* nombre_archivo_crear)
{
	if(f_mount(&FAT_FS, MicroSD.Raiz_SD, 1) == FR_OK)
	{
		if(f_open(&Archivo, nombre_archivo_crear, FA_CREATE_NEW | FA_WRITE | FA_READ) != FR_OK)
		{
			MicroSD.Flags.Archivo_Existente = 1;
		}
			f_close(&Archivo);
	}else{
			MicroSD.Flags.Error.Monta_Memoria = 1;
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
	if(f_mount(&FAT_FS, MicroSD.Raiz_SD, 1) == FR_OK)
	{
		if(f_unlink(nombre_archivo_borrar) != FR_OK)
		{
			MicroSD.Flags.Error.Borra_Carpeta = 1;
		}
	}else{
			MicroSD.Flags.Error.Monta_Memoria = 1;
	}
}

/**
  * @brief Función que permite escribir datos en un archivo de la raiz de la tarjeta Micro SD ya existente.
  * @param TCHAR*: "Nombre del Archivo a Escribir".
  * @param void*: Datos a escribir en archivo
  * @param Tipo_Dato: Tipo de dato a escribir en memoria micro SD (admite enteros, decimales y cadenas de caraceteres)
  * @retval None
  */
void Escribe_Archivo (const TCHAR* nombre_archivo, void *Datos, tTipoDatos Tipo_Dato)
{
	if(f_mount(&FAT_FS, MicroSD.Raiz_SD, 1) == FR_OK)
	{
		if( Datos_String (Datos, Tipo_Dato) == 1)
		{
			if(f_open(&Archivo, nombre_archivo, FA_OPEN_APPEND | FA_WRITE | FA_READ) != FR_OK)
			{
				MicroSD.Flags.Error.Apertura_Archivo = 1;
			}
			if (f_write(&Archivo, &MicroSD.Datos_String, MicroSD.CS.n_bytes, &testByte) != FR_OK)
			{
				MicroSD.Flags.Error.Escritura_Archivo = 1;
			}
			if (f_close(&Archivo) != FR_OK)
			{
				MicroSD.Flags.Error.Cierre_Archivo = 1;
			}
				memset(&MicroSD.CS,   0 , sizeof (MicroSD.CS));									/*!< Borra datos usados en la conversion a string 		 */
				memset(MicroSD.Datos_String, '\0', sizeof (MicroSD.Datos_String));				/*!< Resetea string para tratamiento del siguiente dato  */
		}else{
				MicroSD.Flags.Error.Tipo_de_Dato_Desconocido = 1;
			 }
	}else{
				MicroSD.Flags.Error.Monta_Memoria = 1;
		 }
}

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
