//***********************************************************************************************************************
//	  ________      _    _________   _      _   ______    _    _			 _      _   _   _          _____
//	 / ______/     / \  |___   ___| | |    | | |  __  \  | \  | |			| \    / | | | / /        /  __ \
//   | |_____     / _ \     | |		| |    | | | |__| |  |  \ | |			|  \  /  | | |/ /		 |_/  / |
//   \______ \   / /_\ \    | |     | |    | | |  __  /  |   \| |			| \ \/ / | |   |            / /  
//    ______| | / _____ \   | |		|  \__/  | | |  \ \  | |\   |  _______  | |\__/| | | |\ \   _     / /___ 
//	 /_______/ /_/     \_\  |_|      \______/  |_|   \_\ |_| \__| |_______| |_|    |_| |_| \_\ |_|   |______|
//
//***********************************************************************************************************************
//		   Created: 27.04.2020 16:43:59
//			Author: VAL
//  Actual version: 1.51
//***********************************************************************************************************************
#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "rtc.h"
#include "tone.h"
#include "dht11.h"
#include "buttons.h"
#include "pulsarHD44780.h"
#include "standart_interface.h"

//***********************************************************************************************************************
//	User defined parameters
//***********************************************************************************************************************
// For Cyrillic  version DOT = 0x6F and BELL = 0xED
// For Aliexpres version DOT = 0x6F and BELL = Ahaha, without bell, BELL =  
#define BELL '*'//0xED
#define DOT  0x6F

// Choose language (ENG, RUS, UKR)
#define ENG
//#define RUS
//#define UKR

// Return from menu if 60 sec without activities
#define MENU_RETURN

//***********************************************************************************************************************
//	System parameters
//***********************************************************************************************************************
#define FSM_SHOW_TIME		0x00
#define FSM_SHOW_TEMP		0x01
#define FSM_SHOW_DATE		0x02
#define FSM_SHOW_ALARM		0x03
#define FSM_MENU1			0x10
#define FSM_MENU1_HOUR		0x11
#define FSM_MENU1_MIN		0x12
#define FSM_MENU1_SEC		0x13
#define FSM_MENU1_FORMAT	0x14
#define FSM_MENU2			0x20
#define FSM_MENU2_YEAR		0x21
#define FSM_MENU2_MONTH		0x22
#define FSM_MENU2_DAY		0x23
#define FSM_MENU2_WDAY		0x84
#define FSM_MENU3			0x30
#define FSM_MENU3_AL1_HOUR	0x31
#define FSM_MENU3_AL1_MIN	0x32
#define FSM_MENU3_AL1_ON	0x34
#define FSM_MENU4			0x40
#define FSM_MENU4_AL2_HOUR	0x41
#define FSM_MENU4_AL2_MIN	0x42
#define FSM_MENU4_AL2_ON	0x44
#define FSM_MENU5			0x50
#define FSM_MENU5_SENSOR	0x51
#define FSM_MENU6			0x60
#define FSM_MENU6_BL_NIGHTT	0x61
#define FSM_MENU6_BL_DAYT	0x62
#define FSM_MENU6_BL_NAGHTB	0x63
#define FSM_MENU6_BL_DAYB	0x64
#define FSM_MENU7			0x70
#define FSM_MENU7_PRESET	0x71
#define FSM_MENU7_TIME		0x72

#define FULL				1
#define TRUNC				0

#define OK_BUTTON			1
#define UP_BUTTON			0
#define DOWN_BUTTON			2
#define BUTTON_PIN			PINC

//***********************************************************************************************************************
//	Program memory constants
//***********************************************************************************************************************  

// Segments for big screen mode
const char segments[8][8] PROGMEM = {
	{0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F},			//
	{0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E},			//
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},			//
	{0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F},			//
	{0x1F,0x1F,0x00,0x00,0x00,0x00,0x1F,0x1F},			//
	{0x1E,0x1E,0x00,0x00,0x00,0x00,0x1E,0x1E},			//
	{0x0F,0x0F,0x00,0x00,0x00,0x00,0x0F,0x0F},			//
	{0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F}			//
};

const char arrov[]		PROGMEM = ">  ";
const uint8_t max[10]	PROGMEM = {1, 1, 2, 6, 20, 23, 23, 255, 255, 1};
const char space[]		PROGMEM = "         ";


#ifdef ENG
	
	const char timeStr[]	PROGMEM = "Time";
	const char dateStr[]	PROGMEM = "Date";
	const char al1Str[]		PROGMEM = "Alarm ";
	const char pm[]			PROGMEM = "PM";
	const char am[]			PROGMEM = "AM";
	const char sensorT[]	PROGMEM = "Sensor";
	const char brightness[]	PROGMEM = "Brightness";
	const char off[]		PROGMEM = "OFF";
	const char on[]			PROGMEM = "ON";
	const char temp[]		PROGMEM = "Temperature";
	const char wakeup[]		PROGMEM = "Wake UP";
	const char preset[]		PROGMEM = "Preset";
	const char format[]		PROGMEM = "Format";
	const char nTime[]		PROGMEM = "N time - ";
	const char dTime[]		PROGMEM = "D time - ";
	const char nBr[]		PROGMEM = "N br - ";
	const char dBr[]		PROGMEM = "D br - ";
	const char swTime[]		PROGMEM = "Sw time - ";

	const char week[7][12]	PROGMEM =
	{
		"   Sunday",
		"   Monday",
		"   Tuesday",
		" Wednesday",
		"  Thursday",
		"   Friday",
		"  Saturday"
	};
#endif	
	
#ifdef RUS

	const char timeStr[]	PROGMEM = {0x42, 0x70, 0x65, 0xBC, 0xC7, 0x00};											// Время
	const char dateStr[]	PROGMEM = {0xE0, 0x61, 0xBF, 0x61, 0x00};												// Дата 
	const char al1Str[]		PROGMEM = {0xA0, 0x79, 0xE3, 0xB8, 0xBB, 0xC4, 0xBD, 0xB8, 0xBA, 0x20, 0x00};			// Будильник
	const char pm[]			PROGMEM = {0xE0, 0xA8, 0x00};															// ДП
	const char am[]			PROGMEM = {0xA8, 0xA8, 0x00};															// ПП 
	const char sensorT[]	PROGMEM = {0xE0, 0x61, 0xBF, 0xC0, 0xB8, 0xBA, 0x00};									// Датчик 
	const char brightness[]	PROGMEM = {0xB1, 0x70, 0xBA, 0x6F, 0x63, 0xBF, 0xC4, 0x00};								// Яркость 
	const char off[]		PROGMEM = {0x42, 0xAE, 0x4B, 0xA7, 0x00};												// ВЫКЛ	
	const char on[]			PROGMEM = {0x42, 0x4B, 0xA7, 0x00};														// ВКЛ
	const char temp[]		PROGMEM = {0x54, 0x65, 0xBC, 0xBE, 0x65, 0x70, 0x61, 0xBF, 0x79, 0x70, 0x61, 0x00};		// Температура 
	const char wakeup[]		PROGMEM = {0x42, 0x63, 0xBF, 0x61, 0xB3, 0x61, 0xB9, 0x00};								// Вставай 
	const char preset[]		PROGMEM = {0xAC, 0x61, 0xB2, 0xBB, 0x6F, 0xBD, 0x00};									// Шаблон 
	const char format[]		PROGMEM = {0xAA, 0x6F, 0x70, 0xBC, 0x61, 0xBF, 0x00};									// Формат 
	const char nTime[]		PROGMEM = {0x42, 0x70, 0x65, 0xBC, 0xC7, 0x20, 0xBD, 0x6F, 0xC0, 0xC4, 0x20, 0x00};		// Время ночь 
	const char dTime[]		PROGMEM = {0x42, 0x70, 0x65, 0xBC, 0xC7, 0x20, 0xE3, 0x65, 0xBD, 0xC4, 0x20, 0x00};		// Время день 
	const char nBr[]		PROGMEM = {0x4D, 0xB8, 0xBD, 0x20, 0xC7, 0x70, 0xBA, 0x20, 0x00};						// Ночь ярк 
	const char dBr[]		PROGMEM = {0x4D, 0x61, 0xBA, 0x63, 0x20, 0xC7, 0x70, 0xBA, 0x20, 0x00};					// День ярк 
	const char swTime[]		PROGMEM = {0xA8, 0x65, 0x70, 0x65, 0x78, 0x6F, 0xE3, 0x20, 0x00};						// Переход 
	
	const char week[7][12]	PROGMEM =
	{
		{0x42, 0x6F, 0x63, 0xBA, 0x70, 0x65, 0x63, 0x65, 0xBD, 0xC4, 0x65, 0x00},		// Воскресенье
		{0xA8, 0x6F, 0xBD, 0x65, 0xE3, 0x65, 0xBB, 0xC4, 0xBD, 0xB8, 0xBA, 0x00},		// Понедельник 
		{0x20, 0x20, 0x42, 0xBF, 0x6F, 0x70, 0xBD, 0xB8, 0xBA, 0x00},					// Вторник 
		{0x20, 0x20, 0x20, 0x43, 0x70, 0x65, 0xE3, 0x61, 0x00},							// Среда 
		{0x20, 0x20, 0xAB, 0x65, 0xBF, 0xB3, 0x65, 0x70, 0xB4, 0x00},					// Четверг
		{0x20, 0x20, 0xA8, 0xC7, 0xBF, 0xBD, 0xB8, 0xE5, 0x61, 0x00},					// Пятница 
		{0x20, 0x20, 0x43, 0x79, 0xB2, 0xB2, 0x6F, 0xBF, 0x61, 0x00}					// Суббота 
	};
	
