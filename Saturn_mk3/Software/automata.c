#include "automata.h"
#include "font.h"
#include "Labels.h"



//------------------------------------------------------------------------------------------------------------
//
//											FSM STATES ARRAY
//
//------------------------------------------------------------------------------------------------------------

void(*gus1)(void) = &inc_time;
void(*gus2)(void) = &dec_time;

void gusket_1(void)
{
	gus1();
}

void gusket_2(void)
{
	gus2();
}

const state_ptr state[] = 
	{	print_hour_st, 
		print_menu_time_st,
		print_menu_time_set_st,
		print_menu_date_st,
		print_menu_date_set_st,
		print_menu_al_1_st,
		print_menu_al_1_set_st,
		print_menu_al_2_st,
		print_menu_al_2_set_st,
		print_menu_sensor_st,
		print_menu_sensor_set_st,
		print_menu_brts_st,
		print_menu_brts_set_st,
		print_menu_template_st,
		print_menu_template_set_st,
		print_menu_font_st,
		print_menu_font_set_st,
		fail_st,
		fail_st,
		inc_indirectly_st,
		dec_indirectly_st,
		inc_time,
		dec_time,
		inc_date,
		dec_date,
		inc_interface_st,
		dec_interface_st,
		gusket_1,
		gusket_2,
	};

//------------------------------------------------------------------------------------------------------------
//
//											FSM TRANSITION MAP
//
//------------------------------------------------------------------------------------------------------------


