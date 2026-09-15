/*
 * 8051 - Doua timere cu intrerupere
 *
 * Timer0 comuta LED-ul de pe P1.0, iar Timer1 comuta LED-ul de pe P1.1.
 */
#include <reg51.h>
#include <stdio.h>

sbit LED = P1^0;
sbit LED1 = P1^1;
int counter = 0; 
int counter1 = 0;

void timer0_isr() interrupt 1{ 
TH0 = 0xFC; 
TL0 = 0x18; 
counter++; 
 
if(counter  == 1000){
LED =~ LED;   
counter = 0;}    
}

void timer1_isr() interrupt 3{ 
TH1 = 0xFC; 
TL1 = 0x18; 
counter1++;
  
if(counter1  == 1000){
LED1 =~ LED1;   
counter1 = 0;}    
}

void main(){
    TMOD = 0x01;      
    TR0 = 1;           
    //ET0 = 1;     
    TR1 = 1;   
    // ET1 =1;        
   // EA = 1;       
    IE = 0x8a; 
   
    while(1)
    {} // Cele doua timere lucreaza independent.
}