#endif

#ifdef UKR
	
	const char timeStr[]	PROGMEM = {0xAB, 'a',  'c',  0x00};														// Час
	const char dateStr[]	PROGMEM = {0xE0, 0x61, 0xBF, 0x61, 0x00};												// Дата 
	const char al1Str[]		PROGMEM = {0xA0, 0x79, 0xE3, 0xB8, 0xBB, 0xC4, 0xBD, 0xB8, 0xBA, 0x20, 0x00};			// Будильник
	const char pm[]			PROGMEM = {0xE0, 0xA8, 0x00};															// ДП
	const char am[]			PROGMEM = {0xA8, 0xA8, 0x00};															// ПП 
	const char sensorT[]	PROGMEM = {'C',   'e', 0xBD,  'c',  'o',  'p', 0x00};									// Сенсор 
	const char brightness[]	PROGMEM = {0xB1,  'c', 0xBA,  'p',  'a', 0xB3,  'i',  'c', 0xBF, 0xC4, 0x00};			// Яскравість 
	const char off[]		PROGMEM = { 'B', 0xA5, 0x4D,  'K', 0x00};												// ВИМК	
	const char on[]			PROGMEM = {0xA9,  'B',  'I',  'M',  'K', 0x00};											// УВІМК
	const char temp[]		PROGMEM = {0x54, 0x65, 0xBC, 0xBE, 0x65, 0x70, 0x61, 0xBF, 0x79, 0x70, 0x61, 0x00};		// Температура 
	const char wakeup[]		PROGMEM = {0x42, 0x63, 0xBF, 0x61, 0xB3, 0x61, 0xB9, 0x00};								// Вставай 
	const char preset[]		PROGMEM = {0xAC, 0x61, 0xB2, 0xBB, 0x6F, 0xBD, 0x00};									// Шаблон 
	const char format[]		PROGMEM = {0xAA, 0x6F, 0x70, 0xBC, 0x61, 0xBF, 0x00};									// Формат 
	const char nTime[]		PROGMEM = {0xAB,  'a',  'c', 0x20, 0xBD,  'i', 0xC0, 0x20, 0x00};						// Час ніч 
	const char dTime[]		PROGMEM = {0xAB,  'a',  'c', 0x20, 0xE3,  'e', 0xBD, 0xC4, 0x20, 0x00};					// Час день 
	const char nBr[]		PROGMEM = { 'T',  'e', 0xBC, 0xBD, 0x20, 0xC7,  'c', 0xBA, 0x20, 0x00};					// Темн яск
	const char dBr[]		PROGMEM = { 'C', 0xB3,  'i', 0xBF, 0x20, 0xC7,  'c', 0xBA, 0x20, 0x00};					// Світ яск 
	const char swTime[]		PROGMEM = {0xA8, 0x65, 0x70, 0x65, 0x78,  'i', 0xE3, 0x20, 0x00};						// Перехід 
	
	const char week[7][12]	PROGMEM =
	{
		{0x20, 0x20, 0x20, 'H',  'e',  0xE3, 'i',  0xBB, 0xC7, 0x00},							// Неділя
		{0x20, 0xA8, 0x6F, 0xBD, 0x65, 0xE3, 'i',  0xBB, 'o',  0xBA, 0x00},						// Понеділок 
		{0x20, 0x20, 0x42, 'i',  0xB3, 0xBF, 0x6F, 0x70, 'o' , 0xBA, 0x00},						// Вівторок 
		{0x20, 0x20, 0x20, 0x20, 0x43, 0x65, 0x70, 0x65, 0xE3, 0x61, 0x00},						// Середа 
		{0x20, 0x20, 0x20, 0xAB, 0x65, 0xBF, 0xB3, 0x65, 0x70, 0xB4, 0x00},						// Четверг
		{0x20, 0x20, 0xA8, '`',  0xC7, 0xBF, 0xBD, 0xB8, 0xE5, 0xC7, 0x00},						// П'ятниця 
		{0x20, 0x20, 0x20, 0x43, 0x79, 0xB2, 0x6F, 0xBF, 0x61, 0x00}							// Субота 
	};
#endif

const char sensor[2][7]	PROGMEM =
{
	"DS3231",
	"DHT-11"
};
//***********************************************************************************************************************
// EEPROM constants
//***********************************************************************************************************************
#define alarm1En	0
#define	alarm2En	1
#define	sensorType	2
#define	viewPreset	3
#define	tempRetTime 4
#define	brDayTime	5
#define	brNightTime 6
#define	brDay		7
#define	brNight		8
#define	t12HFormat	9

uint8_t eeconst[] EEMEM = {0, 0, 0, 0, 10, 7, 20, 255, 10, 0}; 

#define M_LENGHT 144

unsigned int songLen EEMEM = M_LENGHT;

