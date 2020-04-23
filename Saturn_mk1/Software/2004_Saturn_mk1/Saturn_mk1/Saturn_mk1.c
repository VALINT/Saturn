//***********************************************************************************************************************
//	  ________      _    _________   _      _   ______    _    _			 _      _   _   _         ___
//	 / ______/     / \  |___   ___| | |    | | |  __  \  | \  | |			| \    / | | | / /       /   |
//   | |_____     / _ \     | |		| |    | | | |__| |  |  \ | |			|  \  /  | | |/ /		/ /| |
//   \______ \   / /_\ \    | |     | |    | | |  __  /  |   \| |			| \ \/ / | |   |       /_/ | |
//    ______| | / _____ \   | |		|  \__/  | | |  \ \  | |\   |  _______  | |\__/| | | |\ \   _    __| |__
//	 /_______/ /_/     \_\  |_|      \______/  |_|   \_\ |_| \__| |_______| |_|    |_| |_| \_\ |_|  |_______|
//	
//***********************************************************************************************************************
// Created: 15.04.2020 22:49:59
//  Author: VAL
//***********************************************************************************************************************
#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

//***********************************************************************************************************************
//	User defined parameters
//***********************************************************************************************************************
//#define USE_COMMON_ANODE
#define USE_COMMON_CATHODE
//#define USE_SOLID_DISPLAY
#define USE_COMBINED_DISPLAY

//***********************************************************************************************************************
//	System parameters
//***********************************************************************************************************************
#define ButtonDebounce	3
#define ButtonHoldCond  1000
#define FSM_SHOW_TIME	0x00
#define FSM_SHOW_SEC	0x01
#define FSM_SET_HOUR	0x10
#define FSM_SET_MIN		0x11
#define FSM_CLR_SEC		0x12

#define OK_BUTTON		1
#define UP_BUTTON		0
#define DOWN_BUTTON		2


#ifdef USE_COMMON_ANODE 
	#define XOR_OUT 0xFF
#endif
#ifdef USE_COMMON_CATHODE 
	#define XOR_OUT 0x00
#endif

