/*
 * Init_MicroSD.h
 *
 *  Created on: 3 mar. 2022
 *      Author: Juan Galbis Domènech
 */

#ifndef INC_INIT_MICROSD_H_
#define INC_INIT_MICROSD_H_

#include "ff.h"

#define   LONGIUD_MAX_STRING 	20
#define	  MAX_DECIMALES	     	1000 			/*!< 1000 = 3 DECIMALES, 100 = 2 DECIMALES ...*/



/**
************************************************************************************
\typedef        tFlagsMicroSD
\brief          Tipo definido para guardado de los flags de error generados durante
				la gestión de la tarjeta micro SD
\struct         tFlagsMicroSD
\brief          Estructura para guardado de los flags de error generados durante
				la gestión de la tarjeta micro SD
**********************************************************************************/
typedef struct sFlagsMicroSD{

	struct
	{
		uint8_t  Monta_Memoria:1;
		uint8_t  Crea_Carpeta:1;
		uint8_t  Archivo_Existente:1;
		uint8_t  Borra_Carpeta:1;
		uint8_t  Borra_Archivo:1;
		uint8_t  Escritura_Archivo:1;
		uint8_t  Apertura_Archivo:1;
		uint8_t	 Cierre_Archivo:1;
		uint8_t  Tipo_de_Dato_Desconocido:1;
	} Error;

}tFlagsMicroSD;

typedef struct sConvString{

	float   Parte_Decimal;
	int 	Parte_Entera;
	float 	Dato_Decimal;
	int 	Parte_Decimal_Entera;
	int  	Dato_Entero;
	uint8_t estado;

}tConvString;


/**
*******************************************************************************
\typedef        tTipoDatos
\brief          Tipo definido para la identificación de los distintos tipos de
				dato admisibles en la función Escribe_Archivo
\enum           tTipoDatos
\brief          Enumerado definido para la identificación de los distintos
				tipos de dato admisibles en la función Escribe_Archivo
******************************************************************************/
typedef enum eTipoDatos{

	Entero  		  = 0,
	Decimal 		  = 1,
	Cadena_Caracteres = 2,
	Desconocido		  = 3

}tTipoDatos;

typedef struct sMicroSD{

	char Raiz_SD [2];
	char Datos_String [LONGIUD_MAX_STRING];
	tFlagsMicroSD Flags;
	tConvString	  CS;

}tMicroSD;


void 	SDIO_SD_Init 	 (void);
void 	Init_MicroSD 	 (void);

void 	Crea_Carpeta 	 (const TCHAR* nombre_carpeta_crear);									//OK
void 	Borra_Carpeta	 (const TCHAR* nombre_carpeta_borrar);									//OK
void 	Crea_Archivo 	 (const TCHAR* nombre_archivo_crear);									//OK
void 	Borra_Archivo	 (const TCHAR* nombre_archivo_borrar);									//OK
uint8_t Datos_String 	 (void *datos, tTipoDatos Tipo_Dato);
void	Escribe_Archivo  (const TCHAR* nombre_archivo, void *Datos, tTipoDatos Tipo_Dato);

#endif /* INC_INIT_MICROSD_H_ */
