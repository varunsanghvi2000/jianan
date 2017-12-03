/* Name: main.c
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"
#include <math.h>

int count;
int count2;


int main(void)
{
	teensy_clockdivide(0);	//set system clock to 16 MHz
	
	set(DDRD, 7);		//set D6 as output
	set(DDRC, 6);		//set C6 as output
	set(DDRB, 4);
	set(DDRB, 5);
	set(DDRF, 0);
	set(DDRF, 1);
	
// 	__Timer 4__
// 		
// 		clear(TCCR4B, CS43);
// 		clear(TCCR4B, CS42);
// 		clear(TCCR4B, CS41);	//turn on clock source for timer 0 16MHz
// 		set(TCCR4B, CS40);
// 			
// 		clear(TCCR4D, WGM41);	//pwm mode, up to OCR4C
// 		clear(TCCR4D, WGM40);
// 			
// 		set(TCCR4C, PWM4D);
// 		set(TCCR4C, COM4D1);	//clear D7 at OCR4D, set at rollover (255)
// 		clear(TCCR4C, COM4D0);
// 		
// 		//__Timer 3__
// 		
// 		clear(TCCR3B, CS32);
// 		clear(TCCR3B, CS31);
// 		set(TCCR3B, CS30);	//turn on clock source for timer 3 16MHz
// 		
// 		clear(TCCR3B, WGM33);	//timer 3 mode 5, reset at rollover (8-bit = 255)
// 		set(TCCR3B, WGM32);	
// 		clear(TCCR3A, WGM31);
// 		set(TCCR3A, WGM30);
// 		
// 		clear(TCCR3A, COM3A0);	//clear C6 at OCR3A, set at rollover (255)
// 		set(TCCR3A, COM3A1);
// 		
// 		m_usb_init();	//initialize the USB subsystem
// 		while(!m_usb_isconnected());	//wait for connection
	
	//motor 1 direction
	set(PORTD, 7);
	set(PORTB, 4);
	clear(PORTB, 5);
	
	
	//motor 2 direction
	set(PORTC, 6);
	set(PORTF, 0);
	clear(PORTF, 1);
	
	 
for(;;){
	
	
	
// 	for (count=120; count<255; count++){
// 		OCR4D = count;		//ramp up motor 1 forward
// 		OCR3A = 375-count;	//ramp down motor 2 backward
// 		teensy_wait(25);	//ms*135 ~= total time for cycle
// 		
// 	}
// 	for (count2=255; count2>120; count2--){
// 		OCR4D = count2;		//ramp down motor 1 forward
// 		OCR3A = 375-count2;	//ramp up motor 2 backward
// 		teensy_wait(25);	//ms*136 ~= total time for cycle
// 		
// 	}
// 	
// 	OCR4D = 0;		//stop motor 1
// 	OCR3A = 0;		//stop motor 2
// 	teensy_wait(1000);	//delay 1 sec 
	
}

    return 0;   /* never reached */
}