const state_codes_t transition_table[ST_MAX][E_MAX] =
{
	[ST_SHOW_HOURS]			[E_OK_BUTTON_CLICK] 	= ST_INC_INDIRECTLY,
	[ST_SHOW_HOURS]			[E_OK_BUTTON_HOLD]		= ST_MENU_TIME,
	
	[ST_MENU_TIME]			[E_UP_BUTTON_CLICK]		= ST_MENU_FONT,
	[ST_MENU_TIME]			[E_UP_BUTTON_HOLD]		= ST_MENU_FONT,
	[ST_MENU_TIME]			[E_OK_BUTTON_CLICK]		= ST_MENU_SET_TIME,
	[ST_MENU_TIME]			[E_OK_BUTTON_HOLD]		= ST_SHOW_HOURS,
	[ST_MENU_TIME]			[E_DOWN_BUTTON_CLICK]	= ST_MENU_DATE,
	[ST_MENU_TIME]			[E_DOWN_BUTTON_HOLD]	= ST_MENU_DATE,
	
	[ST_MENU_SET_TIME]		[E_UP_BUTTON_CLICK]		= ST_INC_TIME,
	[ST_MENU_SET_TIME]		[E_UP_BUTTON_HOLD]		= ST_INC_TIME,
	[ST_MENU_SET_TIME]		[E_OK_BUTTON_CLICK]		= ST_DEC_INDIRECTLY,
	[ST_MENU_SET_TIME]		[E_OK_BUTTON_HOLD]		= ST_MENU_TIME,
	[ST_MENU_SET_TIME]		[E_DOWN_BUTTON_CLICK]	= ST_DEC_TIME,
	[ST_MENU_SET_TIME]		[E_DOWN_BUTTON_HOLD]	= ST_DEC_TIME,
	
	[ST_MENU_DATE]			[E_UP_BUTTON_CLICK]		= ST_MENU_TIME,
	[ST_MENU_DATE]			[E_UP_BUTTON_HOLD]		= ST_MENU_TIME,
	[ST_MENU_DATE]			[E_OK_BUTTON_CLICK]		= ST_MENU_SET_DATE,
	[ST_MENU_DATE]			[E_OK_BUTTON_HOLD]		= ST_SHOW_HOURS,
	[ST_MENU_DATE]			[E_DOWN_BUTTON_CLICK]	= ST_MENU_ALARM1,
	[ST_MENU_DATE]			[E_DOWN_BUTTON_HOLD]	= ST_MENU_ALARM1,

	[ST_MENU_SET_DATE]		[E_UP_BUTTON_CLICK]		= ST_INC_DATE,
	[ST_MENU_SET_DATE]		[E_UP_BUTTON_HOLD]		= ST_INC_DATE,
	[ST_MENU_SET_DATE]		[E_OK_BUTTON_CLICK]		= ST_DEC_INDIRECTLY,
	[ST_MENU_SET_DATE]		[E_OK_BUTTON_HOLD]		= ST_MENU_DATE,
	[ST_MENU_SET_DATE]		[E_DOWN_BUTTON_CLICK]	= ST_DEC_DATE,
	[ST_MENU_SET_DATE]		[E_DOWN_BUTTON_HOLD]	= ST_DEC_DATE,
	
	[ST_MENU_ALARM1]		[E_UP_BUTTON_CLICK]		= ST_MENU_DATE,
	[ST_MENU_ALARM1]		[E_UP_BUTTON_HOLD]		= ST_MENU_DATE,
	[ST_MENU_ALARM1]		[E_OK_BUTTON_HOLD]		= ST_SHOW_HOURS,
	[ST_MENU_ALARM1]		[E_OK_BUTTON_CLICK]		= ST_MENU_SET_AL1,
	[ST_MENU_ALARM1]		[E_DOWN_BUTTON_CLICK]	= ST_MENU_ALARM2,
	[ST_MENU_ALARM1]		[E_DOWN_BUTTON_HOLD]	= ST_MENU_ALARM2,
	
	[ST_MENU_SET_AL1]		[E_UP_BUTTON_CLICK]		= ST_GUSKET_1,
	[ST_MENU_SET_AL1]		[E_UP_BUTTON_HOLD]		= ST_GUSKET_1,
	[ST_MENU_SET_AL1]		[E_OK_BUTTON_CLICK]		= ST_DEC_INDIRECTLY,
	[ST_MENU_SET_AL1]		[E_OK_BUTTON_HOLD]		= ST_MENU_ALARM1,
	[ST_MENU_SET_AL1]		[E_DOWN_BUTTON_CLICK]	= ST_GUSKET_2,
	[ST_MENU_SET_AL1]		[E_DOWN_BUTTON_HOLD]	= ST_GUSKET_2,	
	
	[ST_MENU_ALARM2]		[E_UP_BUTTON_CLICK]		= ST_MENU_ALARM1,
	[ST_MENU_ALARM2]		[E_UP_BUTTON_HOLD]		= ST_MENU_ALARM1,
	[ST_MENU_ALARM2]		[E_OK_BUTTON_HOLD]		= ST_SHOW_HOURS,
	[ST_MENU_ALARM2]		[E_OK_BUTTON_CLICK]		= ST_MENU_SET_AL2,
	[ST_MENU_ALARM2]		[E_DOWN_BUTTON_CLICK]	= ST_MENU_SENSOR,
	[ST_MENU_ALARM2]		[E_DOWN_BUTTON_HOLD]	= ST_MENU_SENSOR,
	
	[ST_MENU_SET_AL2]		[E_UP_BUTTON_CLICK]		= ST_INC_TIME,
	[ST_MENU_SET_AL2]		[E_UP_BUTTON_HOLD]		= ST_INC_TIME,
	[ST_MENU_SET_AL2]		[E_OK_BUTTON_CLICK]		= ST_DEC_INDIRECTLY,
	[ST_MENU_SET_AL2]		[E_OK_BUTTON_HOLD]		= ST_MENU_ALARM2,
	[ST_MENU_SET_AL2]		[E_DOWN_BUTTON_CLICK]	= ST_DEC_TIME,
	[ST_MENU_SET_AL2]		[E_DOWN_BUTTON_HOLD]	= ST_DEC_TIME,
	
	[ST_MENU_SENSOR]		[E_UP_BUTTON_CLICK]		= ST_MENU_ALARM2,
	[ST_MENU_SENSOR]		[E_UP_BUTTON_HOLD]		= ST_MENU_ALARM2,
	[ST_MENU_SENSOR]		[E_OK_BUTTON_HOLD]		= ST_SHOW_HOURS,
	[ST_MENU_SENSOR]		[E_OK_BUTTON_CLICK]		= ST_MENU_SET_SENSOR,
	[ST_MENU_SENSOR]		[E_DOWN_BUTTON_CLICK]	= ST_MENU_BRIGHTNESS,
	[ST_MENU_SENSOR]		[E_DOWN_BUTTON_HOLD]	= ST_MENU_BRIGHTNESS,
	
	[ST_MENU_SET_SENSOR]	[E_UP_BUTTON_CLICK]		= ST_INC_INDIRECTLY,
	[ST_MENU_SET_SENSOR]	[E_UP_BUTTON_HOLD]		= ST_MENU_SET_SENSOR,
	[ST_MENU_SET_SENSOR]	[E_OK_BUTTON_CLICK]		= ST_MENU_SET_SENSOR,
	[ST_MENU_SET_SENSOR]	[E_OK_BUTTON_HOLD]		= ST_MENU_SENSOR,
	[ST_MENU_SET_SENSOR]	[E_DOWN_BUTTON_CLICK]	= ST_DEC_INDIRECTLY,
	[ST_MENU_SET_SENSOR]	[E_DOWN_BUTTON_HOLD]	= ST_MENU_SET_SENSOR,	
		
	[ST_MENU_BRIGHTNESS]	[E_UP_BUTTON_CLICK]		= ST_MENU_SENSOR,
	[ST_MENU_BRIGHTNESS]	[E_UP_BUTTON_HOLD]		= ST_MENU_SENSOR,
	[ST_MENU_BRIGHTNESS]	[E_OK_BUTTON_HOLD]		= ST_SHOW_HOURS,
	[ST_MENU_BRIGHTNESS]	[E_OK_BUTTON_CLICK]		= ST_MENU_SET_BRIGHTNESS,
	[ST_MENU_BRIGHTNESS]	[E_DOWN_BUTTON_CLICK]	= ST_MENU_TEMPLATE,
	[ST_MENU_BRIGHTNESS]	[E_DOWN_BUTTON_HOLD]	= ST_MENU_TEMPLATE,
	
	[ST_MENU_SET_BRIGHTNESS][E_UP_BUTTON_CLICK]		= ST_INC_INDIRECTLY,
	[ST_MENU_SET_BRIGHTNESS][E_UP_BUTTON_HOLD]		= ST_INC_INDIRECTLY,
	[ST_MENU_SET_BRIGHTNESS][E_OK_BUTTON_CLICK]		= ST_INC_INTERFACE,
	[ST_MENU_SET_BRIGHTNESS][E_OK_BUTTON_HOLD]		= ST_MENU_BRIGHTNESS,
	[ST_MENU_SET_BRIGHTNESS][E_DOWN_BUTTON_CLICK]	= ST_DEC_INDIRECTLY,
	[ST_MENU_SET_BRIGHTNESS][E_DOWN_BUTTON_HOLD]	= ST_DEC_INDIRECTLY,	
	
	[ST_MENU_TEMPLATE]		[E_UP_BUTTON_CLICK]		= ST_MENU_BRIGHTNESS,
	[ST_MENU_TEMPLATE]		[E_OK_BUTTON_HOLD]		= ST_SHOW_HOURS,
	[ST_MENU_TEMPLATE]		[E_OK_BUTTON_CLICK]		= ST_MENU_SET_TEMPLATE,
	[ST_MENU_TEMPLATE]		[E_DOWN_BUTTON_CLICK]	= ST_MENU_FONT,
	
	[ST_MENU_SET_TEMPLATE]	[E_UP_BUTTON_CLICK]		= ST_INC_INDIRECTLY,
	[ST_MENU_SET_TEMPLATE]	[E_UP_BUTTON_HOLD]		= ST_INC_INDIRECTLY,
	[ST_MENU_SET_TEMPLATE]	[E_OK_BUTTON_CLICK]		= ST_MENU_SET_TEMPLATE,
	[ST_MENU_SET_TEMPLATE]	[E_OK_BUTTON_HOLD]		= ST_MENU_TEMPLATE,
	[ST_MENU_SET_TEMPLATE]	[E_DOWN_BUTTON_CLICK]	= ST_DEC_INDIRECTLY,
	[ST_MENU_SET_TEMPLATE]	[E_DOWN_BUTTON_HOLD]	= ST_DEC_INDIRECTLY,	
	
	[ST_MENU_FONT]			[E_UP_BUTTON_CLICK]		= ST_MENU_TEMPLATE,
	[ST_MENU_FONT]			[E_OK_BUTTON_HOLD]		= ST_SHOW_HOURS,
	[ST_MENU_FONT]			[E_OK_BUTTON_CLICK]		= ST_MENU_SET_FONT,
	[ST_MENU_FONT]			[E_DOWN_BUTTON_CLICK]	= ST_MENU_TIME,
	
	[ST_MENU_SET_FONT]		[E_UP_BUTTON_CLICK]		= ST_INC_INDIRECTLY,
	[ST_MENU_SET_FONT]		[E_UP_BUTTON_HOLD]		= ST_INC_INDIRECTLY,
	[ST_MENU_SET_FONT]		[E_OK_BUTTON_CLICK]		= ST_INC_INTERFACE,
	[ST_MENU_SET_FONT]		[E_OK_BUTTON_HOLD]		= ST_MENU_FONT,
	[ST_MENU_SET_FONT]		[E_DOWN_BUTTON_CLICK]	= ST_DEC_INDIRECTLY,
	[ST_MENU_SET_FONT]		[E_DOWN_BUTTON_HOLD]	= ST_DEC_INDIRECTLY,
};

