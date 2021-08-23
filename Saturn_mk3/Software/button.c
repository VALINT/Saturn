/*-----------------------------------------------------------------------------------------------------------------------
 * 	Buttons module
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


#include "button.h"

void check_button(Button *button)
{
	// Check buttons
	if(!(*(button->pin) & (1 << button->number)))	//If button is pressed
	{
		if(button->hold_timer < BUTTON_HOLD_COND)
		{
			button->hold_timer++;
			if(button->hold_timer >= BUTTON_DEBOUNCE) button->state = BT_PUSHED;
		}
		else if(button->hold_timer == BUTTON_HOLD_COND && button->state != BT_WORKED_OUT)
		{
			button->state = BT_HOLD;
			if(button->callback) {button->callback(button->hold_event);}
		}
		else if(button->state == BT_HOLD)
		{
			if(button->callback) {button->callback(button->hold_event);}
		}
	}
	else	//If button is pressed
	{
		if(button->hold_timer >= BUTTON_DEBOUNCE && button->hold_timer < BUTTON_HOLD_COND) 
		{
			button->state = BT_CLICK;
			if(button->callback) {button->callback(button->click_event);}
		}
		else if(button->state != BT_CLICK) button->state = BT_FREE;
		button->hold_timer = 0;
	}
}

bt_state_t get_bt_state(Button *button)
{
	return button->state;
}

void set_bt_state(Button *button, bt_state_t state)
{
	button->state = state;
	return;
}
