/* Name: 1.4.2
/* Author: Liad Yamin
*/

#include "teensy_general.h"
#include "t_usb.h"

int main()
{
    int i=0,prev_val=0;
	m_usb_init();
	while(!m_usb_isconnected()); // wait for a connection
    
    

 
    // -----------------------------------------------------------------------------
    // PIns initialzation
    // -----------------------------------------------------------------------------

    
    clear(DDRF,0);                 // set F0 pin as an input
	clear(PORTF,0);
    set(DDRD,1);                    // set D1 pin as an output
    clear(PORTD,1);
    
    
    // -----------------------------------------------------------------------------
    // set system clock and timer
    // -----------------------------------------------------------------------------

    
    teensy_clockdivide(0);              //Set initialize system clock no prescaler
    set(TCCR1B,CS12);                   //256 prescaler 62500 hz
    set(TCCR1B,WGM12);                  //up to OCR1A
    OCR1A=62500;
  
    // -----------------------------------------------------------------------------
    //  calculates the imput signal frequency
    // -----------------------------------------------------------------------------

    
    while (1)
	{
        i=0;                            //zero the counter
        while(TCNT1<OCR1A)
        {
        if(PINF & (1<<PINF0))           //if signal is high
        {
            if(prev_val==0)             // compare to previous value
            {
                i++;                    // add counter
                prev_val=1;             // set prvious value to high
            }
        }
        
        else
        {
            prev_val=0;                 // set prvious value to low
        }
        set(ADCSRA,ADIF);				// reset adc bit
        }
     m_usb_tx_string("the frequncy is:");
     m_usb_tx_uint(i);
     m_usb_tx_char(10);
     m_usb_tx_char(13);
       
        
        // -----------------------------------------------------------------------------
        // set thersholds for high and low frequencies
        // -----------------------------------------------------------------------------

        
        if(i>200 && i<400)
        {
            set(PORTD,1);           //Turn on neopixel

        }
        
        else
        {
            clear(PORTD,0);
            clear(PORTD,1);
        }
   
    }
}

