#include <xc.h>
#include "UART.h"

void uartRC_init(void){
    TXSTAbits.SYNC = 0;         // Comunicación ascincrona (full-duplex)
    TXSTAbits.BRGH = 1;         // Baud rate de alta velocidad 
    BAUDCTLbits.BRG16 = 0;      // 16-bits para generar el baud rate
    
    SPBRG = 25;
    SPBRGH = 0;                 // Baud rate ~9600, error -> 0.16%
    
    RCSTAbits.SPEN = 1;         // Habilitamos comunicación
    TXSTAbits.TX9 = 0;          // Utilizamos solo 8 bits
    TXSTAbits.TXEN = 1;         // Habilitamos transmisor
    RCSTAbits.CREN = 1;         // Habilitamos receptor
}


char uartRC_Read(void){
    return RCREG;
}

void uartTX_Write(char selection){
    while (TXSTAbits.TRMT == 0);
    TXREG = selection;
}

void uartTX_Write_Str(unsigned char *word){
    while (*word != '\0')
    {
        while (TXIF != 1);
        TXREG = *word;
        *word++;
    }
    return;    
}