//------------------------------------------------------------------------------------------------------------
//
//											SERVICE FUNCTIONS
//
//------------------------------------------------------------------------------------------------------------

#define OK_BUTTON_WORKED_OUT if(button_ok.state == BT_HOLD) button_ok.state = BT_WORKED_OUT
#define UP_BUTTON_WORKED_OUT if(button_up.state == BT_HOLD) button_up.state = BT_WORKED_OUT
#define DOWN_BUTTON_WORKED_OUT if(button_down.state == BT_HOLD) button_down.state = BT_WORKED_OUT

void allign_time(void);
void allign_date(void);
void process_time(void);
void draw_screen(Screen*); 
void print_str(Screen*, char*); 
void print_moving_str(Screen*, char*, uint8_t, char*);
void print_shaking_str(Screen*, char*, uint8_t, char*);
void print_char(Screen*, char); 

void allign_time()
{
	// Allign time format 
	// 0b0 - 24H format 
	// 0b1 - 12H format
	(*(device_data_g.time_pointer+3)) &= 1;
	
	//Allign seconds
	if((*(device_data_g.time_pointer)) > 127)	//Equivalent > 0
	{
		// If seconds < 0 -> seconds = 59; minutes --;
		(*(device_data_g.time_pointer)) = 59;
		(*(device_data_g.time_pointer+1))--;
	}
	else if((*(device_data_g.time_pointer)) > 59)
	{
		// If seconds > 59 -> seconds = 0; minutes ++;
		(*(device_data_g.time_pointer)) = 0;
		(*(device_data_g.time_pointer+1))++;
	}
	
	// Allign minutes
	if((*(device_data_g.time_pointer+1)) > 127) //Equivalent > 0
	{
		// If minutes < 0 -> minutes = 59; hours --;
		(*(device_data_g.time_pointer+1)) = 59;
		(*(device_data_g.time_pointer+2))--;
	}
	else if((*(device_data_g.time_pointer+1)) > 59)
	{
		// If minutes > 23 -> minutes = 0; hours ++;
		(*(device_data_g.time_pointer+1)) = 0;
		(*(device_data_g.time_pointer+2))++;
	}
	
	//Allign hours
	if((*(device_data_g.time_pointer+2)) > 127) //Equivalent > 0
	{
		(*(device_data_g.time_pointer+2)) = 23;
	}
	else if((*(device_data_g.time_pointer+2)) > 23)
	{
		(*(device_data_g.time_pointer+2)) = 0;
	}
	return;
}

