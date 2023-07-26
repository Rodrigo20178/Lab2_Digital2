/*
 * File:   Lab2.c
 * Author: Rodrigo García
 *
 * Created on 20 de julio de 2023, 03:55 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes
#define _XTAL_FREQ 4000000
#define RS RB0
#define EN RB1

// Librerías
#include "LCD8bits.h"
#include "adc.h"
#include "Map.h"
#include "UART.h"

// Variables
float voltaje;
unsigned char valor[20];
unsigned char valor2[20];
uint8_t pot;
char valorSerial;
uint8_t cont;



// Prototipo de Funciones
void setup(void);

//Función de Interrupciones
void __interrupt() isr(void){
    if(PIR1bits.ADIF){

        
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.RCIF){
        valorSerial = RCREG;
        if(valorSerial == '+'){
            cont+= 1;
            if (cont == 255){
                cont = 0;
            }
        }
        else if (valorSerial == '-'){
            cont-= 1;
            if (cont == 0){
                cont = 255;
            }
        }
    }
        PIR1bits.RCIF = 0;
}


// MAIN
void main(void) {
    setup();
    uartRC_init();
    
    while(1)
    {
        
        if(ADCON0bits.GO == 0)
        {
            pot = ADRESH;
            voltaje = map(pot, 0, 255, 0, 5);
            __delay_us(50);
            ADCON0bits.GO = 1;
        }
       
        
        
        sprintf(valor, "%.2f", voltaje);
        
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Pot");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(valor);
        Lcd_Set_Cursor(2,5);
        Lcd_Write_String("V ");
        
        
        
        sprintf(valor, "%i", cont);
        
        Lcd_Set_Cursor(1,10);
        Lcd_Write_String("Cont");
        Lcd_Set_Cursor(2,11);
        Lcd_Write_String(valor);
    }
    return;
}


// Configuraciones
void setup(void){
    ANSEL = 0x01;
    ANSELH = 0x00;
    
    TRISA = 0x01;
    PORTA = 0;
      
    
    TRISB = 0x00;
    TRISB = 0;
    
    
    TRISD = 0x00;
    PORTD = 0;
    
    //Configuración de oscilador
    OSCCONbits.IRCF = 0b110; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
    // Configuriación ADC
    adc_init(1);
    ADCON0bits.CHS = 0;     
    __delay_us(100);
    ADCON0bits.GO = 1;  
    
    // Configuración LCD
    Lcd_Init();
    
 
    
    // Configuración de Interrupciones
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    PIE1bits.RCIE = 1;          // Habilitamos Interrupciones de recepción
}