// Melody for alarms
int music[M_LENGHT] EEMEM = {
	C5,  C5,  C5,  C5,  C5,  C5,  C5,  C5,  Eb5, Eb5, Eb5, Eb5, Eb5, Eb5, Eb5, Eb5,
	F5,  F5,  F5,  F5,  F5,  F5,  Eb5, Eb5, Eb5, Eb5, Eb5, Eb5, F5,  F5,  F5,  F5,
	F5,  F5,  F5,  F5,  0,   F5,  F5,  F5,  Bb5, Bb5, Bb5, Bb5, Ab5, Ab5, Ab5, Ab5,
	G5,  G5,  F5,  F5,  F5,  F5,  G5,  G5,  G5,  G5,  G5,  G5,  0,   0,   0,   0,
	G5,  G5,  G5,  G5,  G5,  G5,  G5,  G5,  Bb5, Bb5, Bb5, Bb5, Bb5, Bb5, Bb5, Bb5,
	C6,  C6,  C6,  C6,  C6,  C6,  F5,  F5,  F5,  F5,  F5,  F5,  Eb5, Eb5, Eb5, Eb5,
	Bb5, Bb5, Bb5, Bb5, 0,   Bb5, Bb5, Bb5, G5,  G5,  G5,  G5,  Bb5, Bb5, Bb5, Bb5,
	Bb5, Bb5, Bb5, Bb5, Bb5, Bb5, C6,  C6,  C6,  C6,  C6,  C6,  C6,  C6,  C6,  C6,
	0,	 0,	  0,   0,	0,	 0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

//***********************************************************************************************************************
//	Program variables
//***********************************************************************************************************************
volatile Button			upButton	= {freeSt, &PINC, UP_BUTTON, 0};		// UP   button structure
volatile Button			okButton	= {freeSt, &PINC, OK_BUTTON, 0};		// OK   button structure
volatile Button			downButton	= {freeSt, &PINC, DOWN_BUTTON, 0};		// DOWN button structure
volatile rtc_data		timeData;											// RTC structure
volatile DHT			tempHumid;							// DHT11/DHT22 structure
volatile uint8_t		main_fsm	= 0,					// Main FSM variable
						subFrame	= 0,					// Need for FSM clocking and sound sampling
						fsmEn		= 0,					// FSM clock (16 times per second)
						lcdClr		= 0;					// Clear LCD flag
										
volatile uint8_t		menuRetTime = 60,					// Return fro menu timer
						swTimer =	  0,					// Switching between screens Time -> Temperature -> Date
						alarm =		  0,					// Alarm flag. 
						t12HFHelper=  0;					// Need to recalculate 24H format to 12H
					
		uint8_t			param[10];							// User parameters
		int				soundC=		  0,					// Song pointer
						soundLen=     0;					// Song length
						
// Helper functions
void printTime(void);										// Print big screen time
void print3x2s(char symbol, char place);					// Print big digit
void procButtons(uint8_t *data);							// Optimized buttons processing for FSM
void nextState(uint8_t *st, uint8_t *helper, uint8_t type, uint8_t prev, uint8_t next, uint8_t ok, uint8_t hold);
void writeEEP();											// Writing parameter bit in EEPROM if the last one was changed

// External interrupt 0 handler.
ISR(INT0_vect)												// Front edge interrupt from RTC 1Hz signal
{
	RTC_read_time(&timeData);								// Read actual time from RTC

	
	if(timeData.sec == 0)									// If new minute
	{
		if(main_fsm < 10)
		{
			RTC_read_date(&timeData);						// Read actual time from RTC
			RTC_read_alarm(&timeData);						// Read actual alarms time from RTC
			RTC_read_temp(&timeData);						// Read temperature from RTC
		}		
		// Check alarm event
		if( (timeData.al1hour == timeData.hour) && (timeData.al1min == timeData.min) && (param[alarm1En]) ){alarm = 1;}
		if( (timeData.al2hour == timeData.hour) && (timeData.al2min == timeData.min) && (param[alarm2En]) ){alarm = 1;}	
	}

#ifdef MENU_RETURN	
	if(main_fsm >= FSM_MENU1 && menuRetTime > 0)
	{
		menuRetTime -= 1;
	}
#endif
	
	if( (param[sensorType] == 1) )							// Every 5 seconds read DHT11/DHT22 data
	{
		if( timeData.sec % 5 == 0 )
		{
			asm("WDR");										// Clear Watchdog
			DHTreadData(&tempHumid);						// Read DHT11/DHT22 data 
			asm("WDR");										// Clear Watchdog
		}		
	}
}

ISR(TIMER2_COMP_vect)										// 128 times per second
{
	TCNT2 = 0;												// Clear Timer2
		
	checkButton(&okButton);									// Renew OK   button status
	checkButton(&upButton);									// Renew UP   button status
	checkButton(&downButton);								// Renew DOWN button status
	
	subFrame++;												// Refresh subFrame timer
	subFrame &= 0b01111111;									// Clear subFrame timer if he greater than 127
	
	if(!subFrame)											// Switching timer, need for switching between screens time->temp->date
	{
		if(swTimer > 0 && main_fsm < 4) swTimer--;			// Decrement if greater than 0
	}
	
	fsmEn = subFrame%8;										// FSM clocking 16 times per second

	if(alarm)												// Play song if alarm
	{
		if(subFrame%8 == 0)									// 16 times per second
		{
			if(soundC < soundLen-1) soundC++;
			else soundC = 0;
			
			OCR1A = eeprom_read_word( &(music[soundC]) );	// Write coefficient to OCR reg (timer in CTC mode)
		}		
	}
}

int main(void)
{
	uint8_t		modHour		= 0,							// Mod hours/minutes/seconds  
				modMin		= 0,							// Need for Time and Alarms setting
				modSec		= 0,							//
				modSensor;									// Need for sensor changing
	uint16_t	brTemporal  = 0;							// Need for brightness counting
	uint8_t	   *timePtr		= &timeData.sec;
	uint16_t	brArr[16];									// Array for brightness measurement filtration
	uint8_t		brPtr		= 0;							// Array index

	// Setup Watchdog
	WDTCR    = (1 << WDCE)|(1 << WDE);
	WDTCR   |= (1 << WDE)|(1 << WDP2)|(1 << WDP1)|(1 << WDP0);
	asm("WDR");												// Clear watchdog
	
	// Setup PORTs
	DDRD  = 0xF3;
	DDRB  = 0x0F;
	DDRC  = 0x00;
	PORTC = 0x07;
	
	// Enable External interrupt INT0
	MCUCR = (1 << ISC01)|(1 << ISC00);
	GICR  = (1 << INT0);
	
	// Setup Timer2 (Compare int/ 128 times per second/ F_CPU = 8MHz)
	TCCR2 = (1 << CS22)|(1 << CS21);
	OCR2  = 244;
	TIMSK = (1 << OCIE2);
	
	// Setup ADC
	ADMUX = (1 << REFS0)|(1 << MUX1)|(1 << MUX0);
	ADCSRA= (1 << ADEN)|(1 << ADSC)|(1 << ADPS2)|(1 << ADPS1);
	
	// Setup Timer 1 (8 bit Fast PWM mode)
	TCCR1A = (1 << WGM10)|(1 << COM1A1);
	TCCR1B = (1 << CS10)|(1 << WGM12);
	OCR1A = 255;										// Maximum duty cycle
	
	// Read user parameters from EEPROM
	for (uint8_t i = 0; i < 10; i++)
	{
		asm("WDR");
		eeprom_busy_wait();
		param[i] = eeprom_read_byte(&eeconst[i]);
		if(param[i] > pgm_read_byte(&(max[i]))) param[i] = 0;
	}
	swTimer = param[tempRetTime];
	modSensor=param[sensorType];
	
	// Read song length from EEPROM
	eeprom_busy_wait();
	soundLen = eeprom_read_word(&songLen);
	
	if( !(MCUCSR & (1 << WDRF)) )				// If NOT WATCHDOG RESET (Power ON Reset or Hard Reset)	
	{
		// Initialize HD44780 LCD
		asm("WDR");
		_delay_ms(100);
		LCD_Initial();
		_delay_ms(10);
		LCD_Clear();
		_delay_ms(10);
		
		// Send big digits segments to LCD
		LCD_SentByte(0x40,COMMAND);
		asm("WDR");
		for (uint8_t i=0; i < 8; i++)
		{
			for(uint8_t j = 0; j < 8; j++)
			{
				uint8_t buf = (uint8_t)pgm_read_byte(&segments[i][j]);
				LCD_SentByte(buf ,DATA);
			}
		}
		
		// Read DHT11/22 data if chosen sensor type 1
		if( (param[sensorType] == 1) )
		{
			DHTreadData(&tempHumid);							// Read DHT11/DHT22 data
			asm("WDR");
		}	
	}
	else														// If Watchdog reset (DHT11/22 failure)
	{
		param[sensorType] = 0;									// Change sensor type on 0 (DS3231 internal sensor)
		MCUCSR &= ~(1 << WDRF);									// Clear watchdog reset flag
		asm("WDR");
	}
	
	// Initialize I2C and RTC
	asm("WDR");
	_i2c_init();
	RTC_init();
	
	// Real all RTC data
	RTC_read_time(&timeData);
    RTC_read_date(&timeData);
	RTC_read_alarm(&timeData);
	RTC_read_temp(&timeData);

	// Enable global interrupts
	asm("SEI");
	
	// Endless loop
	while(1)
    {
		if(!fsmEn)	// FSM enable flag (16 times per second)
		{
			asm("WDR");
			switch (main_fsm)
			{
				//**********************************************************************************************************************************
				// Show time mode
				//**********************************************************************************************************************************
				case FSM_SHOW_TIME:
					if(okButton.state == clickSt) {okButton.state = freeSt;  main_fsm = FSM_SHOW_TEMP; swTimer = param[tempRetTime]; lcdClr = 1;} 
					else if(okButton.state == holdSt)  {okButton.state = wasteSt; main_fsm = FSM_MENU1; lcdClr = 1;}
					timePtr = &timeData.sec;
					
					if(!swTimer)
					{
						if(param[viewPreset] == 1 || param[viewPreset] == 2){swTimer = param[tempRetTime]; main_fsm = FSM_SHOW_TEMP; lcdClr = 1;}
						else if(param[viewPreset] == 3){swTimer = param[tempRetTime]; main_fsm = FSM_SHOW_DATE; lcdClr = 1;}
					}
					if(timeData.sec == 60 - param[tempRetTime] - param[tempRetTime])
					{
						if(param[viewPreset] == 4){ main_fsm = FSM_SHOW_TEMP; lcdClr = 1;}
					}
					else if(timeData.sec == 60 - param[tempRetTime])
					{
						if(param[viewPreset] == 5){ main_fsm = FSM_SHOW_TEMP;  lcdClr = 1;}
						else if(param[viewPreset] == 6){ main_fsm = FSM_SHOW_DATE;  lcdClr = 1;}
					}					
					if(alarm)				// Set Timer1 in CTC mode and go to Alarm screen
					{
						setUpCTC(); 
						lcdClr = 1; 
						main_fsm = FSM_SHOW_ALARM;
					}
					if(!lcdClr) printTime();
					else{ LCD_Clear(); lcdClr = 0;}
				break;
				
				//**********************************************************************************************************************************
				// Show temperature mode
				//**********************************************************************************************************************************
				case FSM_SHOW_TEMP:
					if(okButton.state == clickSt) {okButton.state = freeSt;  main_fsm = FSM_SHOW_DATE; swTimer = param[tempRetTime]; lcdClr = 1;}
					else if(okButton.state == holdSt)  {okButton.state = wasteSt; main_fsm = FSM_MENU1; lcdClr = 1;}
					
					// Go to next screen
					if(!swTimer)
					{
						if(param[viewPreset] == 1){swTimer = param[tempRetTime]; main_fsm = FSM_SHOW_DATE; lcdClr = 1;}
						else if(param[viewPreset] == 2){swTimer = param[tempRetTime]; main_fsm = FSM_SHOW_TIME; lcdClr = 1;}
					}
					if((timeData.sec == 60 - param[tempRetTime] && param[viewPreset] == 4) || param[viewPreset] == 3 || param[viewPreset] == 6)
					{ 
						main_fsm = FSM_SHOW_DATE; 
						lcdClr = 1;
					}
					if(timeData.sec == 0 && param[viewPreset] == 5) { main_fsm = FSM_SHOW_TIME; lcdClr = 1;}
					
					if(alarm)				// Set Timer1 in CTC mode and go to Alarm screen
					{
						setUpCTC();
						lcdClr = 1;
						main_fsm = FSM_SHOW_ALARM;
					}
					if(!lcdClr)
					{
						if( param[sensorType] == 1 ) // Show DHT11 data
						{
							LCD_GotoXY(0, 2);
							LCD_SentLineC( PSTR(" t-  ") );
							LCD_SentChar(48 + tempHumid.tempWhole/10);
							LCD_SentChar(48 + tempHumid.tempWhole%10);
							LCD_SentChar('.');
							LCD_SentChar(48 + tempHumid.tempFractional/10);
							LCD_SentChar(0xDF);								
							LCD_SentChar('C');
						
							LCD_GotoXY(1, 2);
							LCD_SentLineC(PSTR("RH-  "));
							LCD_SentChar(48 + tempHumid.rhWhole/10);
							LCD_SentChar(48 + tempHumid.rhWhole%10);
							LCD_SentChar('.');
							LCD_SentChar(48 + tempHumid.rhFractional/10);
							LCD_SentChar('%');
						}
						else if( param[sensorType] == 0) // Show DS3231 data
						{
							LCD_GotoXY(0, 2);
							LCD_SentLineC( &temp );
							LCD_GotoXY(1, 4);
							if(timeData.wtemperature & 0x80)
							{
								LCD_SentChar('-');
								timeData.wtemperature = (~timeData.wtemperature) + 1;
							}								
							LCD_SentChar(48 + timeData.wtemperature/10);
							LCD_SentChar(48 + timeData.wtemperature%10);
							LCD_SentChar('.');
							LCD_SentChar(48 + timeData.ftemperature/10);
							LCD_SentChar(48 + timeData.ftemperature%10);
							LCD_SentChar(0xDF);
							LCD_SentChar('C');
						}					
					}
					else
					{
						LCD_Clear();
						lcdClr = 0;
					}	
															
				break;
				
				//**********************************************************************************************************************************
				// Show date mode
				//**********************************************************************************************************************************
				case FSM_SHOW_DATE:
					if(okButton.state == clickSt) {okButton.state = freeSt;  main_fsm = FSM_SHOW_TIME; swTimer = param[tempRetTime]; lcdClr = 1;}
					else if(okButton.state == holdSt)  {okButton.state = wasteSt; main_fsm = FSM_MENU1; lcdClr = 1;}
					
					// Go to next screen
					if(!swTimer)
					{
						if(param[viewPreset] == 1){swTimer = param[tempRetTime]; main_fsm = FSM_SHOW_TIME; lcdClr = 1;}
						else if(param[viewPreset] == 3){swTimer = param[tempRetTime]; main_fsm = FSM_SHOW_TIME; lcdClr = 1;}
					}
					if(timeData.sec == 0 || param[viewPreset] == 2 || param[viewPreset] == 5) { main_fsm = FSM_SHOW_TIME; lcdClr = 1;}
	
					if(alarm)				// Set Timer1 in CTC mode and go to Alarm screen
					{
						setUpCTC();
						lcdClr = 1;
						main_fsm = FSM_SHOW_ALARM;
					}
					
					if(!lcdClr)
					{
						LCD_GotoXY(0, 2);
						LCD_SentLineC( &(week[timeData.wday-1][0]) );
					}
					else
					{
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Alarm mode
				//**********************************************************************************************************************************	
				case FSM_SHOW_ALARM:
					if(okButton.state == clickSt) {okButton.state = freeSt;  main_fsm = FSM_SHOW_TIME; swTimer = param[tempRetTime]; lcdClr = 1; alarm = 0;}
					else if(okButton.state == holdSt)  {okButton.state = wasteSt; main_fsm = FSM_SHOW_TIME; swTimer = param[tempRetTime]; lcdClr = 1; alarm = 0;}
				
					if(!lcdClr)
					{
						LCD_GotoXY(0, 4);
						LCD_SentLineC( &wakeup );
					}
					else // Return Timer 1 in 8 bit Fast PWM mode and clear screen
					{
						soundC = 0;
						TCCR1A = (1 << WGM10)|(1 << COM1A1);
						TCCR1B = (1 << CS10)|(1 << WGM12);
						OCR1A  = 255;
						LCD_Clear();
						lcdClr = 0;
					}	
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 1 - Time setting
				//**********************************************************************************************************************************
				case FSM_MENU1:
					nextState(&main_fsm, &lcdClr, FULL, FSM_MENU7, FSM_MENU2, FSM_MENU1_HOUR, FSM_SHOW_TIME);
					
					timePtr = &timeData.sec;
					
					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentLineC( &arrov[0]);
						LCD_SentLineC( &timeStr[0] );
					}
					else
					{
						writeEEP();
						LCD_Clear();
						lcdClr = 0;
					}
					
				break;
			
				//**********************************************************************************************************************************
				// Menu mode / Menu 1 - Time setting / Set hours
				//**********************************************************************************************************************************
				case FSM_MENU1_HOUR:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU1_HOUR, FSM_MENU1_HOUR, FSM_MENU1_MIN, FSM_MENU1);
					procButtons(&modHour);
					
					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentChar(32);
						LCD_GotoXY(0,3);
						LCD_SentLineC( &timeStr[0] );
						LCD_GotoXY(1,0);
						LCD_SentLineC( &arrov[0] );
					}
					else { LCD_Clear(); lcdClr = 0; }

				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 1 - Time setting / Set minutes
				//**********************************************************************************************************************************
				case FSM_MENU1_MIN:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU1_MIN, FSM_MENU1_MIN, FSM_MENU1_SEC, FSM_MENU1);
					procButtons(&modMin);
					if( lcdClr )
					{ 
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 1 - Time setting / Set seconds
				//**********************************************************************************************************************************
				case FSM_MENU1_SEC:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU1_SEC, FSM_MENU1_SEC, FSM_MENU1_FORMAT, FSM_MENU1);
					procButtons(&modSec);
					if( lcdClr )
					{ 
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 1 - Time setting / Set time fotmat (24H or 12H)
				//**********************************************************************************************************************************
				case FSM_MENU1_FORMAT:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU1_FORMAT, FSM_MENU1_FORMAT, FSM_MENU1_HOUR, FSM_MENU1);
					procButtons(&param[t12HFormat]);
					
					// Parameter limitation
					if(param[t12HFormat] > 250) param[t12HFormat] = 1;
					else if(param[t12HFormat] > 1) param[t12HFormat] = 0;
					
					// Display parameter value
					if(!lcdClr)
					{
						LCD_GotoXY(1, 3);
						LCD_SentLineC( &format );
						LCD_SentLineC( (param[t12HFormat])? PSTR( " 12  "):PSTR(" 24  ") );
					}
					else	// Clear screen
					{
						LCD_Clear();
						lcdClr = 0;
					}
				
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 2 - Date setting
				//**********************************************************************************************************************************
				case FSM_MENU2:
					nextState(&main_fsm, &lcdClr, FULL, FSM_MENU1, FSM_MENU3, FSM_MENU2_DAY, FSM_SHOW_TIME);
				
					if(!lcdClr)
					{
						LCD_GotoXY(0, 0);
						LCD_SentLineC( &arrov[0] );
						LCD_SentLineC( &dateStr[0] );
						LCD_GotoXY(1, 3);
					}
					else
					{	
						writeEEP();
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 2 - Date setting / Set day
				//**********************************************************************************************************************************
				case FSM_MENU2_DAY:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU2_DAY, FSM_MENU2_DAY, FSM_MENU2_MONTH, FSM_MENU2);
					procButtons(&timeData.day);
					
					// Parameter limitation
					t12HFHelper = (29 + (uint8_t)(!timeData.year%4 && timeData.month == 2 ) + (2*(uint8_t)(timeData.month!=2)) + (uint8_t)( (timeData.month != 4) && (timeData.month != 6) && (timeData.month != 9) && (timeData.month != 11) ) );
					
					if( timeData.day < 1 || timeData.day > t12HFHelper ) 
						{timeData.day = 1;}
					else if(timeData.day == t12HFHelper ) 
						{	timeData.day = t12HFHelper - 1;}
					
					if(!lcdClr)		// Clear screen
					{
						LCD_GotoXY(0,0);
						LCD_SentChar(32);
						LCD_GotoXY(0, 3);
						LCD_SentLineC( &dateStr[0] );
						LCD_GotoXY(1, 0);
						LCD_SentLineC( &arrov[0] );
					}
					else
					{
						// Write date to RTC
						RTC_write_date(&timeData);
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 2 - Date setting / Set month
				//**********************************************************************************************************************************
				case FSM_MENU2_MONTH:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU2_MONTH, FSM_MENU2_MONTH, FSM_MENU2_YEAR, FSM_MENU2);
					procButtons(&timeData.month);
					
					if(timeData.month == 0) timeData.month = 12;
					else if(timeData.month > 12) timeData.month = 1;
					
					t12HFHelper = (29 + (uint8_t)(!timeData.year%4 && timeData.month == 2 ) + (2*(uint8_t)(timeData.month!=2)) + (uint8_t)( (timeData.month != 4) && (timeData.month != 6) && (timeData.month != 9) && (timeData.month != 11) ) );
					
					if( timeData.day < 1 || timeData.day >= t12HFHelper )
						{timeData.day = t12HFHelper - 1;}
					
					
					if( lcdClr )
					{
						// Write date to RTC
						RTC_write_date(&timeData);
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 2 - Date setting / Set year
				//**********************************************************************************************************************************
				case FSM_MENU2_YEAR:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU2_YEAR, FSM_MENU2_YEAR, FSM_MENU2_WDAY, FSM_MENU2);
					procButtons(&timeData.year);
					
					if(timeData.year > 250) timeData.year = 99;
					else if(timeData.year > 99) timeData.year = 0;
					
					if( lcdClr )
					{
						// Write date to RTC
						RTC_write_date(&timeData);
						LCD_Clear();
						lcdClr = 0;
					}				
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 2 - Date setting / Set week day
				//**********************************************************************************************************************************
				case FSM_MENU2_WDAY:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU2_WDAY, FSM_MENU2_WDAY, FSM_MENU2_DAY, FSM_MENU2);
					procButtons(&timeData.wday);
				
					if(timeData.wday == 0) timeData.wday = 7;
					else if(timeData.wday > 7) timeData.wday = 1;
					
					LCD_GotoXY(1, 2);
					LCD_SentLineC( &(week[timeData.wday-1][0]) );
					LCD_SentLineC( &space[0] );
					
					if( lcdClr )
					{
						// Write date to RTC
						RTC_write_date(&timeData);
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 3 - Alarm1 setting
				//**********************************************************************************************************************************
				case FSM_MENU3:
					nextState(&main_fsm, &lcdClr, FULL, FSM_MENU2, FSM_MENU4, FSM_MENU3_AL1_HOUR, FSM_SHOW_TIME);
			
					timePtr = &timeData.al1sec;

					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentLineC( &arrov[0] );
						LCD_SentLineC( &al1Str[0] );
						LCD_SentChar( '1' );
					}
					else
					{
						writeEEP();
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 3 - Alarm1 setting / Set hour
				//**********************************************************************************************************************************
				case FSM_MENU3_AL1_HOUR:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU3_AL1_HOUR, FSM_MENU3_AL1_HOUR, FSM_MENU3_AL1_MIN, FSM_MENU3);
					procButtons(&modHour);
	
					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentChar(32);
						LCD_GotoXY(0,3);
						LCD_SentLineC( &al1Str[0] );
						LCD_SentChar( '1' );
						LCD_GotoXY(1,0);
						LCD_SentLineC( &arrov[0] );
					}
					else 
					{
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 3 - Alarm1 setting / Set minutes
				//**********************************************************************************************************************************
				case FSM_MENU3_AL1_MIN:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU3_AL1_MIN, FSM_MENU3_AL1_MIN, FSM_MENU3_AL1_ON, FSM_MENU3);
					procButtons(&modMin);
					
					if( lcdClr )
					{ 
						LCD_Clear(); 
						lcdClr = 0; 
					}					
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 3 - Alarm1 setting / Turn ON / OFF
				//**********************************************************************************************************************************
				case FSM_MENU3_AL1_ON:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU3_AL1_ON, FSM_MENU3_AL1_ON, FSM_MENU3_AL1_HOUR, FSM_MENU3);
					procButtons(&param[alarm1En]);
					
					if(param[alarm1En] > 250 ) param[alarm1En] = 1;
					else if(param[alarm1En] > 1 ) param[alarm1En] = 0;
					
					if( !lcdClr )
					{
						LCD_GotoXY(1,3);	
						if(param[alarm1En]) LCD_SentLineC( &on[0] );
						else LCD_SentLineC( &off[0] );	
						LCD_SentLineC( &space[0] );
					}
					else 
					{
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 4 - Alarm2 setting
				//**********************************************************************************************************************************
				case FSM_MENU4:
					nextState(&main_fsm, &lcdClr, FULL, FSM_MENU3, FSM_MENU5, FSM_MENU4_AL2_HOUR, FSM_SHOW_TIME);
					timePtr = &timeData.al2sec;

					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentLineC( &arrov[0] );
						LCD_SentLineC( &al1Str[0] );
						LCD_SentChar( '2' );
					}
					else	// Clear screen
					{
						writeEEP();
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 4 - Alarm2 setting / Set hour
				//**********************************************************************************************************************************
				case FSM_MENU4_AL2_HOUR:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU4_AL2_HOUR, FSM_MENU4_AL2_HOUR, FSM_MENU4_AL2_MIN, FSM_MENU4);
					procButtons(&modHour);
				
					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentChar(32);
						LCD_GotoXY(0,3);
						LCD_SentLineC( &al1Str[0] );
						LCD_SentChar( '2' );
						LCD_GotoXY(1,0);
						LCD_SentLineC( &arrov[0] );
					}
					else 
					{ 
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 4 - Alarm2 setting / Set minutes
				//**********************************************************************************************************************************
				case FSM_MENU4_AL2_MIN:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU4_AL2_MIN, FSM_MENU4_AL2_MIN, FSM_MENU4_AL2_ON, FSM_MENU4);
					procButtons(&modMin);
					
					if( lcdClr )
					{ 
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 4 - Alarm2 setting / Switch ON / OFF
				//**********************************************************************************************************************************
				case FSM_MENU4_AL2_ON:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU4_AL2_ON, FSM_MENU4_AL2_ON, FSM_MENU4_AL2_HOUR, FSM_MENU4);
					procButtons(&param[alarm2En]);
					
					// Parameter limitation
					if(param[alarm2En] > 250) param[alarm2En] = 1;
					else if(param[alarm2En] > 1) param[alarm2En] = 0;
					
					// Display parameter value
					if( !lcdClr )
					{
						LCD_GotoXY(1,3);
						if(param[alarm2En]) LCD_SentLineC( &on[0] );
						else LCD_SentLineC( &off[0] );
						LCD_SentLineC( &space[0] );
					}
					else	// Clear screen
					{
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 5 - Sensor type setting
				//**********************************************************************************************************************************
				case FSM_MENU5:
					nextState(&main_fsm, &lcdClr, FULL, FSM_MENU4, FSM_MENU6, FSM_MENU5_SENSOR, FSM_SHOW_TIME);
					
					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentLineC( &arrov[0] );
						LCD_SentLineC( &sensorT[0] );
						
						LCD_GotoXY(1,3);
						LCD_SentLineC( &(sensor[modSensor][0]) );
					}
					else // Clear screen
					{
						param[sensorType] = modSensor;
						writeEEP();
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 5 - Sensor type setting / Set sensor type
				//**********************************************************************************************************************************
				case FSM_MENU5_SENSOR:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU5_SENSOR, FSM_MENU5_SENSOR, FSM_MENU5_SENSOR, FSM_MENU5);
					procButtons(&modSensor);
					
					// Parameter limitation
					if(modSensor > 250) modSensor = 1;
					else if(modSensor > 1) modSensor = 0;
					
					if(!lcdClr )	//Clear screen
					{
						LCD_GotoXY(0,0);
						LCD_SentChar(32);
						LCD_GotoXY(1,0);
						LCD_SentLineC( &arrov[0] );
						LCD_SentLineC( &(sensor[modSensor][0]) );
					}
					else 
					{
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 6 - Brightness setting
				//**********************************************************************************************************************************
				case FSM_MENU6:
					nextState(&main_fsm, &lcdClr, FULL, FSM_MENU5, FSM_MENU7, FSM_MENU6_BL_NIGHTT, FSM_SHOW_TIME);
				
					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentLineC( &arrov[0] );
						LCD_SentLineC( &brightness[0] );
					}
					else	// Clear screen
					{
						writeEEP();
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 6 - Brightness setting / Set night mode time
				//**********************************************************************************************************************************
				case FSM_MENU6_BL_NIGHTT:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU6_BL_NIGHTT, FSM_MENU6_BL_NIGHTT, FSM_MENU6_BL_DAYT, FSM_MENU6);
					procButtons(&param[brNightTime]);
					
					// Parameter limitation
					if(param[brNightTime] > 250) param[brNightTime] = 23;
					else if(param[brNightTime] > 23) param[brNightTime] = 0;
					
					// Display parameter value
					LCD_GotoXY(0,0);
					LCD_SentChar(32);
					LCD_GotoXY(1,3);
					LCD_SentLineC( &nTime[0] );
					LCD_SentChar(48 + param[brNightTime]/10);
					LCD_SentChar(48 + param[brNightTime]%10);
					LCD_SentChar(' ');
					LCD_SentChar(' ');
					
					if(lcdClr)	// Clear screen
					{
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 6 - Brightness setting / Set day mode time
				//**********************************************************************************************************************************
				case FSM_MENU6_BL_DAYT:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU6_BL_DAYT, FSM_MENU6_BL_DAYT, FSM_MENU6_BL_NAGHTB, FSM_MENU6);				
					procButtons(&param[brDayTime]);
				
					// Parameter limitation
					if(param[brDayTime] > 250) param[brDayTime] = param[brNightTime];
					else if(param[brDayTime] > param[brNightTime]) param[brDayTime] = 0;
					
					// View parameter value
					LCD_GotoXY(1,3);
					LCD_SentLineC( &dTime[0] );
					LCD_SentChar(48 + param[brDayTime]/10);
					LCD_SentChar(48 + param[brDayTime]%10);
					LCD_SentChar(32);
					LCD_SentChar(' ');
					
					if(lcdClr)	// Clear screen
					{
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 6 - Brightness setting / Set night mode min brightness value
				//**********************************************************************************************************************************
				case FSM_MENU6_BL_NAGHTB:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU6_BL_NAGHTB, FSM_MENU6_BL_NAGHTB, FSM_MENU6_BL_DAYB, FSM_MENU6);
					procButtons(&param[brNight]);
					
					// View parameter value
					LCD_GotoXY(1,3);
					LCD_SentLineC( &nBr[0] );
					LCD_SentChar(48 + param[brNight]/100);
					LCD_SentChar(48 + param[brNight]%100/10);
					LCD_SentChar(48 + param[brNight]%10);
					LCD_SentChar(32);
					LCD_SentChar(' ');
					
					if(lcdClr)	// Clear screen
					{
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 6 - Brightness setting / Set night mode max brightness value 
				//**********************************************************************************************************************************	
				case FSM_MENU6_BL_DAYB:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU6_BL_DAYB, FSM_MENU6_BL_DAYB, FSM_MENU6_BL_NIGHTT, FSM_MENU6);
					procButtons(&param[brDay]);
					
					// View parameter value
					LCD_GotoXY(1,3);
					LCD_SentLineC( &dBr[0] );
					LCD_SentChar(48 + param[brDay]/100);
					LCD_SentChar(48 + param[brDay]%100/10);
					LCD_SentChar(48 + param[brDay]%10);
					LCD_SentChar(' ');
					LCD_SentChar(' ');
					
					if(lcdClr)	// Clear screen
					{
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 7 - View setting 
				//**********************************************************************************************************************************
				case FSM_MENU7:
					nextState(&main_fsm, &lcdClr, FULL, FSM_MENU6, FSM_MENU1, FSM_MENU7_PRESET, FSM_SHOW_TIME);
				
					if(!lcdClr )
					{
						LCD_GotoXY(0,0);
						LCD_SentLineC( &arrov[0] );
						LCD_SentLineC( &preset[0] );
					}
					else	// Clear screen
					{
						writeEEP();
						LCD_Clear();
						lcdClr = 0;
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 7 - View setting / Set view preset (0 - 6)
				//**********************************************************************************************************************************
				//	0	-	No switching between screens
				//	1	-	Switching between screens every swTimer seconds (max 20)
				//	2	-	Switching between Time and Temperature screens every swTimer seconds (max 20)
				//	3	-	Switching between Time and Date screens every swTimer seconds (max 20)
				//	4	-	Switching between screens at end of minute 
				//	5	-	Switching between Time and Temperature screens at end of minute
				//	6	-	Switching between Time and Date screens at end of minute
				case FSM_MENU7_PRESET:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU7_PRESET, FSM_MENU7_PRESET, FSM_MENU7_TIME, FSM_MENU7);
					procButtons(&param[viewPreset]);
					
					// Parameter limitation
					if(param[viewPreset] > 250) param[viewPreset] = 6;
					else if(param[viewPreset] > 6) param[viewPreset] = 0; 
					
					// Display parameter value
					LCD_GotoXY(0,0);
					LCD_SentChar(32);
					LCD_GotoXY(1, 3);
					LCD_SentLineC( &preset );
					LCD_SentLineC( PSTR(" - ") );
					LCD_SentChar(48 + param[viewPreset]%10);
					LCD_SentLineC( &space[0] );
					
					if(lcdClr)	//Clear screen
					{
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
				
				//**********************************************************************************************************************************
				// Menu mode / Menu 7 - View setting / Set switching timer
				//**********************************************************************************************************************************
				case FSM_MENU7_TIME:
					nextState(&main_fsm, &lcdClr, TRUNC, FSM_MENU7_PRESET, FSM_MENU7_PRESET, FSM_MENU7_PRESET, FSM_MENU7);
					procButtons(&param[tempRetTime]);
					
					// Parameter limitation
					if(param[tempRetTime] > 250) param[tempRetTime] = 20;
					else if(param[tempRetTime] > 20) param[tempRetTime] = 0; 
					
					// Display parameter value
					LCD_GotoXY(1, 3);
					LCD_SentLineC( &swTime[0] );
					LCD_SentChar(48 + param[tempRetTime]/10);
					LCD_SentChar(48 + param[tempRetTime]%10);
					
					if(lcdClr) //Clear screen
					{
						LCD_Clear(); 
						lcdClr = 0; 
					}
				break;
			}
			
			// If normal mode
			if(!alarm)
			{
				// If night mode
				if( (timeData.hour >= param[brNightTime] || timeData.hour < param[brDayTime]) && param[brNightTime] != param[brDayTime])
				{
					brArr[brPtr] = ADCW;
					brPtr++;
					brPtr &= 0x0F;
					brTemporal = 0;
					for(uint8_t i = 0; i < 16; i++)
					{
						brTemporal += brArr[i];
					}
					brTemporal = brTemporal >> 5;
					if(brTemporal > 255) brTemporal = 255;
					else if(brTemporal < param[brNight]) brTemporal = param[brNight];
					OCR1A = (uint8_t)brTemporal;
					ADCSRA |= (1 << ADSC);
				}
				else{ OCR1A = 255;}	//If day mode				
			}
			
			// IF time was modified				
			if(modSec || modMin || modHour)
			{
				if(modSec)
				{
					*timePtr += modSec;
					if(	*timePtr > 250 ) *timePtr = 59;
					else if(*timePtr > 59 ) *timePtr = 0;
					modSec = 0;	
				}
				if(modMin)
				{
					*(timePtr + 1) += modMin;
					if(	*(timePtr + 1) > 250 ) *(timePtr + 1) = 59;
					else if(*(timePtr + 1) > 59 ) *(timePtr + 1) = 0;
					modMin = 0;	
				}
				if(modHour)
				{
					*(timePtr + 2) += modHour;
					if(	*(timePtr + 2) > 250 ) *(timePtr + 2) = 23;
					else if( *(timePtr + 2) > 23 ) *(timePtr + 2) = 0;
					modHour = 0;
				}
				RTC_write_time(&timeData);
				RTC_write_alarm(&timeData);
			}
			
			// Display time in menu (for Time, Alarm1 and Alarm2)
			if(((main_fsm & 0xF0) == 0x10 ||
				(main_fsm & 0xF0) == 0x30 ||
				(main_fsm & 0xF0) == 0x40  ) &&
			   ((main_fsm & 0x0F) == 0x00 ||
			    (main_fsm & 0x0F) == 0x01 ||
				(main_fsm & 0x0F) == 0x02 ||
				(main_fsm & 0x0F) == 0x03  ) )
			{
				LCD_GotoXY(1, 3);
				
				// Blinking in menu
				if( ( subFrame > 32 && (main_fsm & 0x0F) == 0x01) || (main_fsm & 0x0F) == 0x00 ||  (main_fsm & 0x0F) == 0x02 ||  (main_fsm & 0x0F) == 0x03 )
				{
					if(param[t12HFormat])
					{
						t12HFHelper = *(timePtr+2);
						if(t12HFHelper == 0) t12HFHelper = 12;
						else if(t12HFHelper > 12) t12HFHelper -= 12;
						LCD_SentChar(( t12HFHelper/10 )? 48 + t12HFHelper/10 : ' ');
						LCD_SentChar(48 + t12HFHelper%10);
					}
					else
					{
						LCD_SentChar(( *(timePtr+2)/10 )? 48 + *(timePtr+2)/10 : ' ');
						LCD_SentChar(48 + *(timePtr+2)%10);
					}					
				}
				else
				{
					LCD_SentChar(' ');
					LCD_SentChar(' ');
				}
				LCD_SentChar(':');
				
				// Blinking in menu
				if( ( subFrame > 32 && (main_fsm & 0x0F) == 0x02) || (main_fsm & 0x0F) == 0x00  ||  (main_fsm & 0x0F) == 0x01 ||  (main_fsm & 0x0F) == 0x03 )
				{
					LCD_SentChar(48 + *(timePtr+1)/10);
					LCD_SentChar(48 + *(timePtr+1)%10);
				}
				else
				{
					LCD_SentChar(' ');
					LCD_SentChar(' ');
				}
				LCD_SentChar(':');
				
				// Blinking in menu
				if( (subFrame > 32 && (main_fsm & 0x0F) == 0x03) || (main_fsm & 0x0F) == 0x00  ||  (main_fsm & 0x0F) == 0x01 ||  (main_fsm & 0x0F) == 0x02 )
				{
					LCD_SentChar(48 + *(timePtr+0)/10);
					LCD_SentChar(48 + *(timePtr+0)%10);
				}
				else
				{
					LCD_SentChar(' ');
					LCD_SentChar(' ');
				}
				if(param[t12HFormat])
				{
					LCD_SentChar(' ');
					LCD_SentLineC((*(timePtr+2) > 12 || *(timePtr+2) == 0)? &am[0] : &pm[0] );
				}
				else LCD_SentLineC( &space[0] );					
			}
			
			// Show date in View Date and Menu Date modes
			if( main_fsm == 0x02 ||
			   (main_fsm >= 0x20 && main_fsm < 0x30) )
			{
				LCD_GotoXY(1, 3);
				if( subFrame > 32 || main_fsm != 0x23 )		// Blinking in menu
				{
					LCD_SentChar(48 + timeData.day/10);
					LCD_SentChar(48 + timeData.day%10);
				}
				else LCD_SentLineC( PSTR("  ") );
								
				LCD_SentChar('-');
				
				if( subFrame > 32 || main_fsm != 0x22 )		// Blinking in menu
				{
					LCD_SentChar(48 + timeData.month/10);
					LCD_SentChar(48 + timeData.month%10);
				}
				else LCD_SentLineC( PSTR("  ") );
				LCD_SentChar('-');
				
				if( subFrame > 32 || main_fsm != 0x21 )		// Blinking in menu
				{
					LCD_SentChar('2');
					LCD_SentChar('0');
					LCD_SentChar(48 + timeData.year/10);
					LCD_SentChar(48 + timeData.year%10);
				}
				else LCD_SentLineC( &space[0] );					
			}							
		}		
		asm("nop");
    }
}