void allign_date()
{
	// Allign weekday
	if((*(device_data_g.time_pointer)) < 1)
	{
		(*(device_data_g.time_pointer)) = 1;
	}
	else if((*(device_data_g.time_pointer)) > 7)
	{
		(*(device_data_g.time_pointer)) = 7;
	}
	
	// Allign month day
	if((*(device_data_g.time_pointer+1)) < 1)
	{
		(*(device_data_g.time_pointer+1)) = 1;
	}
	else if((*(device_data_g.time_pointer+1)) > 31)
	{
		(*(device_data_g.time_pointer+1)) = 31;
	}
	
	//Allign month
	if((*(device_data_g.time_pointer+2)) < 1)
	{
		(*(device_data_g.time_pointer+2)) = 1;
	}
	else if((*(device_data_g.time_pointer+2)) > 12)
	{
		(*(device_data_g.time_pointer+2)) = 12;
	}
	
	//Allign year
	if((*(device_data_g.time_pointer+3)) > 127)
	{
		(*(device_data_g.time_pointer+3)) = 0;
	}
	else if((*(device_data_g.time_pointer+3)) > 99)
	{
		(*(device_data_g.time_pointer+3)) = 99;
	}
	
	return;
}

//Common ending of Inc/Dec time and alarms 
void process_time(void)
{
	allign_time();
	if(device_data_g.menu_mode == 0) 	//If time setting menu screen
	{	
		DS3231_write_time((void*)&RTC_data);
	}
	add_event(E_GO_BACK);
}

void inc_time(void)
{
	//--------------------------------------------------------------
	// This function works with time settings and alarm settings
	//--------------------------------------------------------------
	// time_pointer -> timeStructure
	// time_menu_mode:
	//		0 - seconds;
	//		1 - minutes;
	//		2 - hours;
	//		3 - time format;
	//---------------------------------------------------------------
	(*(device_data_g.time_pointer + device_data_g.time_menu_mode))++;
	process_time();
}

void dec_time(void)
{
	//--------------------------------------------------------------
	// This function works with time settings and alarm settings
	//--------------------------------------------------------------
	// time_pointer -> timeStructure
	// time_menu_mode:
	//		0 - seconds;
	//		1 - minutes;
	//		2 - hours;
	//		3 - time format;
	//---------------------------------------------------------------
	(*(device_data_g.time_pointer + device_data_g.time_menu_mode))--;
	process_time();
}

void inc_date(void)
{
	(*(device_data_g.time_pointer + device_data_g.date_menu_mode))++;
	allign_date();
	DS3231_write_date((void*)&RTC_data);
	add_event(E_GO_BACK);
}

void dec_date(void)
{
	(*(device_data_g.time_pointer + device_data_g.date_menu_mode))--;
	allign_date();
	DS3231_write_date((void*)&RTC_data);
	add_event(E_GO_BACK);
}

void inc_indirectly_st(void)
{
	(*device_data_g.data_pointer)++;
	add_event(E_GO_BACK);
}

void dec_indirectly_st(void)
{
	(*device_data_g.data_pointer)--;
	add_event(E_GO_BACK);
}

void inc_interface_st(void)
{
	(*device_data_g.interface_pointer)++;
	add_event(E_GO_BACK);
}

void dec_interface_st(void)
{
	(*device_data_g.interface_pointer)--;
	add_event(E_GO_BACK);
}

