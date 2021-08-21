/*
 * File:   lab5.c
 * Author: Guille Lam

 */

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  
#include <string.h> 
#include <stdlib.h>

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//Variables 
int cuenta;
char centenas, decenas, unidades, valor, centenas1, residuo, decenas1, unidades1;

//Prototipos 
void setup(void);
void Text(void);
char division (char valor);

//Interrupciones
void __interrupt() isr(void)
{
    if (RBIF == 1)  
    {
        if (PORTBbits.RB0 == 0) 
        {cuenta = cuenta + 1;}
        if  (PORTBbits.RB1 == 0)    
        {cuenta = cuenta - 1;}
        INTCONbits.RBIF = 0;}}

//Main
void main(void) {
    setup();    
    while(1)   
    {
        PORTA = cuenta;
        Text();
        while(RCIF == 0);
        centenas = RCREG -48;
        while(RCIF == 0);
        decenas = RCREG -48;
        while(RCIF == 0);
        unidades = RCREG -48;
        PORTD = ((centenas*100)+(decenas*10)+ unidades);
    }
    return;}


//Funciones
void setup(void){
    
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISD = 0;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    OSCCONbits.IRCF0 = 0;        
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  
    
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;

    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    
    SPBRG = 25;
    SPBRGH = 1;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    TXSTAbits.TX9 = 0;        
    
    TXSTAbits.TXEN = 1;
    
    PIR1bits.RCIF = 0; 
    PIR1bits.TXIF = 0;  
    
    INTCONbits.GIE = 1;     
    INTCONbits.PEIE = 1;    
    PIE1bits.RCIE = 0;      
    PIE1bits.TXIE = 0;      
    INTCONbits.RBIF = 1;    
    INTCONbits.RBIE = 1;}

void Text(void){
    __delay_ms(250); 
     division(cuenta);
    if (RCREG == 'a'){              
     __delay_ms(50);
    if(TXIF == 1){
        TXREG = centenas1; 
    }
    __delay_ms(50);
    if(TXIF == 1){
        TXREG = decenas1; 
       }
    __delay_ms(50);
    if(TXIF == 1){
        TXREG = unidades1; 
       }
    __delay_ms(50);}}

char division (char valor){
    centenas1 = valor/100;
    residuo = valor%100; 
    decenas1 = residuo/10; 
    unidades1 = residuo%10; 
    centenas1 = centenas1 + 48;
    decenas1 = decenas1 + 48;
    unidades1 = unidades1 + 48;} 

int concat(int a, int b)
{
    char s1[20];
    char s2[20];
    sprintf(s1, "%d", a);
    sprintf(s2, "%d", b);
    strcat(s1, s2);
    int c = atoi(s1);
    return c;}