// Big digits mode
void printTime(void)
{
	uint8_t helper = timeData.hour;
	if(param[t12HFormat])								// If time format 12H
	{
		// time value correction 
		if( helper == 0) helper = 12;				
		else if( helper > 12) helper -= 12;
	}	 
	
	// Print hours
	print3x2s( (helper/10 + 1) * (helper/10 != 0), 0);
	print3x2s( helper%10 + 1, 3);
	
	// Print minutes		
	print3x2s( timeData.min/10 + 1, 7);
	print3x2s( timeData.min%10 + 1, 10);
	
	// Print dots or rings (only for Cyrillic displays)
	LCD_GotoXY(0, 6);
	LCD_SentChar( (timeData.sec%2)? (param[alarm1En])? BELL:DOT:' ' );
	LCD_GotoXY(1, 6);
	LCD_SentChar( (timeData.sec%2)? (param[alarm2En])? BELL:DOT:' ' );			
	
	// Print seconds
	LCD_GotoXY(0, 14);
	LCD_SentChar((48 + timeData.sec/10)*(timeData.sec/10 != 0) + (32*(timeData.sec/10 == 0)));
	LCD_SentChar( 48 + timeData.sec%10);
	
	// Print PM or AM if time mode 12H
	if(param[t12HFormat])
	{
		LCD_GotoXY(1, 14);
		LCD_SentLineC( (timeData.hour > 12 || timeData.hour == 0)? &am[0] : &pm[0] );
	}		
}	