void draw_screen(Screen* screen)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		switch(device_data_g.time_mode)
		{
		case 1:
		screen->draw[i] = *((digits[device_data_g.font_number]) + (RTC_data.second/10)*8 + i)<<6  |
						  *((digits[device_data_g.font_number]) + (RTC_data.second%10)*8 + i)  ;
		break;
		case 0:
		screen->draw[i] = *((digits[device_data_g.font_number]) + (RTC_data.hour/10)*8 + i)<<19 |
						  *((digits[device_data_g.font_number]) + (RTC_data.hour%10)*8 + i)<<13 |
						   0x3*(uint8_t)(device_data_g.dot && (i == 7) && !(device_data_g.font_number&2)) <<11 |
						   0x2*(uint8_t)(device_data_g.dot && ((i == 2) || (i == 5)) && device_data_g.font_number&2) <<11 |
						  *((digits[device_data_g.font_number]) + (RTC_data.minute/10)*8 + i)<<6  |
						  *((digits[device_data_g.font_number]) + (RTC_data.minute%10)*8 + i)  ;
		break;
		case 2:
		screen->draw[i] = 	0x1*(uint8_t)(i == 7) <<23 |
							*(digits[device_data_g.font_number] + (RTC_data.day/10)*8 + i)<<18 |
							*(digits[device_data_g.font_number] + (RTC_data.day%10)*8 + i)<<12 |
							0x1*(uint8_t)(i == 7) <<11 |
							*(digits[device_data_g.font_number] + (RTC_data.month/10)*8 + i)<<6  |
							*(digits[device_data_g.font_number] + (RTC_data.month%10)*8 + i)  ;
		break;
		case 3:
		screen->draw[i] = 	*(digits[device_data_g.font_number] + (2)*8 + i)<<18 |
							*(digits[device_data_g.font_number] + (0)*8 + i)<<12 |
							*(digits[device_data_g.font_number] + (RTC_data.year/10)*8 + i)<<6  |
							*(digits[device_data_g.font_number] + (RTC_data.year%10)*8 + i)  ;
		break;
		}
	}
}
 
void print_str(Screen* screen, char* string)
{
	while(*string)
	{
		for(uint8_t i = 0; i < 8; i++)
		{
			screen->draw[i] = ((screen->draw[i] << 6) | ascii_5x7_basic_rows[((*string) - 32)][i]);
		}
		string++;
	}
}

void print_str_number(Screen* screen, char* string, uint8_t num)
{
	print_str(screen, string);
	print_char(&screen_draw, 48+num/10);
	print_char(&screen_draw, 48+num%10);
}

void print_moving_str(Screen* screen, char* string, uint8_t id, char *font)
{
	static uint8_t  current_id = 255;
	static uint8_t  current_size;
	static int16_t current_frame;
	static int8_t 	symbol;
	uint8_t 		symbol_shift;
	char 			smbl;		
	int8_t 			c;

	if(current_id == id)
	{
		current_frame++;
	}
	else
	{
		current_id 		= id;
		current_size 	= strlen(string);
		current_frame 	= 0;
		symbol_shift	= 0;
		symbol			= 0;
	}
	
	if(current_size > 4)
	{	
		symbol_shift 	 = current_frame / 3;
		if((symbol_shift / 6) >= 1)
		{
			symbol++;
			current_frame = 0;
			symbol_shift = 0;
			if(symbol > (int8_t)(current_size-1))
			{	
				symbol = -4;
				current_frame = 0;
				symbol_shift = 0;
			}
		}		
		
		for(c = symbol; c < (symbol + 4); c++)
		{
			if(c < 0 || c >= current_size) smbl = ' ';
			else smbl = *(string+c);
			for(uint8_t i = 0; i < 8; i++)
			{
				screen->draw[i] = ((screen->draw[i] << 6) | *( font + ((smbl) - 32) * 8 + i));
			}
		}
		
		if(c < 0 || ((c) >= current_size)) smbl = ' ';
		else smbl = *(string+c);
		
		if(symbol_shift)
			for(uint8_t i = 0; i < 8; i++)
			{
				screen->draw[i] = ((screen->draw[i] << symbol_shift) | *( font + ((smbl) - 32) * 8 + i) >> (6 - symbol_shift));
			}
	}
	else
	{
		print_str(screen, string);
	}
}

void print_shaking_str(Screen* screen, char* string, uint8_t id, char *font)
{
	static uint8_t  current_id = 255;
	static uint8_t  current_size;
	static uint16_t current_frame;
	static int8_t 	symbol;
	static int8_t	wait_timer;
	static bool		dir_left;
	uint8_t 		symbol_shift;
	char 			smbl;		
	int8_t 			c;

	if(current_id == id)
	{
		if(!wait_timer)
		{
			dir_left? current_frame-- : current_frame++;
		}
		else
		{
			wait_timer --;
		}
	}
	else
	{
		current_id 		= id;
		current_size 	= strlen(string);
		dir_left		= false;
		wait_timer		= 20;
		current_frame 	= 1;
		symbol_shift	= 0;
		symbol			= 0;
	}
	
	if(current_size > 4)
	{	
		symbol_shift 	 = GET_SYMBOL_SHIFT(current_frame);
		symbol = GET_START_SYMBOL(current_frame);
		if( !wait_timer && current_frame >= (current_size-4)*SYMBOL_TICKS+1 )
		{
			current_frame--;
			dir_left = !dir_left;
			wait_timer = SHAKING_DELAY;
		}
		if( !wait_timer && current_frame <= 0)
		{
			current_frame++;
			dir_left = !dir_left;
			wait_timer = SHAKING_DELAY;
		}
		
		for(c = symbol; c < (symbol + 4); c++)
		{
			if(c < 0 || c >= current_size) smbl = ' ';
			else smbl = *(string+c);
			for(uint8_t i = 0; i < 8; i++)
			{
				screen->draw[i] = ((screen->draw[i] << 6) | *( font + ((smbl) - 32) * 8 + i));
			}
		}
		
		if(c < 0 || ((c) >= current_size)) smbl = ' ';
		else smbl = *(string+c);
		
		if(symbol_shift)
			for(uint8_t i = 0; i < 8; i++)
			{
				screen->draw[i] = ((screen->draw[i] << symbol_shift) | *( font + ((smbl) - 32) * 8 + i) >> (6 - symbol_shift));
			}
	}
	else
	{
		print_str(screen, string);
	}
}

