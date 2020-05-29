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
 * Author: VAL
 * Created: 12.02.2018
 *-----------------------------------------------------------------------------------------------------------------------
 */ 

#include "pulsarHD44780.h"


#define SET_E_TO_1		EPORT |= (1 << EPIN)
#define SET_E_TO_0		EPORT &=~ (1 << EPIN)
#define SET_RS_TO_1		RSPORT |= (1 << RSPIN)
#define SET_RS_TO_0		RSPORT &=~ (1 << RSPIN)

//void SentBit(unsigned char bit);
//void SentOutByte(uint8_t number);
void SentFourBits(uint8_t data);
//char ConvertToCyrillic(char str);


uint8_t out = 0;								// This byte is generated for sending in the shift register

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// One wire mode
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_ONE_WIRE_MODE 
	void SentBit(bool bit)							// This function sent bit in shift register short pulse if TRUE, long pulse is FALSE.
	{
		if(bit)										// Short pulse 1 (True)
		{	DATAPORT &=~ (1 << DATAPIN);
			asm("nop");
		DATAPORT |= (1 << DATAPIN);}
		else										// Long pulse 0 (False)
		{	DATAPORT &=~ (1 << DATAPIN);
			_delay_us(5);
		DATAPORT |= (1 << DATAPIN);}
		_delay_us(5);								// Wait until capacitor recharged
	}

	void SentOutByte(uint8_t number)				// This function sent byte in shift register
	{
		for(uint8_t bit = 0; bit < 8; bit++)
		{
			if((number & 0b00000001) == 1)SentBit(1);
			else SentBit(0);
			number >>= 1;
		}
		_delay_us(150);
	}

	void SentFourBits(uint8_t data)					// This function sent four bits, signal RW and signal E in the shift register.
	{
		out &=~ (0b00001111);
		out |= (0b00010000);						//Set E in 1 - transmission beginning.
		SentOutByte(out);
		#ifdef USE_SN74HC164						//Use only in SN74HC164 mode. Additional 'E' (Strobe) signal.
			SET_E_TO_1;
		#endif
		out =  out| data;
		SentOutByte(out);
		out &= (0b11101111);						//Set E in 0 - transmission finishing.
		SentOutByte(out);
		#ifdef USE_SN74HC164
			_delay_ms(1);
			SET_E_TO_0;								//Use only in SN74HC164 mode. Additional 'E' (Strobe) signal.
		#endif
		_delay_us(STROBE_DELAY_MS);					// You can variate this delay for faster or most stability work
	}
	
	void LCD_SentByte(uint8_t byte,bool rs)			// This function sent byte in the LCD
	{
		uint8_t highbyte = 0;
		highbyte = byte>>4;							// This part separates byte on two senior and younger parts, needed for 4-bits mode
		if(rs)	out |= (0b00100000);				// DATA / COMMAND
		else	out &=~ (0b00100000);				// rs = 0 - COMMAND rs = 1 - DATA
		SentFourBits(highbyte);						// transmission of the senior and younger part of the byte, consistently
		SentFourBits(byte);
	}
	
	inline void LCD_SentChar(uint8_t data)			// This function sent data byte in the LCD
	{
		LCD_SentByte(data, DATA);
	}

	
	void LCD_Initial(void)							// This function initializations LCD
	{
		DATADDR |=  (1 << DATAPIN);

		#ifdef USE_SN74HC164
		EDDR	|= (1 << EPIN);
		#endif
		
		_delay_ms(20);									//----------------------------------
		SentFourBits(0b00000011);						//
		_delay_ms(5);									//
		SentFourBits(0b00000011);						//
		_delay_us(100);									//
		SentFourBits(0b00000011);						//
		_delay_ms(1);									//
		SentFourBits(0b00000010);						//
		_delay_ms(1);									//
		LCD_SentByte(0b00101100, COMMAND);				//
		_delay_ms(1);									//
		LCD_SentByte(0b00001100, COMMAND);				//
		_delay_ms(1);									//
		LCD_SentByte(0b00000001, COMMAND);				//
		_delay_ms(1);									//
		LCD_SentByte(0b00000110, COMMAND);				//
		_delay_ms(1);									//------------------------
	}