// Print big digit from segments in GRAM
void print3x2s(char symbol, char place)
{
	switch (symbol)
	{
		case 0://EMPTY
		LCD_GotoXY(0,place);
		LCD_SentChar(32);
		LCD_SentChar(32);
		LCD_SentChar(32);
		LCD_GotoXY(1, place);
		LCD_SentChar(32);
		LCD_SentChar(32);
		LCD_SentChar(32);
		break;
		
		case 1://0
		LCD_GotoXY(0,place);
		LCD_SentChar(0);
		LCD_SentChar(2);
		LCD_SentChar(1);
		LCD_GotoXY(1, place);
		LCD_SentChar(0);
		LCD_SentChar(3);
		LCD_SentChar(1);
		break;
		
		case 2://1
		LCD_GotoXY(0,place);
		LCD_SentChar(32);
		LCD_SentChar(255);
		LCD_SentChar(32);
		LCD_GotoXY(1, place);
		LCD_SentChar(32);
		LCD_SentChar(255);
		LCD_SentChar(32);
		break;
		
		case 3://2
		LCD_GotoXY(0,place);
		LCD_SentChar(6);
		LCD_SentChar(4);
		LCD_SentChar(1);
		LCD_GotoXY(1, place);
		LCD_SentChar(0);
		LCD_SentChar(3);
		LCD_SentChar(3);
		break;
		
		case 4://3
		LCD_GotoXY(0,place);
		LCD_SentChar(6);
		LCD_SentChar(4);
		LCD_SentChar(1);
		LCD_GotoXY(1, place);
		LCD_SentChar(7);
		LCD_SentChar(3);
		LCD_SentChar(1);
		break;
		
		case 5://4
		LCD_GotoXY(0,place);
		LCD_SentChar(0);
		LCD_SentChar(3);
		LCD_SentChar(1);
		LCD_GotoXY(1, place);
		LCD_SentChar(32);
		LCD_SentChar(32);
		LCD_SentChar(1);
		break;
		
		case 6://5
		LCD_GotoXY(0,place);
		LCD_SentChar(0);
		LCD_SentChar(4);
		LCD_SentChar(5);
		LCD_GotoXY(1, place);
		LCD_SentChar(7);
		LCD_SentChar(3);
		LCD_SentChar(1);
		break;
		
		case 7://6
		LCD_GotoXY(0,place);
		LCD_SentChar(0);
		LCD_SentChar(4);
		LCD_SentChar(5);
		LCD_GotoXY(1, place);
		LCD_SentChar(0);
		LCD_SentChar(3);
		LCD_SentChar(1);
		break;
		
		case 8://7
		LCD_GotoXY(0,place);
		LCD_SentChar(2);
		LCD_SentChar(2);
		LCD_SentChar(1);
		LCD_GotoXY(1, place);
		LCD_SentChar(32);
		LCD_SentChar(32);
		LCD_SentChar(1);
		break;
		
		case 9://8
		LCD_GotoXY(0,place);
		LCD_SentChar(0);
		LCD_SentChar(4);
		LCD_SentChar(1);
		LCD_GotoXY(1, place);
		LCD_SentChar(0);
		LCD_SentChar(3);
		LCD_SentChar(1);
		break;
		
		case 10://9
		LCD_GotoXY(0,place);
		LCD_SentChar(0);
		LCD_SentChar(4);
		LCD_SentChar(1);
		LCD_GotoXY(1, place);
		LCD_SentChar(7);
		LCD_SentChar(3);
		LCD_SentChar(1);
		break;
	}
}