void print_char(Screen* screen, char chr)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		screen->draw[i] = ((screen->draw[i] << 6) | ascii_5x7_basic_rows[((chr) - 32)][i]);
	}
}

//------------------------------------------------------------------------------------------------------------
//
//													FSM STATEMENTS
//
//------------------------------------------------------------------------------------------------------------

void print_hour_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	device_data_g.data_pointer = &device_data_g.time_mode;
	device_data_g.time_mode &= 0x03;
	
	draw_screen(&screen_draw);
}

void print_menu_time_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	device_data_g.data_pointer = &device_data_g.time_menu_mode;
	(*device_data_g.data_pointer) = 2;
	device_data_g.menu_mode = 0;
	device_data_g.time_mode = 0;
	print_shaking_str(&screen_draw, (char*)labels[device_data_g.menu_mode], device_data_g.menu_mode, (char*)&ascii_5x7_basic_rows[0][0]);
}

void print_menu_time_set_st(void)
{
	device_data_g.time_menu_mode &= 0x03;
	device_data_g.time_pointer = &RTC_data.second;// + device_data_g.time_menu_mode;
	
	switch(device_data_g.time_menu_mode)
	{
		case 2:
			print_str_number(&screen_draw, "H ",RTC_data.hour);
			//print_str(&screen_draw, "H ");
			//print_char(&screen_draw, 48+RTC_data.hour/10);
			//print_char(&screen_draw, 48+RTC_data.hour%10);
		break;
		case 1:
			print_str_number(&screen_draw, "M ",RTC_data.minute);	
			//print_str(&screen_draw, "M ");
			//print_char(&screen_draw, 48+RTC_data.minute/10);
			//print_char(&screen_draw, 48+RTC_data.minute%10);
		break;
		case 0:
			print_str_number(&screen_draw, "S ",RTC_data.second);
			//print_str(&screen_draw, "S ");
			//print_char(&screen_draw, 48+RTC_data.second/10);
			//print_char(&screen_draw, 48+RTC_data.second%10);
		break;
		case 3:
			print_str(&screen_draw, "F ");
			print_str(&screen_draw, RTC_data.time_mode? "12" : "24");
		break;
	}
}

void print_menu_date_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	device_data_g.data_pointer = &device_data_g.date_menu_mode;
	(*device_data_g.data_pointer) = 3;
	device_data_g.menu_mode = 1;
	print_shaking_str(&screen_draw, (char*)labels[device_data_g.menu_mode], device_data_g.menu_mode, (char*)&ascii_5x7_basic_rows[0][0]);
}

void print_menu_date_set_st(void)
{
//date_menu_mode	
	device_data_g.date_menu_mode &= 0x03;
	device_data_g.time_pointer = &RTC_data.w_day;// + device_data_g.time_menu_mode;
	
	switch(device_data_g.date_menu_mode)
	{
		case 3:
			print_str_number(&screen_draw, "Y ",RTC_data.year);
			//print_str(&screen_draw, "Y ");
			//print_char(&screen_draw, 48+RTC_data.year/10);
			//print_char(&screen_draw, 48+RTC_data.year%10);
		break;
		case 2:
			print_str_number(&screen_draw, "M ",RTC_data.month);
			//print_str(&screen_draw, "M ");
			//print_char(&screen_draw, 48+RTC_data.month/10);
			//print_char(&screen_draw, 48+RTC_data.month%10);
		break;
		case 1:
			print_str_number(&screen_draw, "D ",RTC_data.day);
			//print_str(&screen_draw, "D ");
			//print_char(&screen_draw, 48+RTC_data.day/10);
			//print_char(&screen_draw, 48+RTC_data.day%10);
		break;
		case 0:
			print_str(&screen_draw, "W  ");
			print_char(&screen_draw, 48+RTC_data.w_day%10);
		break;
	}
}	

void print_menu_al_1_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	device_data_g.menu_mode = 2;
	device_data_g.data_pointer = &device_data_g.time_menu_mode;
	(*device_data_g.data_pointer) = 2;
	print_shaking_str(&screen_draw, (char*)labels[device_data_g.menu_mode], device_data_g.menu_mode, (char*)&ascii_5x7_basic_rows[0][0]);
}