#endif
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Four - bits mode
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef USE_FOUR_BIT_MODE
	#ifdef USE_EASY_MODE
	
		void sent_four_bits(uint8_t data)		// This function sent four bits, signal RW and signal E in the shift register.
		{
			SET_E_TO_1;							//Set E in 1 - transmission beginning.
			_delay_us(50);
			EASY_DATA_PORT &= ~ 0xf0;
			EASY_DATA_PORT |= (data << 4);
			SET_E_TO_0;							//Set E in 0 - transmission finishing.
			_delay_us(50);
		}

		void LCD_SentByte(uint8_t byte, unsigned char rs)	// This function sent byte in the LCD
		{
			char highbyte = 0;
			highbyte = byte>>4;					 // This part separates byte on two senior and younger parts, needed for 4-bits mode
			if (rs == COMMAND)SET_RS_TO_0;		// DATA / COMMAND
			else SET_RS_TO_1;					// rs = 0 - COMMAND rs = 1 - DATA
			sent_four_bits(highbyte);			// transmission of the senior and younger part of the byte, consistently
			sent_four_bits(byte);
		}

		inline void LCD_SentChar(uint8_t data)			// This function sent data byte in the LCD
		{
			LCD_SentByte(data, DATA);
		}
		
		void LCD_Initial()						 // This function initializations LCD
		{
			EDDR  |= (1 << EPIN);
			RSDDR |= (1 << RSPIN);
			
			_delay_ms(20);								//----------------------------------
			sent_four_bits(0b00000011);					//
			_delay_ms(5);								//
			sent_four_bits(0b00000011);					//
			_delay_us(100);								//
			sent_four_bits(0b00000011);					//
			_delay_ms(1);								//
			sent_four_bits(0b00000010);					//
			_delay_ms(1);								//
			LCD_SentByte(0b00101100, COMMAND);			//
			_delay_ms(1);								//
			LCD_SentByte(0b00001100, COMMAND);			//
			_delay_ms(1);								//
			LCD_SentByte(0b00000001, COMMAND);			//
			_delay_ms(1);								//
			LCD_SentByte(0b00000110, COMMAND);			//
			_delay_ms(1);								//------------------------
		}
		
	#else
	
		void sent_four_bits(uint8_t data)		// This function sent four bits, signal RW and signal E in the shift register.
		{							 
			SET_E_TO_1;							//Set E in 1 - transmission beginning.
			_delay_us(50);														
			if(data & 0b1000) D7PORT |= (1 << D7PIN); else D7PORT &=~ (1 << D7PIN);
			if(data & 0b0100) D6PORT |= (1 << D6PIN); else D6PORT &=~ (1 << D6PIN);	
			if(data & 0b0010) D5PORT |= (1 << D5PIN); else D5PORT &=~ (1 << D5PIN);
			if(data & 0b0001) D4PORT |= (1 << D4PIN); else D4PORT &=~ (1 << D4PIN);
			SET_E_TO_0;							//Set E in 0 - transmission finishing.
			_delay_us(50);
		}																		

		void LCD_SentByte(uint8_t byte, unsigned char rs)	// This function sent byte in the LCD	 
		{
			char highbyte = 0;													
			highbyte = byte>>4;					 // This part separates byte on two senior and younger parts, needed for 4-bits mode 
			if (rs == COMMAND)SET_RS_TO_0;		// DATA / COMMAND
			else SET_RS_TO_1;					// rs = 0 - COMMAND rs = 1 - DATA
			sent_four_bits(highbyte);			// transmission of the senior and younger part of the byte, consistently
			sent_four_bits(byte);												
		}

		inline void LCD_SentChar(uint8_t data)			// This function sent data byte in the LCD								
		{																		
			LCD_SentByte(data, DATA);
		}																		
	
		void LCD_Initial()						 // This function initializations LCD
		{
			EDDR  |= (1 << EPIN);
			RSDDR |= (1 << RSPIN);
			D7DDR |= (1 << D7PIN);
			D6DDR |= (1 << D6PIN);
			D5DDR |= (1 << D5PIN);
			D4DDR |= (1 << D4PIN);												
		
			_delay_ms(20);								//----------------------------------
			sent_four_bits(0b00000011);					//
			_delay_ms(5);								//
			sent_four_bits(0b00000011);					//
			_delay_us(100);								//
			sent_four_bits(0b00000011);					//
			_delay_ms(1);								//
			sent_four_bits(0b00000010);					//
			_delay_ms(1);								//
			LCD_SentByte(0b00101100, COMMAND);			//
			_delay_ms(1);								//
			LCD_SentByte(0b00001100, COMMAND);			//
			_delay_ms(1);								//
			LCD_SentByte(0b00000001, COMMAND);			//
			_delay_ms(1);								//
			LCD_SentByte(0b00000110, COMMAND);			//
			_delay_ms(1);								//------------------------
		}
		
	#endif																			
