/*
 * Init_MicroSD.h
 *
 *  Created on: 3 mar. 2022
 *      Author: jugaldo
 */

#ifndef INC_INIT_MICROSD_H_
#define INC_INIT_MICROSD_H_

#include "ff.h"

typedef struct sMicroSD{

    union
        {
        uint8_t All;
        struct
        {
        	uint8_t  Monta_Memoria:1;
        	uint8_t  Crea_Carpeta:1;
        	uint8_t  Crea_Archivo:1;
        	uint8_t  Borra_Carpeta:1;
        	uint8_t  Borra_Archivo:1;
        	uint8_t  Escritura_Archivo:1;
        	uint8_t  Apertura_Archivo:1;
        	uint8_t	 Cierre_Archivo:1;
        } Error;
    }Flags;

}sMicroSD;

void SDIO_SD_Init 	 (void);
void Init_MicroSD 	 (void);

void Datos_Char 	 (void *datos);

void Crea_Carpeta 	 (const TCHAR* nombre_carpeta_crear);				//OK
void Borra_Carpeta	 (const TCHAR* nombre_carpeta_borrar);				//OK
void Crea_Archivo 	 (const TCHAR* nombre_archivo_crear);				//OK
void Borra_Archivo	 (const TCHAR* nombre_archivo_borrar);				//OK
void Escribe_Archivo (const TCHAR* nombre_archivo, void *datos);





#endif /* INC_INIT_MICROSD_H_ */