void print_menu_al_1_set_st(void)
{
	if(device_data_g.time_menu_mode == 0)
	{
		device_data_g.time_menu_mode = 4;
	}
	else if(device_data_g.time_menu_mode > 4)
	{
		device_data_g.time_menu_mode = 1;
	}
	//device_data_g.time_menu_mode &= 0x03;
	device_data_g.time_pointer = &RTC_data.al1_second;// + device_data_g.time_menu_mode;
	
	if(device_data_g.time_menu_mode == 4)
	{
		gus1 = &inc_interface_st;
		gus2 = &dec_interface_st;
	}
	else
	{
		gus1 = &inc_time;
		gus2 = &dec_time;
	}
	
	switch(device_data_g.time_menu_mode)
	{
		case 2:
			print_str_number(&screen_draw, "H ",RTC_data.al1_hour);
			//print_str(&screen_draw, "H ");
			//print_char(&screen_draw, 48+RTC_data.al1_hour/10);
			//print_char(&screen_draw, 48+RTC_data.al1_hour%10);
		break;
		case 1:
			print_str_number(&screen_draw, "M ",RTC_data.al1_minute);
			//print_str(&screen_draw, "M ");
			//print_char(&screen_draw, 48+RTC_data.al1_minute/10);
			//print_char(&screen_draw, 48+RTC_data.al1_minute%10);
		break;
		case 0:
			print_str_number(&screen_draw, "S ",RTC_data.al1_second);
			//print_str(&screen_draw, "S ");
			//print_char(&screen_draw, 48+RTC_data.al1_second/10);
			//print_char(&screen_draw, 48+RTC_data.al1_second%10);
		break;
		case 3:		
			//print_str(&screen_draw, "E ");
			print_str(&screen_draw, RTC_data.al1_active? " ON " : " OFF");
		break;
		case 4:
			device_data_g.interface_pointer = &device_data_g.sd_1st_song_num;
			print_shaking_str(&screen_draw, &device_data_g.sd_1st_song_name[0], *device_data_g.interface_pointer, (char*)&ascii_5x7_basic_rows[0][0]);
		break;
	}
}

void print_menu_al_2_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	device_data_g.data_pointer = &device_data_g.time_menu_mode;
	(*device_data_g.data_pointer) = 2;
	device_data_g.menu_mode = 3;
	print_shaking_str(&screen_draw, (char*)labels[device_data_g.menu_mode], device_data_g.menu_mode, (char*)&ascii_5x7_basic_rows[0][0]);
}

void print_menu_al_2_set_st(void)
{
	
	if(device_data_g.time_menu_mode == 0)
	{
		device_data_g.time_menu_mode = 4;
	}
	else if(device_data_g.time_menu_mode > 4)
	{
		device_data_g.time_menu_mode = 1;
	}
	//device_data_g.time_menu_mode &= 0x03;
	device_data_g.time_pointer = &RTC_data.al2_second;// + device_data_g.time_menu_mode;
	
	switch(device_data_g.time_menu_mode)
	{
		case 2:
			print_str_number(&screen_draw, "H ",RTC_data.al2_hour);
			//print_str(&screen_draw, "H ");
			//print_char(&screen_draw, 48+RTC_data.al2_hour/10);
			//print_char(&screen_draw, 48+RTC_data.al2_hour%10);
		break;
		case 1:
			print_str_number(&screen_draw, "M ",RTC_data.al2_minute);
			//print_str(&screen_draw, "M ");
			//print_char(&screen_draw, 48+RTC_data.al2_minute/10);
			//print_char(&screen_draw, 48+RTC_data.al2_minute%10);
		break;
		case 0:
			print_str_number(&screen_draw, "S ",RTC_data.al2_second);
			//print_str(&screen_draw, "S ");
			//print_char(&screen_draw, 48+RTC_data.al2_second/10);
			//print_char(&screen_draw, 48+RTC_data.al2_second%10);
		break;
		case 3:
			//print_str(&screen_draw, "E ");
			print_str(&screen_draw, RTC_data.al2_active? " ON " : " OFF");
		break;
		case 4:
			//print_str(&screen_draw, "E ");
			print_shaking_str(&screen_draw, "Polyphony 1", device_data_g.menu_mode+1, (char*)&ascii_5x7_basic_rows[0][0]);
		break;
	}
}

void print_menu_sensor_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	device_data_g.menu_mode = 4;
	device_data_g.data_pointer = &device_data_g.sensor_type;
	print_shaking_str(&screen_draw, (char*)labels[device_data_g.menu_mode], device_data_g.menu_mode, (char*)&ascii_5x7_basic_rows[0][0]);
}

void print_menu_sensor_set_st(void)
{
	OK_BUTTON_WORKED_OUT;
	*device_data_g.data_pointer &= 0x3;
	print_shaking_str(&screen_draw, (char*)sensor[device_data_g.sensor_type], device_data_g.menu_mode+device_data_g.sensor_type+1, (char*)&ascii_5x7_basic_rows[0][0]);
	return;
}

void print_menu_brts_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	device_data_g.menu_mode = 5;
	device_data_g.interface_pointer = &device_data_g.menu_neasted_page;
	*(device_data_g.interface_pointer) = 0;
	device_data_g.data_pointer = &device_data_g.day_bridhtness_hour;
	print_shaking_str(&screen_draw, (char*)labels[device_data_g.menu_mode], device_data_g.menu_mode, (char*)&ascii_5x7_basic_rows[0][0]);
}

