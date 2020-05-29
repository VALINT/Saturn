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


#include "buttons.h"

void checkButton(Button *button)
{
	// Check buttons
	if(!(*(button->pin) & (1 << button->number)))
	{
		if(button->hold_timer < ButtonHoldCond)
		{
			button->hold_timer++;
			if(button->hold_timer >= ButtonDebounce) button->state = pushedSt;
		}
		else if(button->hold_timer == ButtonHoldCond && button->state != wasteSt) button->state = holdSt;
	}
	else
	{
		if(button->hold_timer >= ButtonDebounce && button->hold_timer < ButtonHoldCond) button->state = clickSt;
		else if(button->state != clickSt) button->state = freeSt;
		button->hold_timer = 0;
	}
}
