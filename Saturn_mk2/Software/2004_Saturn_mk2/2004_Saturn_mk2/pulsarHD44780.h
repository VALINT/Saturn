/*-----------------------------------------------------------------------------------------------------------------------
 * HD44780 module
 *_______________________________________________________________________________________________________________________
 *  __    __ __ __     __   _____	 _   _  ___   _   _  _   _  ____   ____   _____
 *  \ \  / //  \\ \    \_\ / ___/	| |_| || _ \ | |_| || |_| ||___ | / __ \ /  _  \  
 *   \ \/ //    \\ \___ |/ \___ \	|  _  |||_| ||___  ||___  |   / / | __ | | |_| |
 *    \__//__/\__\\____\   /____/	|_| |_||___/     |_|    |_|  |_|  \____/ \_____/
 *					     _     _  ____  ____    _    ____  _   _
 *					    | |   | || __ \| |\ \  / \  | |\ \\ \_/ /
 *					    | |__ | || __ || |/ / / _ \ | |/ / \   /
 *					    |____||_||____/|_|\_\/_/ \_\|_|\_\  |_|
 *_______________________________________________________________________________________________________________________
 * Author:		VAL
 * Created:		12.02.2018
 * Version:		1.5
 * Last update: 04.06.2020
 *-----------------------------------------------------------------------------------------------------------------------
 * V 1.0	(12.02.2018)
 * This is a simplified library needed for managing the display 
 * with a HD44780 controller via one wire. Works only in 4-bit mode. 
 * One-wire control is implemented using the shift register SN74HC595. 
 * In theory, you can using SN74HC164, but I didn't check it.
 *
 * V 1.1	(14.02.2018)
 * I modified the library to work with SN74HC164.
 *
 * V 1.2	(25.04.2018)
 * Added support of Cyrillic alphabet.
 *
 * V 1.3	(25.04.2018)
 * Added support of 4-bits and 8-bits work modes.
 *
 * V 1.4	(12.10.2018)
 * Added SentLine function without length of line argument needing. It is more conveniently.
 *
 * V 1.5	(04.05.2020)
 * Removed  SentLine function with number of symbols argument.
 * Changed  SentChar function
 * Rewrote  SentLine function for work with pointers
 * Added functions for work with program memory
 * Added functions for work with EEPROM data memory
 * Added separated functions for work only with Latina alphabet and for work with Cyrillic alphabet.
 * Added EASY_MODE - more optimized work mode used for case when data bus located on one port.
 *-----------------------------------------------------------------------------------------------------------------------
 */ 


#ifndef PULSARHD44780_H_
#define PULSARHD44780_H_

#ifndef F_CPU
	#define F_CPU 8000000Ul
#endif


#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

// Below, you can select the desired mode of operation.
//- Work on one wire (using a shift register SN74HC595)
//- Work on two wires (using a shift register SN74HC164)
//- Standard 4-bit mode
//- standard 8-bit mode

//You also need to select the output that will be used to control the LCD. Output can be selected on any ports and in any order. 

//---------------------------------------------------------------------------------------
// Select mode
// #define USE_ONE_WIRE_MODE
// #define USE_FOUR_BIT_MODE
// #define USE_EIGHT_BIT_MODE
#define USE_FOUR_BIT_MODE

//---------------------------------------------------------------------------------------
// Easy mode more optimal version if you use 4MSB nibble on one port
#define USE_EASY_MODE
#define EASY_DATA_PORT PORTD
//---------------------------------------------------------------------------------------
// Uncomment this definition if you need to use shift register SN74HC164 or his analog.
#define USE_SN74HC164
//---------------------------------------------------------------------------------------
// In this case you can select the PORT and PIN to be used for data transfer.
#define DATAPIN			0
#define DATAPORT		PORTD
#define DATADDR			DDRD
// In this case you can select the PORT and PIN to be used for strobe.
// Only for shift register SN74HC164 or his analog.
#define EPIN			1
#define EPORT			PORTD
#define EDDR			DDRD
// If you need using 4-bit mode select above and below.
#define RSPIN			0
#define RSPORT			PORTD
#define	RSDDR			DDRD

#define RWPIN			0
#define	RWPORT			PORTD
#define	RWDDR			DDRD

#define D7PIN			7
#define	D7PORT			PORTD
#define	D7DDR			DDRD

#define D6PIN			6
#define	D6PORT			PORTD
#define	D6DDR			DDRD

#define D5PIN			5
#define	D5PORT			PORTD
#define	D5DDR			DDRD

#define D4PIN			4
#define	D4PORT			PORTD
#define	D4DDR			DDRD
// If you need using 8-bit mode.
#define D3PIN			3
#define	D3PORT			PORTD
#define	D3DDR			DDRD

#define D2PIN			2
#define	D2PORT			PORTD
#define	D2DDR			DDRD

#define D1PIN			1
#define	D1PORT			PORTD
#define	D1DDR			DDRD

#define D0PIN			0
#define	D0PORT			PORTD
#define	D0DDR			DDRD
//---------------------------------------------------------------------------------------
// You can variate this delay for faster or most stability work
#define STROBE_DELAY_MS 50

//---------------------------------------------------------------------------------------
// Definitions for using in functions
#define COMMAND			0x00
#define DATA			0x01

#define CURSOR_OFF		0x00
#define CURSOR_ON		0x02
#define CURSOR_BLINK	0x03

#define DISPLAY_OFF		0x00
#define DISPLAY_ON		0x04

#define SCROLL_LEFT		0x00
#define SCROLL_RIGHT	0x04
//----------------------------------------------------------------------------------------
//	Sets cursor on desired position.
		void LCD_GotoXY(uint8_t line, uint8_t coloum);
	
//	Scrolls the visible part in the desired direction and on the required number of characters
//	void LCD_Scroll(1, SCROLL_LEFT);
		void LCD_Scroll(uint8_t number, uint8_t dir);
	
//	Sent text data in the LCD
		void LCD_SentLine(char *line);
		
		void LCD_SentLineC(const char *line);

//		void LCD_SentLineE(const char *line);
		
//		void LCD_SentLineP(char *line, char x, char y);
		
//		void LCD_SentLinePC(const char *line, char x, char y);

//		void LCD_SentLinePE(const char *line, char x, char y);			
		
//		void LCD_SentLineCyr(char *line);
		
//		void LCD_SentLineCyrC(const char *line);

//		void LCD_SentLineCyrE(const char *line);

//		void LCD_SentLineCyrP(char *line, char x, char y);
				
//		void LCD_SentLineCyrPC(const char *line, char x, char y);

//		void LCD_SentLineCyrPE(const char *line, char x, char y);
//	Sent data byte or command in the LCD
//	LCD_SentByte(data_byte,DATA)
//	LCD_SentByte(aommand_number,COMMAND)
		void LCD_SentByte(uint8_t byte, unsigned char rs);
	
//	Sent data byte in the LCD (analog LCD_SentByte(data_byte,DATA))
		void LCD_SentChar(uint8_t data);
		
//	Sets desired cursor mode
//	LCD_Cursor(CURSOR_BLINK);
		void LCD_Cursor(uint8_t cursor);
		
//	You can turn on LCD or turn off his
//	LCD_State(DISPLAY_ON);
		void LCD_State(uint8_t state);
		
// This function initializations LCD		
		void LCD_Initial(void);
		
// This function clears LCD		
		void LCD_Clear(void);

		

#endif /* PULSARHD44780_H_ */