void print_menu_brts_set_st(void)
{
	device_data_g.menu_neasted_page &= 0x3;
	switch(device_data_g.menu_neasted_page)
	{
		case 0:
			device_data_g.data_pointer = &device_data_g.day_bridhtness_hour;
			if(device_data_g.day_bridhtness_hour > 127)
			{
				device_data_g.day_bridhtness_hour = 0;
			}
			else if(device_data_g.day_bridhtness_hour > device_data_g.nigth_bridhtness_hour)
			{
				device_data_g.day_bridhtness_hour = device_data_g.nigth_bridhtness_hour;
			}
			
			print_str_number(&screen_draw, "D ",device_data_g.day_bridhtness_hour);
			//print_str(&screen_draw, "D ");
			//print_char(&screen_draw, 48+device_data_g.day_bridhtness_hour/10);
			//print_char(&screen_draw, 48+device_data_g.day_bridhtness_hour%10);
			
		break;
		case 1:
			device_data_g.data_pointer = &device_data_g.day_bridhtness;
			if(device_data_g.day_bridhtness > 127)
			{
				device_data_g.day_bridhtness = 0;
			}
			else if(device_data_g.day_bridhtness > 10)
			{
				device_data_g.day_bridhtness = 10;
			}
			
			print_str_number(&screen_draw, "DB",device_data_g.day_bridhtness);
			//print_str(&screen_draw, "DB");
			//print_char(&screen_draw, 48+device_data_g.day_bridhtness/10);
			//print_char(&screen_draw, 48+device_data_g.day_bridhtness%10);
			
		break;
		case 2:
			device_data_g.data_pointer = &device_data_g.nigth_bridhtness_hour;
		
			if(device_data_g.nigth_bridhtness_hour < device_data_g.day_bridhtness_hour || device_data_g.nigth_bridhtness_hour > 127)
			{
				device_data_g.nigth_bridhtness_hour = device_data_g.day_bridhtness_hour;
			}
			else if(device_data_g.nigth_bridhtness_hour > 23)
			{
				device_data_g.nigth_bridhtness_hour = 23;
			}
			
			print_str_number(&screen_draw, "N ",device_data_g.nigth_bridhtness_hour);
			//print_str(&screen_draw, "N ");
			//print_char(&screen_draw, 48+device_data_g.nigth_bridhtness_hour/10);
			//print_char(&screen_draw, 48+device_data_g.nigth_bridhtness_hour%10);
			
		break;
		case 3:
			device_data_g.data_pointer = &device_data_g.nigth_bridhtness;
		
			if(device_data_g.nigth_bridhtness > 127)
			{
				device_data_g.nigth_bridhtness = 0;
			}
			else if(device_data_g.nigth_bridhtness > 10)
			{
				device_data_g.nigth_bridhtness = 10;
			}
			
			print_str_number(&screen_draw, "NB",device_data_g.nigth_bridhtness);
			//print_str(&screen_draw, "NB");
			//print_char(&screen_draw, 48+device_data_g.nigth_bridhtness/10);
			//print_char(&screen_draw, 48+device_data_g.nigth_bridhtness%10);
		break;
	}
	return;
}

void print_menu_template_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	device_data_g.menu_mode = 6;
	device_data_g.data_pointer = &device_data_g.template_number;
	print_shaking_str(&screen_draw, (char*)labels[device_data_g.menu_mode], device_data_g.menu_mode, (char*)&ascii_5x7_basic_rows[0][0]);
}

void print_menu_template_set_st(void)
{
	OK_BUTTON_WORKED_OUT;
	*device_data_g.data_pointer &= 0x7;
	print_str(&screen_draw, "Tp ");
	print_char(&screen_draw, 48+device_data_g.template_number%10);
	return;
}

void print_menu_font_st(void)
{
	OK_BUTTON_WORKED_OUT;
	UP_BUTTON_WORKED_OUT;
	DOWN_BUTTON_WORKED_OUT;
	
	device_data_g.menu_mode = 7;
	device_data_g.data_pointer = &device_data_g.font_number;
	device_data_g.interface_pointer = &device_data_g.menu_neasted_page;
	*device_data_g.interface_pointer = 0;
	print_shaking_str(&screen_draw, (char*)labels[device_data_g.menu_mode], device_data_g.menu_mode, (char*)&ascii_5x7_basic_rows[0][0]);
}

void print_menu_font_set_st(void)
{
	OK_BUTTON_WORKED_OUT;
	*device_data_g.data_pointer &= 0x3;
	*device_data_g.interface_pointer &= 0x1;
	
	switch(*device_data_g.interface_pointer)
	{
		case 0:
			print_str(&screen_draw, "F  ");
			print_char(&screen_draw, 48+device_data_g.font_number%10);
		break;
		case 1:
			print_shaking_str(&screen_draw, (char*)&test_line, device_data_g.menu_mode+1, (char*)digits[device_data_g.font_number]);
		break;
	}
	return;
}

void fail_st(void)
{
	return;
}
