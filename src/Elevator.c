#include <hidef.h>      /* common defines and macros */#include <MC9S12C128.h>#pragma LINK_INFO DERIVATIVE "mc9s12c128"#include "stdio.h"#include "termio.h"void moveUp(int n);            //method declarationvoid moveDown(int n);			//method declarationint rti_cnt;			//counts the number of real time interruptsint current_floor;	//keeps track of the floor which the elevator is onint direction;        //records the direction in which the elevator should move to reach its destination int destination;		//the floor to which the elevator should moveint floor_difference; //the number of floors between where the elevator is and where it should be  void interrupt 7 RTI_ISR(){CRGFLG &= 0x80; /* clear RTIF flag *///printf("here");if (rti_cnt == 0){//if this is the 250th time the real time interrupt is being called  if (PORTA == 0x01) {//if ‘A’ is pressed on the keypad   destination = 1;  //change the destination to floor 1   printf("A"); } if (PORTA == 0x02) {//if ‘B’ is pressed on the keypad   destination = 2;  //change the destination to floor 2   printf("B"); } if (PORTA == 0x04){//if ‘C’ is pressed on the keypad   destination = 3; //change the destination to floor 3   printf("C"); } if (PORTA == 0x08){//if ‘D’ is pressed on the keypad   destination = 4; //change the destination to floor 4   printf("D"); } if (PORTA == 0x00){//if no keys on the keypad are pressed     destination = current_floor;//change destination to the floor that the elevator is currently on   printf("^"); } if (destination > current_floor) {//if the destination is above the floor that the elevator is currently on     direction = 1;//indicates that the elevator should travel in the upward direction    floor_difference = destination - current_floor;//gives the number of floors that the elevator needs to move in the appropriate direction  } else if(destination < current_floor){//otherwise if the destination is below the floor that the elevator is currently on    direction = 0;//indicates that the elevator needs to travel in the downward direction     floor_difference = current_floor - destination;//gives the number of floors that the elevator needs to move in the appropriate direction } else {               direction = 2;//indicates that the elevator does not need to move    floor_difference = 0;//same as above }printf(" %d ",current_floor);printf(" %d ",destination); if (direction == 1) {//if the elevator is supposed to move in the upward direction   moveUp(floor_difference);//call the method moveUp to move the elevator ’floor_difference’ floors up } else if(direction == 0){//otherwise if the elevator is supposed to move in the downward direction   moveDown(floor_difference);//call the method moveDown to move the elevator ‘floor_difference’ floors down } current_floor = destination;//update the program to reflect that the elevator has reached the destination floorrti_cnt = 250; //update the counter which counts the number of real time interrupts}else{//if this isn’t the 250th time that the interrupt is being called rti_cnt--;//decrement the counter that indicates the number of real time interrupts left till we check the keypad for input again }}void main(){ rti_cnt = 250;     //set the initial value that affects how often we check the keypad for input current_floor = 1;//set an initial value assuming that the elevator starts out on floor 1 direction = 1;//set a default value of ‘up’ since if the elevator starts out on floor 1 it can’t move downward destination = 1;//sets a default value floor_difference = 0;//sets a default value TERMIO_Init(); DDRB = 0xFF; //makes port B an output port DDRA = 0x00; //makes port A an input port //PORTB = 0xF0;   //f0 TSCR1 = 0x80; /* enable clock timer */  //PWM FOR DC MOTOR PWMCLK = 0x00;/*set clock A as source*/ PWMPOL=1;          /*make waveform start with high level*/ PWMCTL=0x0C;       /*select 8 bit mode*/ PWMPRCLK=2;       /*set the clock A prescaler*/ PWMCAE=1;    /*select center aligned mode*/ PWMPER0 = 250;  /*Set period of PWM0*/ PWME|=0x01;/*Enable PWMO channel*/ PWMDTY0= 0;  /*set the duty cycle*/         //125 //while(1); //EnableInterrupts; //PWM FOR SERVO PWMPOL |= 0x04;  //make waveform start with a high level PWMPRCLK |= 0x70;//set the clock B prescaler PWMCAE |= 0x04;  //select the center aligned mode for the PWM signal  PWMPER2 = 156;   //set the period for the signal of PWM2 (this value creates a time period of about 20 ms)   PWME |= 0x04;    //enable PWM2 channel PWMDTY2 = 16;    //set the duty cycle of the waveform (approx 10%)  //REAL TIME INTERRUPT RTICTL= 0x40; //set RTI clock prescale factor to 2^13 CRGFLG &= 0x80;//clear flag CRGINT = 0x80;//enable Real Time Interrupt EnableInterrupts; while(1) {}//infinite loop}void moveUp(int n){ //function moveUp int k = n; long i = 168000; PORTB = 0x0F; //sets the motor to move in a particular direction so that the elevator moves upward PWMDTY0 = 125;//sets the duty for the PWM signal going to the enable pin of the H-bridge while(k>0){ //waits for some time before turning the motor off   while(i>=0){i--;}   k--;   i = 168000; } PWMDTY0 = 0; //turns the motor off}void moveDown(int m){ //function moveDown int t = m; long i = 102000; PORTB = 0xF0; //sets the motor to move in a particular direction so that the elevator moves downward PWMDTY0 = 125;//sets the duty for the PWM signal going to the enable pin of the H-bridge while(t>0){ //waits for some time before turning the motor off   while(i>=0){i--;}   t--;   i = 102000; } PWMDTY0 = 0; //turns the motor off}