// -----------------------------------------------------------------------------
// Liad Yamin Macros
// version: 1
// date: September 19, 2017
// author: Liad Yamin
// -----------------------------------------------------------------------------
#ifndef teensy_general__
#define teensy_general__

// -----------------------------------------------------------------------------
// Useful pre-compile constants
// -----------------------------------------------------------------------------

#define TRUE	1
#define FALSE	0

#define OFF		0
#define ON		1
#define TOGGLE	2
#define DUTYCYCLE 10

// -----------------------------------------------------------------------------
// General AVR libraries:
// -----------------------------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "teensy_general.h"
#include "t_usb.h"

// -----------------------------------------------------------------------------
// PWM for PIN C6
// -----------------------------------------------------------------------------

void PWM(int DUTYCYCLE)
{
set(DDRC, 6);	//set C6 as output
set(TCCR3B, CS30);	//turn on clock source for timer 0 16MHz
set(TCCR3A, WGM31);	//
set(TCCR3A, WGM30);	//timer 0, up to 0x03FF (1023) PWM mode
set(TCCR3A, COM3A1);	//clears C6 at OCR3A, set C6 at rollover (1023)
OCR3A = 1023*DUTYCYCLE/100;	//
}