#endif																			
																				
																				
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Eight - bits mode															
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#ifdef USE_EIGHT_BIT_MODE
	#ifdef USE_EASY_MODE
		void sent_eight_bits(uint8_t data)      // This function sent eight bits, signal RW and signal E in the shift register.
		{
			SET_E_TO_1;							//Set E in 1 - transmission beginning.
			_delay_us(50);
			EASY_DATA_PORT = data;
			SET_E_TO_0;							 //Set E in 0 - transmission finishing.
			_delay_us(50);
		}

		void LCD_SentByte(uint8_t byte,bool rs)		// This function sent byte in the LCD
		{
			if (rs == COMMAND)SET_RS_TO_0;			// DATA / COMMAND
			else SET_RS_TO_1;						// rs = 0 - COMMAND rs = 1 - DATA
			sent_eight_bits(byte);
		}
		
		void LCD_SentChar(uint8_t data)				// This function sent data byte in the LCD
		{
			SET_RS_TO_1;
			sent_eight_bits(data);
		}
		
		void LCD_Initial()							 // This function initializations LCD
		{
			EDDR  |= (1 << EPIN);
			RSDDR |= (1 << RSPIN);
			
			_delay_ms(20);
			sent_eight_bits(0b00111000);					//----------------------------
			_delay_ms(5);									//
			sent_eight_bits(0b00111000);					//
			_delay_us(100);									//
			sent_eight_bits(0b00111000);					//
			_delay_ms(1);									//
			sent_eight_bits(0b00111000);					//
			_delay_ms(1);									//
			LCD_SentByte(0x38, COMMAND);					//
			_delay_ms(1);									//
			LCD_SentByte(0x0e, COMMAND);					//
			_delay_ms(1);									//
			LCD_SentByte(0b00000001, COMMAND);				//
			_delay_ms(1);									//
			LCD_SentByte(0b00000110, COMMAND);				//
			_delay_ms(1);									//
		}
		
	#else
	
		void sent_eight_bits(uint8_t data)      // This function sent eight bits, signal RW and signal E in the shift register.
		{
			SET_E_TO_1;							//Set E in 1 - transmission beginning.
			_delay_us(50);
			if(data & 0b10000000) D7PORT |= (1 << D7PIN); else D7PORT &=~ (1 << D7PIN);
			if(data & 0b01000000) D6PORT |= (1 << D6PIN); else D6PORT &=~ (1 << D6PIN);
			if(data & 0b00100000) D5PORT |= (1 << D5PIN); else D5PORT &=~ (1 << D5PIN);
			if(data & 0b00010000) D4PORT |= (1 << D4PIN); else D4PORT &=~ (1 << D4PIN);
			if(data & 0b00001000) D3PORT |= (1 << D3PIN); else D3PORT &=~ (1 << D3PIN);
			if(data & 0b00000100) D2PORT |= (1 << D2PIN); else D2PORT &=~ (1 << D2PIN);
			if(data & 0b00000010) D1PORT |= (1 << D1PIN); else D1PORT &=~ (1 << D1PIN);
			if(data & 0b00000001) D0PORT |= (1 << D0PIN); else D0PORT &=~ (1 << D0PIN);
			SET_E_TO_0;							 //Set E in 0 - transmission finishing.
			_delay_us(50);
		}

		void LCD_SentByte(uint8_t byte,bool rs)		// This function sent byte in the LCD	 
		{
			if (rs == COMMAND)SET_RS_TO_0;			// DATA / COMMAND								
			else SET_RS_TO_1;						// rs = 0 - COMMAND rs = 1 - DATA
			sent_eight_bits(byte);					
		}											
												
		void LCD_SentChar(uint8_t data)				// This function sent data byte in the LCD									
		{
			SET_RS_TO_1;
			sent_eight_bits(data);												
		}																			
																				
		void LCD_Initial()							 // This function initializations LCD								
		{
			EDDR  |= (1 << EPIN);
			RSDDR |= (1 << RSPIN);
			D7DDR |= (1 << D7PIN);													
			D6DDR |= (1 << D6PIN);
			D5DDR |= (1 << D5PIN);					
			D4DDR |= (1 << D4PIN);													
			D3DDR |= (1 << D3PIN);
			D2DDR |= (1 << D2PIN);							
			D1DDR |= (1 << D1PIN);							
			D0DDR |= (1 << D0PIN);							
														
			_delay_ms(20);									
			sent_eight_bits(0b00111000);					//----------------------------
			_delay_ms(5);									//
			sent_eight_bits(0b00111000);					//
			_delay_us(100);									//
			sent_eight_bits(0b00111000);					//
			_delay_ms(1);									//
			sent_eight_bits(0b00111000);					//
			_delay_ms(1);									//
			LCD_SentByte(0x38, COMMAND);					//
			_delay_ms(1);									//
			LCD_SentByte(0x0e, COMMAND);					//
			_delay_ms(1);									//
			LCD_SentByte(0b00000001, COMMAND);				//
			_delay_ms(1);									//
			LCD_SentByte(0b00000110, COMMAND);				//
			_delay_ms(1);									//
		}													//------------------------
	#endif											