//
void procButtons(uint8_t *data)
{
	#ifdef MENU_RETURN
		if(upButton.state == clickSt)  {upButton.state = freeSt;  *data = *data + 1; menuRetTime = 60;}
		else if(downButton.state == clickSt){downButton.state = freeSt; *data = *data - 1; menuRetTime = 60;}
		else if(upButton.state == holdSt   ||   upButton.state == wasteSt){upButton.state = wasteSt;  if(!subFrame%16) *data = *data + 1;  menuRetTime = 60;}
		else if(downButton.state == holdSt || downButton.state == wasteSt){downButton.state = wasteSt;if(!subFrame%16) *data = *data - 1;  menuRetTime = 60;}
	#else
		if(upButton.state == clickSt)  {upButton.state = freeSt;  *data = *data + 1;}
		else if(downButton.state == clickSt){downButton.state = freeSt; *data = *data - 1;}
		else if(upButton.state == holdSt   ||   upButton.state == wasteSt){upButton.state = wasteSt;  if(!subFrame%16) *data = *data + 1;}
		else if(downButton.state == holdSt || downButton.state == wasteSt){downButton.state = wasteSt;if(!subFrame%16) *data = *data - 1;}
	#endif
}

// 
void nextState(uint8_t *st, uint8_t *helper, uint8_t type, uint8_t prev, uint8_t next, uint8_t ok, uint8_t hold)
{
	#ifdef MENU_RETURN
		if(menuRetTime == 0)					{*st = 0; *helper = 1; menuRetTime = 60;}
		else if(okButton.state == clickSt)		{okButton.state = freeSt;  *st = ok; swTimer = param[tempRetTime]; menuRetTime = 60;}
		else if(okButton.state == holdSt)		{okButton.state = wasteSt; *st = hold; *helper = 1; swTimer = param[tempRetTime]; menuRetTime = 60;}
		if(type == FULL)
		{
			if(upButton.state == clickSt)		{upButton.state = freeSt; *st = prev; *helper = 1; menuRetTime = 60;}
			else if(downButton.state == clickSt){downButton.state = freeSt; *st = next; *helper = 1; menuRetTime = 60;}
			else if(upButton.state == holdSt || downButton.state == holdSt) {upButton.state = wasteSt; downButton.state = wasteSt; menuRetTime = 60;}
		}
	#else
		if(okButton.state == clickSt)		{okButton.state = freeSt;  *st = ok; swTimer = param[tempRetTime];}
		else if(okButton.state == holdSt)		{okButton.state = wasteSt; *st = hold; *helper = 1; swTimer = param[tempRetTime];}
		if(type == FULL)
		{
			if(upButton.state == clickSt)		{upButton.state = freeSt; *st = prev; *helper = 1;}
			else if(downButton.state == clickSt){downButton.state = freeSt; *st = next; *helper = 1;}
			else if(upButton.state == holdSt || downButton.state == holdSt) {upButton.state = wasteSt; downButton.state = wasteSt;}
		}
	#endif
	
}

// Write user parameters to EEPROM
void writeEEP()												// Writing parameter bit in EEPROM if the last one was changed
{
	for(uint8_t j = 0; j < 10; j++)
	{
		eeprom_busy_wait();									// Waite if EEPROM is busy
		if( param[j] != eeprom_read_byte(&eeconst[j]) )		// If parameter byte was changed
		{
			eeprom_busy_wait();
			eeprom_write_byte(&eeconst[j], param[j]);		// Rewrite parameter byte to the EEPROM
		}		
	}
}

// Set up music
void setUpCTC()
{
	OCR1A = 0;
	TCCR1A = (1 << COM1B0);
	TCCR1B = (1 << CS11)|(1 << WGM12);
	OCR1A = eeprom_read_word(music[0]);
	PORTB |= 0x02;	
}