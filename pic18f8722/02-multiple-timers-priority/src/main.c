/*
 * PIC18F8722 - Timer0, Timer1 si Timer2 cu prioritati
 *
 * Timer0 si Timer2 au prioritate HIGH, iar Timer1 prioritate LOW.
 */
#include <xc.h>
#define _XTAL_FREQ 4000000

void ext0();
void ext1();
void ext2();
void ext3();
void ext4();
void ext5();
int counter = 0;
int counter1 = 0;
int counter2 = 0;

void interrupt high_priority ex1(){
if (INT0IF){
ext0();}
if (TMR0IF){
ext2();}
if (TMR2IF){
ext4();}
}

void interrupt low_priority ex2()
{
if (INT1IF)
{ext1();}
if (TMR1IF)
{ext3();}
if (INT2IF)
{ext5();}
}

void ext0(){
for (int i=0;i<=2;i++){
RD0 = ~RD0;
for (int i=0;i<=100;i++)
__delay_ms(5);}
INT0IF = 0;}

void ext1(){
for (int i=0;i<=2;i++)
{RD1 = ~RD1;
for (int i=0;i<=100;i++)
__delay_ms(5);}
INT1IF = 0;}

void ext5(){
for (int i=0;i<=5;i++){
RD5= ~RD5;
for (int i=1;i<=100;i++){
__delay_ms(5);}
}
INT2IF = 0;}

void ext2(){
counter++;
if (counter == 8){
RD2 = ~RD2;
counter = 0;}
TMR0IF = 0;}

void ext3(){
counter1++;
if (counter1 == 1){
RD3 = ~RD3;
counter1 = 0;}
TMR1IF = 0;}

void ext4(){
counter2++;
if (counter2 == 256){
RD4 = ~RD4;
counter2 = 0;}
TMR2IF = 0;}

void main(void){
   LATD = 0x0;
  TRISD = 0x0;

//Activare intreruperi externe 0, 1
  GIEH = 1;
  GIEL = 1;
  IPEN = 1;
  INT0IE = 1;
  INT1IE = 1;
  INT2IE = 1;
  INT0IF = 0;
  INT1IF = 0;
  INT2IF = 0; 
  INTEDG0 = 1;
  INTEDG1 = 1;
  INTEDG2 = 1;
  INT1IP = 0;  //Nivel low
  INT2IP = 0; 

 //Configurare timer0 - 8 biti, clock intern, prescaler 1:32.
  T08BIT = 1;
  T0CS = 0;
  PSA = 0;
  T0PS2 = 1;
  T0PS1 = 1;
  T0PS0 = 0;
  TMR0IE = 1;
  TMR0IF = 0;
  TMR0IP = 1; // Nivel high
  TMR0L = 0; 
  TMR0ON = 1; 

//Configurare timer1 - clock intern, prescaler 1:4, prioritate LOW.
  T1CONbits.RD16 = 0;
  T1CKPS1 = 1; 
  T1CKPS0 = 0;
  TMR1CS = 0;
  TMR1IE = 1;
  TMR1IF = 0;
  TMR1IP = 0; //Nivel low
  TMR1L = 0;
  TMR1H = 0;
  TMR1ON = 1; 

  //Configurare timer2 - prescaler 1:4, prioritate HIGH.
  T2CKPS1 = 0;
  T2CKPS0 = 1;
  TMR2IE = 1;
  TMR2IF = 0;
  TMR2IP = 1; //Nivel high
  TMR2 = 0;
  PR2 = 255; 
  TMR2ON = 1;
   while (1)
   {} // MCU-ul asteapta intreruperile.
      
 }