#endif

*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
void LCD_Clear(void)								// This function clears LCD		
{
	_delay_ms(1);
	LCD_SentByte(0b00000001, COMMAND);						
	_delay_ms(2);
}

void LCD_GotoXY(uint8_t line, uint8_t coloum)		// Sets cursor on desired position.
{
	uint8_t adres;
	adres = 0x40*line+coloum;
	adres |= 0x80;
	LCD_SentByte(adres, COMMAND);	
}

void LCD_State(uint8_t state)						//	You can turn on LCD or turn off his
{
	LCD_SentByte((0b00001000|state), COMMAND);
}
/*
void LCD_Cursor(uint8_t cursor)						//	Sets desired cursor mode
{
	LCD_SentByte((0b00001100|cursor), COMMAND);
}

void LCD_Scroll(uint8_t number, uint8_t dir)		//	Scrolls the visible part in the desired direction and on the required number of characters
{
	for(uint8_t i = 0; i < number; i++)
	{
		LCD_SentByte((0b00011000|dir), COMMAND);
	}
}
*/
void LCD_SentLine(char *line)
{
	while(*line != '\0')
	{
		LCD_SentChar( *(line++) );
	}	
}	
		
void LCD_SentLineC(const char *line)
{
	char buffer;
	do
	{
		buffer = pgm_read_byte(line++);
		if(buffer == '\0') break;
		LCD_SentChar( buffer );
	}while(1);	
}
/*
void LCD_SentLineE(const char *line)
{
	char buffer;
	do
	{
		buffer = eeprom_read_byte(line++);
		if(buffer == '\0') break;
		LCD_SentChar( buffer );
	}while(1);	
}	
		
void LCD_SentLineP(char *line, char x, char y)
{
	LCD_GotoXY(x, y);
	while(*line != '\0')
	{
		LCD_SentChar( *(line++) );
	}	
}	
		
void LCD_SentLinePC(const char *line, char x, char y)
{
	char buffer;
	LCD_GotoXY(x, y);
	do
	{
		buffer = pgm_read_byte(line++);
		if(buffer == '\0') break;
		LCD_SentChar( buffer );
	}while(1);
}

void LCD_SentLinePE(const char *line, char x, char y)
{
	char buffer;
	LCD_GotoXY(x, y);
	do
	{
		buffer = eeprom_read_byte(line++);
		if(buffer == '\0') break;
		LCD_SentChar( buffer );
	}while(1);	
}	
		
void LCD_SentLineCyr(char *line)			//	Sent text data in the LCD
{
	while(*line != '\0')
	{
		LCD_SentChar(ConvertToCyrillic(*line));
		line++;
	}
}

void LCD_SentLineCyrC(const char *line)			//	Sent text data in the LCD
{
	char buffer;
	do
	{
		buffer = pgm_read_byte(line);
		if(buffer == '\0') break;
		LCD_SentChar( ConvertToCyrillic( buffer ) );
		line++;
	}while(1);
}

void LCD_SentLineCyrE(const char *line)
{	
	char buffer;
	do
	{
		buffer = eeprom_read_byte(line++);
		if(buffer == '\0') break;
		LCD_SentChar( ConvertToCyrillic( buffer ) );
	}while(1);
}

void LCD_SentLineCyrP(char *line, char x, char y)
{
	LCD_GotoXY(x, y);
	while(*line != '\0')
	{
		LCD_SentChar(ConvertToCyrillic(*line));
		line++;
	}
}
		
void LCD_SentLineCyrPC(const char *line, char x, char y)
{
	char buffer;
	LCD_GotoXY(x, y);
	do
	{
		buffer = pgm_read_byte(line);
		if(buffer == '\0') break;
		LCD_SentChar( ConvertToCyrillic( buffer ) );
		line++;
	}while(1);
}

void LCD_SentLineCyrPE(const char *line, char x, char y)
{
	char buffer;
	LCD_GotoXY(x, y);
	do
	{
		buffer = eeprom_read_byte(line++);
		if(buffer == '\0') break;
		LCD_SentChar( ConvertToCyrillic( buffer ) );
	}while(1);
}

char ConvertToCyrillic(char str)
{
		switch (str)
	{
		case 'À': return 0x41; break;      // À
		case 'Á': return 0xA0; break;      // Á
		case 'Â': return 0x42; break;      // Â
		case 'Ã': return 0xA1; break;      // Ã
		case 'Ä': return 0xE0; break;      // Ä
		case 'Å': return 0x45; break;      // Å
		case '¨': return 0xA2; break;      // ¨
		case 'Æ': return 0xA3; break;      // Æ
		case 'Ç': return 0x33; break;      // Ç
		case 'È': return 0xA5; break;      // È
		case 'É': return 0xA6; break;      // É
		case 'Ê': return 0x4B; break;      // Ê
		case 'Ë': return 0xA7; break;      // Ë
		case 'Ì': return 0x4D; break;      // Ì
		case 'Í': return 0x48; break;      // Í
		case 'Î': return 0x4F; break;      // Î
		case 'Ï': return 0xA8; break;      // Ï
		case 'Ð': return 0x50; break;      // Ð
		case 'Ñ': return 0x43; break;      // Ñ
		case 'Ò': return 0x54; break;      // Ò
		case 'Ó': return 0xA9; break;      // Ó
		case 'Ô': return 0xAA; break;      // Ô
		case 'Õ': return 0x58; break;      // Õ
		case 'Ö': return 0xE1; break;      // Ö
		case '×': return 0xAB; break;      // ×
		case 'Ø': return 0xAC; break;      // Ø
		case 'Ù': return 0xE2; break;      // Ù
		case 'Ú': return 0xAD; break;      // Ú
		case 'Û': return 0xAE; break;      // Û
		case 'Ü': return 0x62; break;      // Ü
		case 'Ý': return 0xAF; break;      // Ý
		case 'Þ': return 0xB0; break;      // Þ
		case 'ß': return 0xB1; break;      // ß
		case 'à': return 0x61; break;      // à
		case 'á': return 0xB2; break;      // á
		case 'â': return 0xB3; break;      // â
		case 'ã': return 0xB4; break;      // ã
		case 'ä': return 0xE3; break;      // ä
		case 'å': return 0x65; break;      // å
		case '¸': return 0xB5; break;      // ¸
		case 'æ': return 0xB6; break;      // æ
		case 'ç': return 0xB7; break;      // ç
		case 'è': return 0xB8; break;      // è
		case 'é': return 0xB9; break;      // é
		case 'ê': return 0xBA; break;      // ê
		case 'ë': return 0xBB; break;      // ë
		case 'ì': return 0xBC; break;      // ì
		case 'í': return 0xBD; break;      // í
		case 'î': return 0x6F; break;      // î
		case 'ï': return 0xBE; break;      // ï
		case 'ð': return 0x70; break;      // ð
		case 'ñ': return 0x63; break;      // ñ
		case 'ò': return 0xBF; break;      // ò
		case 'ó': return 0x79; break;      // ó
		case 'ô': return 0xE4; break;      // ô
		case 'õ': return 0x78; break;      // õ
		case 'ö': return 0xE5; break;      // ö
		case '÷': return 0xC0; break;      // ÷
		case 'ø': return 0xC1; break;      // ø
		case 'ù': return 0xE6; break;      // ù
		case 'ú': return 0xC2; break;      // ú
		case 'û': return 0xC3; break;      // û
		case 'ü': return 0xC4; break;      // ü
		case 'ý': return 0xC5; break;      // ý
		case 'þ': return 0xC6; break;      // þ
		case 'ÿ': return 0xC7; break;      // ÿ
		default : return str;
	}
}
*/