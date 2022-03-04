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
        	uint8_t  Crea_Carpeta:1;
        	uint8_t  Borra_Carpeta:1;
        	uint8_t  Reservado:8;
        } Error;
    }Flags;

}sMicroSD;

void SDIO_SD_Init (void);
void Init_MicroSD (void);
void Test_1 	  (void);								//Escritura En archivo
void Crea_Carpeta (const TCHAR* nombre_carpeta);




#endif /* INC_INIT_MICROSD_H_ */
