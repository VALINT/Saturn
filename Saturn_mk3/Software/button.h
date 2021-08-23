/*-----------------------------------------------------------------------------------------------------------------------
 * Buttons module
 *_______________________________________________________________________________________________________________________
 *  __    __ __ __     __   _____	  ____  _   _  _____  _____  ____  __  _  _____
 *  \ \  / //  \\ \    \_\ / ___/	 | __ \| | | ||_   _||_   _|/    \|  \| |/ ___/
 *   \ \/ //    \\ \___ |/ \___ \	 | __ || |_| |  | |    | |  | || || |\  |\___ \
 *    \__//__/\__\\____\   /____/	 |____/ \___/   |_|    |_|  \____/|_| \_|/____/
 *				        _     _  ____  ____    _    ____  _   _
 *				       | |   | || __ \| |\ \  / \  | |\ \\ \_/ /
 *				       | |__ | || __ || |/ / / _ \ | |/ / \   /
 *			           |____||_||____/|_|\_\/_/ \_\|_|\_\  |_|
 *_______________________________________________________________________________________________________________________
 * Created: 30.04.2020 23:14:08
 *  Author: VAL
 *-----------------------------------------------------------------------------------------------------------------------
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "stm32f10x.h"
#include <stdint.h>

#define BUTTON_DEBOUNCE		2
#define BUTTON_HOLD_COND  	100

typedef enum {BT_FREE, BT_PUSHED, BT_CLICK, BT_HOLD, BT_WORKED_OUT} bt_state_t;	// Button states list

typedef struct																	// Button structure
{
	bt_state_t	 		state;
	volatile uint32_t  	*pin;
	uint8_t  			number; 
	uint16_t 			hold_timer;
	uint8_t				click_event;
	uint8_t				hold_event;
	void (*callback)(uint8_t);
} Button;

void check_button(Button *button);
bt_state_t get_bt_state(Button *button);
void set_bt_state(Button *button, bt_state_t state);

#endif /* BUTTONS_H_ */
