/*
 * PIC18F8722 - Timer0 cu intrerupere
 *
 * Timer0 ruleaza din ceasul intern si, la fiecare overflow,
 * modifica un model afisat pe PORTD. 
 */
#include <xc.h>
#define_XTAL_FREQ 10000000

int counter=0;
char ch[]={0b01001010,0b1111010};

void interrupt t0(){
   if(TMR0IF){
      for(int i=0;i<=2;i++){

	 counter++;
	 if(counter==122){

	    LATD=ch[i];
	    counter=0;}
	 }

	 TMR0IF=0;}
   }

void main(void)
 { // Timer0: 8 biti, ceas intern, prescaler 1:16.
    TRISD=0x0;
    GIE=1;
    T08BIT=1;
    T0CS=0;
    PSA=0;
    T0PS2=1;
    T0PS1=0;
    T0PS0=0;
    TMR0IE=1;
    TMR0IF=0;
    TMR0L=0;
    TMR0ON=1;

   while (1){} // Toata activitatea este facuta in ISR.
      
 }