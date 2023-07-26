/* 
 * File:   UART.h
 * Author: Rodrigo García
 *
 * Created on 23 de julio de 2023, 10:35 PM
 */

#ifndef UART_H
#define	UART_H


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
void uartRC_init(void);
char uartRC_Read(void);
void uartTX_Write(char selection);
void uartTX_Write_Str(unsigned char *word);

#endif	/* UART_H */

