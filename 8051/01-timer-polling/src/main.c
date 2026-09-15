/*
 * 8051 - Timer0 in mod polling
 *
 * Timer0 genereaza o intarziere prin overflow, iar programul principal
 * verifica flag-ul TF0. LED-ul este pe P1.0.
 */
#include <reg51.h>
#include <stdio.h>

sbit LED = P1^0;
       
void del(){ 
TH0 = 0xFC; 
TL0 = 0x18; 
TR0 = 1;

while(!TF0);
TR0 = 0;
TF0 = 0;}

void main(void){
int i;

while(1){
TMOD = 0x01;
LED =~LED; 

for (i=0;i<=1000;i++){
del();}
}
}