//***********************************************************************************************************************
//	Program memory constants
//***********************************************************************************************************************
const uint8_t digits[11] PROGMEM = {0x00, 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
//									nop   0     1     2     3     4     5     6     7     8     9

//***********************************************************************************************************************
//	Program variables
//***********************************************************************************************************************
typedef enum State{free, pushed, click, hold, waste} State;	// Button states list

typedef struct Button										// Button structure
{
	// uint8_t  buttonDebounce x;							// Defined above as constant for optimisation of memory using
	// uint16_t buttonHoldCond y;							// Defined above as constant for optimisation of memory using
	State	 state;
	uint16_t hold_timer;
} Button;

Button	up			= {free, 0};
Button	ok			= {free, 0};
Button	down		= {free, 0};
	
volatile uint8_t 
		subSec		= 1,									// Need for any simple animations. Like dots blinking.
		sec			= 0,
		min			= 0,
		hour		= 0,
		decTimer	= 0;									// Additional leave condition from menu.

int8_t	modHour		= 0,									// Additional variables for time modification.
		modMin		= 0,
		modSec		= 0;
		
uint8_t	fsmState	= 0,
		fsmTrig		= 0;

uint8_t currDigit	= 0;

uint8_t buffer;

//***********************************************************************************************************************
//	Interrupts handler
//***********************************************************************************************************************
ISR(TIMER0_OVF_vect) //Time displaying and buttons processing 488 times per second
{
	//Display
	switch(fsmState)
	{
		case FSM_SHOW_TIME:
			if(currDigit == 0)		buffer = (1 + hour/10) * (hour/10 != 0);	// Do not show first digit if they equal 0
			else if(currDigit == 1) buffer = (1 + hour%10);						// 
			else if(currDigit == 2) buffer = (1 + min/10);						//
			else if(currDigit == 3) buffer = (1 + min%10);						//
		break;
		case FSM_SHOW_SEC:
			if(currDigit == 0)		buffer = (1 + min/10);						// Minutes
			else if(currDigit == 1) buffer = (1 + min%10);
			else if(currDigit == 2) buffer = (1 + sec/10);						// Seconds
			else if(currDigit == 3) buffer = (1 + sec%10);
		break;
		case FSM_SET_HOUR:
			if(currDigit == 0)		buffer = (uint8_t)(subSec%4 != 0) * (1 + hour/10) * (hour/10 != 0); // Blinking
			else if(currDigit == 1) buffer = (uint8_t)(subSec%4 != 0) * (1 + hour%10);
			else if(currDigit == 2) buffer = (1 + min/10);
			else if(currDigit == 3) buffer = (1 + min%10);
		break;
		case FSM_SET_MIN:
			if(currDigit == 0)		buffer = (1 + hour/10);
			else if(currDigit == 1) buffer = (1 + hour%10);
			else if(currDigit == 2) buffer = (uint8_t)(subSec%4 != 0) * (1 + min/10);		// Blinking
			else if(currDigit == 3) buffer = (uint8_t)(subSec%4 != 0) * (1 + min%10);
		break;
		case FSM_CLR_SEC:
			if(currDigit == 0)		buffer = 0;
			else if(currDigit == 1) buffer = 0;
			else if(currDigit == 2) buffer =  (uint8_t)(subSec%4 < 2) * (1 + sec/10);		// Blinking
			else if(currDigit == 3) buffer =  (uint8_t)(subSec%4 < 2) * (1 + sec%10);
		break;
	}
	
	PORTB &= 0xF0;
	
	#ifdef USE_SOLID_DISPLAY
		PORTD =  XOR_OUT ^ ( ((subSec & 0x08) << 4) * ((currDigit == 1)) | pgm_read_byte(&(digits[buffer])) );
	#endif
	
	#ifdef USE_COMBINED_DISPLAY
		PORTD =  XOR_OUT ^ ( ((subSec & 0x08) << 4) * ((currDigit == 1)  || (currDigit == 2)) | pgm_read_byte(&(digits[buffer])) ); 
	#endif
	PORTB |= (1 << (currDigit));
	
	currDigit++;
	currDigit &= 0x03;
	
	// Check buttons
	if(!(PINC & (1 << UP_BUTTON)))
	{
		if(up.hold_timer < ButtonHoldCond)
		{
			up.hold_timer++;
			if(up.hold_timer >= ButtonDebounce) up.state = pushed;
		}
		else if(up.hold_timer == ButtonHoldCond && up.state != waste) up.state = hold;
	}
	else
	{
		if(up.hold_timer >= ButtonDebounce && up.hold_timer < ButtonHoldCond) up.state = click;
		else if(up.state != click) up.state = free;
		up.hold_timer = 0;
	}
	
	if(!(PINC & (1 << OK_BUTTON)))
	{
		if(ok.hold_timer < ButtonHoldCond)
		{
			ok.hold_timer++;
			if(ok.hold_timer >= ButtonDebounce) ok.state = pushed;
		}
		else if(ok.hold_timer == ButtonHoldCond && ok.state != waste) ok.state = hold;
	}
	else
	{
		if(ok.hold_timer >= ButtonDebounce && ok.hold_timer < ButtonHoldCond) ok.state = click;
		else if(ok.state != click) ok.state = free;
		ok.hold_timer = 0;
	}
	
	if(!(PINC & (1 << DOWN_BUTTON)))
	{
		if(down.hold_timer < ButtonHoldCond)
		{
			down.hold_timer++;
			if(down.hold_timer >= ButtonDebounce) down.state = pushed;
		}
		else if(down.hold_timer == ButtonHoldCond && down.state != waste) down.state = hold;
	}
	else
	{
		if(down.hold_timer >= ButtonDebounce && down.hold_timer < ButtonHoldCond) down.state = click;
		else if(down.state != click) down.state = free;
		down.hold_timer = 0;
	}
}

ISR(TIMER2_OVF_vect)									// Watch crystal 32768Hz / 32768/256/8 = 16 times per second
{
	asm("cli");											// Disable global interrupts	
	subSec ++;											// Increment SubSeconds counter 
	subSec &= 0x0F;										// Program counter from 0 to 15 (16 positions)
	modSec = (!subSec);									// Every time when subSec == 0 increment the seconds
	fsmTrig = (subSec%4 == 0);							// Additional variable for FSM clock event simulation
	asm("sei");											// Enable global interrupts
}

//***********************************************************************************************************************
//	Main program
//***********************************************************************************************************************
int main(void)
{
	// Configurate IO
	DDRD  = 0xFF;
	PORTD = 0x00;
	
	DDRB  = 0xFF;
	PORTB = 0x00;
	
	DDRC  = 0x00;
	PORTC = 0x1F;
	
	// Configure Timer 0
	TCCR0 = 0x03;
	
	// Configure Timer 2
	ASSR = (1<<AS2);
	
	TCCR2 = 0x02;
	TCNT2 = 0x00;
	OCR2 = 0x00;
	
	TIMSK = (1 << TOIE2)|(1 << TOIE0);	
	asm("sei");
	
    while(1)
    {
		if(fsmTrig)
		{		
			fsmTrig = 0;
			switch(fsmState)
			{
				case FSM_SHOW_TIME:
					if(ok.state == click)
					{
						fsmState = FSM_SHOW_SEC;
						ok.state = free;
					}
					else if(ok.state == hold)
					{
						decTimer = 20;
						fsmState = FSM_SET_HOUR;
						ok.state = waste;
						up.state = free;
						down.state = free;
					}
				break;
			
				case FSM_SHOW_SEC:
					if(ok.state == click)
					{
						fsmState = FSM_SHOW_TIME;
						ok.state = free;
					}
					else if(ok.state == hold)
					{
						decTimer = 20;
						fsmState = FSM_SET_HOUR;
						ok.state = waste;
						up.state = free;
						down.state = free;						
					}
				break;
			
				case FSM_SET_HOUR:
					if(ok.state == click)
					{
						decTimer = 20;
						fsmState = FSM_SET_MIN;
						ok.state = free;
					}
					else if((ok.state == hold) || (decTimer == 0))
					{
						fsmState = FSM_SHOW_TIME;
						ok.state = waste;
						up.state = free;
						down.state = free;
					}
					else if(up.state == click)
					{
						decTimer = 20;
						modHour = 1;
						up.state = free;
					}
					else if(up.state == hold)
					{
						decTimer = 20;
						modHour = 1;
					}
					else if(down.state == click)
					{
						decTimer = 20;
						modHour = -1;
						down.state = free;
					}
					else if(down.state == hold)
					{
						decTimer = 20;
						modHour = -1;
					}																		
				break;
					
				case FSM_SET_MIN:
					if(ok.state == click)
					{
						decTimer = 20;
						fsmState = FSM_CLR_SEC;
						ok.state = free;
					}
					else if((ok.state == hold) || (decTimer == 0))
					{
						fsmState = FSM_SHOW_TIME;
						ok.state = waste;
						up.state = free;
						down.state = free;
					}
					else if(up.state == click)
					{
						decTimer = 20;
						modMin = 1;
						up.state = free;
					}
					else if(up.state == hold)
					{
						decTimer = 20;
						modMin = 1;
					}
					else if(down.state == click)
					{
						decTimer = 20;
						modMin = -1;
						down.state = free;
					}
					else if(down.state == hold)
					{
						decTimer = 20;
						modMin = -1;
					}
				break;
			
				case FSM_CLR_SEC:
					if(ok.state == click)
					{
						decTimer = 20;
						fsmState = FSM_SET_HOUR;
						ok.state = free;
					}
					else if((ok.state == hold) || (decTimer == 0))
					{
						fsmState = FSM_SHOW_TIME;
						ok.state = waste;
						up.state = free;
						down.state = free;
					}
					else if(up.state == click)
					{
						decTimer = 20;
						sec = 0;
						up.state = free;
					}
					else if(up.state == hold)
					{
						decTimer = 20;
						sec = 0;
					}
					else if(down.state == click)
					{
						decTimer = 20;
						sec = 0;
						down.state = free;
					}
					else if(down.state == hold)
					{
						decTimer = 20;
						sec = 0;
					}
				break;
			}
		}
		
		// Modificate time	
		if(modSec)
		{
			if(sec < 59)	{sec++;}
			else			{sec = 0; modMin = 1;}
			if(decTimer)	{decTimer--;}
			modSec = 0;
		}
		if(modMin)
		{
			min += (uint8_t)modMin;
			if(min == 60)		{modHour++; min = 0;}
			else if(min > 60)	{modHour--; min = 59;}
			modMin = 0;
		}
		if(modHour)
		{
			hour += (uint8_t)modHour;
			if(hour >= 24) hour = 0;
			modHour = 0;
		}				
    }
}