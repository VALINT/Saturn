#ifndef _AUTOMATA_H_
#define _AUTOMATA_H_

#include "date_time.h"
#include "mytypes.h"
#include "button.h"
#include "event.h"
#include "DS3231.h"
#include "string.h"

#define MOOVING_STR_SPEED	3
#define SHAKING_DELAY		30		//in frames

#define MOOVING_STR_SYMBOL	6
#define SYMBOL_TICKS	MOOVING_STR_SPEED*MOOVING_STR_SYMBOL
#define GET_SYMBOL_SHIFT(x)	(x%(SYMBOL_TICKS))/	MOOVING_STR_SPEED
#define GET_START_SYMBOL(x)	x/(SYMBOL_TICKS)

void inc_indirectly_st(void);
void dec_indirectly_st(void);
void inc_interface_st(void);
void dec_interface_st(void);
void inc_time(void);
void dec_time(void);
void inc_date(void);
void dec_date(void);
void print_hour_st(void);
void print_menu_time_st(void);
void print_menu_time_set_st(void);
void print_menu_date_st(void);
void print_menu_date_set_st(void);
void print_menu_al_1_st(void);
void print_menu_al_1_set_st(void);
void print_menu_al_2_st(void);
void print_menu_al_2_set_st(void);
void print_menu_sensor_st(void);
void print_menu_sensor_set_st(void);
void print_menu_brts_st(void);
void print_menu_brts_set_st(void);
void print_menu_template_st(void);
void print_menu_template_set_st(void);
void print_menu_font_st(void);
void print_menu_font_set_st(void);
void fail_st(void);

typedef void (* state_ptr)(void);

typedef enum {	ST_SHOW_HOURS,
				ST_MENU_TIME,
				ST_MENU_SET_TIME,
				ST_MENU_DATE,
				ST_MENU_SET_DATE,	
				ST_MENU_ALARM1,
				ST_MENU_SET_AL1,
				ST_MENU_ALARM2,
				ST_MENU_SET_AL2,
				ST_MENU_SENSOR,
				ST_MENU_SET_SENSOR,
				ST_MENU_BRIGHTNESS,
				ST_MENU_SET_BRIGHTNESS,
				ST_MENU_TEMPLATE,
				ST_MENU_SET_TEMPLATE,
				ST_MENU_FONT,
				ST_MENU_SET_FONT,
				ST_FAIL,
				ST_MAX,
				ST_INC_INDIRECTLY,
				ST_DEC_INDIRECTLY,
				ST_INC_TIME,
				ST_DEC_TIME,
				ST_INC_DATE,
				ST_DEC_DATE,
				ST_INC_INTERFACE,
				ST_DEC_INTERFACE,
				ST_GUSKET_1,
				ST_GUSKET_2} state_codes_t;
								
typedef enum {	E_NONE,
				E_OK_BUTTON_CLICK, 
				E_OK_BUTTON_HOLD, 
				E_UP_BUTTON_CLICK, 
				E_UP_BUTTON_HOLD, 
				E_DOWN_BUTTON_CLICK, 
				E_DOWN_BUTTON_HOLD, 
				E_INC_IND,
				E_DEC_IND,
				E_NEXT_ST,
				E_MAX,
				E_GO_BACK,
				E_GO_START} event_codes_t;

extern const state_codes_t transition_table[ST_MAX][E_MAX];
				
typedef struct{ 
	state_codes_t curr_state;
	state_codes_t prev_state;
} automata_state_t;
				

extern bool volatile update_sec; 
extern const state_ptr state[];
extern date_time RTC_data;
extern Screen screen_draw;
extern global_data_t device_data_g;

extern Button button_up;
extern Button button_ok;
extern Button button_down;

#define ENTRY ST_SHOW_HOURS
#define EXIT  ST_FAIL

#endif
