/* Name: main.c
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"
#include <math.h>

int freq, prev_val;
unsigned int count, count2, prev, current;

int main(void)
{
	teensy_clockdivide(0);	//set system clock to 16 MHz
	teensy_disableJTAG();	//when using F4-7, must disable JTAG
	
	//Set inputs & outputs
	
	set(DDRD, 7);		//motor 1 PWM
	set(DDRB, 4);		//motor 1 In1
	set(DDRB, 5);		//motor 1 In2
	
	set(DDRD, 0);		//motor 2 PWM
	set(DDRF, 0);		//motor 2 In1
	set(DDRF, 1);		//motor 2 In2
	
	set(DDRB, 6);		//servo pin output
	clear(DDRC, 7);		//"hit" button input pin
	clear(DDRF, 4);		//"hit" button input pin
	
	clear(DDRF, 7);		//healing sensor pin input
	clear(PORTF, 7);
	
	set(DDRD, 6);		//onboard LED pin
	
	//__Timer 4__Motor1
		
	clear(TCCR4B, CS43);
	clear(TCCR4B, CS42);
	clear(TCCR4B, CS41);	//turn on clock source 16MHz
	set(TCCR4B, CS40);
			
	clear(TCCR4D, WGM41);	// UP mode (8 bit = 255) to OCR4C
	clear(TCCR4D, WGM40);
			
	set(TCCR4C, PWM4D);
	set(TCCR4C, COM4D1);	//clear D7 at OCR4D, set at rollover (255)
	clear(TCCR4C, COM4D0);
	
	OCR4C = 255;	//set top value for Timer 4
		
	//__Timer 0__Motor2
	
	clear(TCCR0B, CS02);	//turn on clock source 16MHz
	clear(TCCR0B, CS01);
	set(TCCR0B, CS00);
		
	clear(TCCR0B, WGM02);		//UP to 255
	set(TCCR0A, WGM01);
	set(TCCR0A, WGM00);
	
	clear(TCCR0A, COM0B0);	//clear D0 at OCR0B, set at rollover (255)
	set(TCCR0A, COM0B1);
	
	//__Timer 3__Frequency
		
	clear(TCCR3B, CS30);
	clear(TCCR3B, CS31);
	set(TCCR3B, CS32);		//turn on clock source 16MHz/256 = 62500
	
	clear(TCCR3B, WGM33);	//mode 4, up to OCR3A
	set(TCCR3B, WGM32);
	clear(TCCR3A, WGM31);
	clear(TCCR3A, WGM30);
	
	OCR3A = 62500;	//set max counter value, so 1 count = 1 Hz
	
	clear(TCCR3A, COM3A0);	//no change to pin C6
	clear(TCCR3A, COM3A1);
	
	//__Timer 1__Servo
	
	clear(TCCR1B, CS12);	//turn on clock source 2MHz
	set(TCCR1B, CS11);
	clear(TCCR1B, CS10);	
	
	set(TCCR1B, WGM13);	//mode 15, UP to OCR1A (16-bit = 65536)
	set(TCCR1B, WGM12);
	set(TCCR1A, WGM11);
	set(TCCR1A, WGM10);
	
	OCR1A = 40000;	//40,000/2,000,000 = 20ms per cycle
	
	clear(TCCR1A, COM1B0);	//clear B6 at OCR1B, set at rollover (255)
	set(TCCR1A, COM1B1);
		
// 		m_usb_init();	//initialize the USB subsystem
// 		while(!m_usb_isconnected());	//wait for connection
	 
for(;;){

	OCR4D = 100;		//motor 1 PWM value (255 max)
	set(PORTB, 4);		//motor 1 direction
	clear(PORTB, 5);
	
	OCR0B = 100;		//motor 2 PWM value (255 max)
	set(PORTF, 0);		//motor 2 direction
	clear(PORTF, 1);
	
	//OCR1B = 1100;	//servo
					//min 1100 (~.55ms pulse), max 4900 (~2.45 ms pulse)
	//teensy_wait(1000);
	
// 	OCR1B = 3000;
// 	teensy_wait(1000);
	
	OCR1B = 4900;
	//teensy_wait(1000);
		
		//calculate frequency of healing LED
		
		if(bit_is_clear(PINF, 7)){
			count = count + 1;	//increment count
			if (count2 > 0){
				current = TCNT3;	//set counter value to current
				count2 = 0;			//reset count2
				
				freq = 1/(2*.000016*(current - prev));	//calculate frequency using difference in counter value and 1/clock frequency
				
				m_usb_tx_string("frequency = ");	//print statements
				m_usb_tx_uint(freq);
				m_usb_tx_string("\n");
				
				TCNT3 = 0;		//reset counter value
			}
		}
		if(bit_is_set(PINF, 7)){
			count2 = count2 + 1;	//increment count2
			if (count > 0){
				prev = TCNT3;	//set counter value to prev
				count = 0;		//reset count
			}
		}

		if(freq>200 && freq<400)
		{
			set(PORTD,6);	//turn on onboard LED
		}
		
		else
		{
			clear(PORTD,6);
		}
				
}

    return 0;   /* never reached */
}