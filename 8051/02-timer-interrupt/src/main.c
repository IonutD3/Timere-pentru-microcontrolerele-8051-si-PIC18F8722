/*
 * 8051 - Timer0 cu intrerupere
 *
 * Spre deosebire de exemplul polling, CPU nu mai asteapta activ
 * overflow-ul timerului; ISR-ul se executa automat.
 */
#include <reg51.h>
#include <stdio.h>

sbit LED = P1^0;
int counter = 0;

void timer0_isr() interrupt 1{ 
// Reincarcam Timer0 la fiecare overflow.
TH0 = 0xFC; 
TL0 = 0x18; 
counter++;  

if(counter  == 1000){
LED =~ LED;   
counter = 0;}    
}

void main(){
    TMOD = 0x01;      
    TR0 = 1;     // Porneste Timer0.      
    ET0 = 1;    // Permite intreruperea Timer0.       
    EA = 1;     // Permite intreruperile globale.
      
    while(1)
    {} // Bucla principala ramane libera.
}