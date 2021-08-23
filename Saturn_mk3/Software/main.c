#include "stm32f10x.h"
#include "date_time.h"
#include "automata.h"
#include "mytypes.h"
#include "button.h"
#include "delay.h"
#include "event.h"
#include "draw.h"
#include "pff.h"
#include "sd.h"
#include "diskio.h"

bool volatile update_sec = false;
date_time RTC_data = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,1,1,1};
automata_state_t main_automata = {ST_SHOW_HOURS,ST_SHOW_HOURS};
Screen screen_draw;

Button button_up = 		{BT_FREE, &(GPIOA->IDR), 0, 0, E_UP_BUTTON_CLICK, 	E_UP_BUTTON_HOLD, 	&add_event};
Button button_ok = 		{BT_FREE, &(GPIOA->IDR), 1, 0, E_OK_BUTTON_CLICK, 	E_OK_BUTTON_HOLD, 	&add_event};
Button button_down = 	{BT_FREE, &(GPIOA->IDR), 2, 0, E_DOWN_BUTTON_CLICK,	E_DOWN_BUTTON_HOLD, &add_event};
						
global_data_t device_data_g = {0,0,0,0,0,0};

void unblocked_delay(void);

void EXTI4_IRQHandler(void)
{
	EXTI->PR |= EXTI_PR_PR4;
	update_sec = true;
}

int main(void)
{
	// Start MCU initialization
	// Initialize Systick
	initSysTick();
	delay_set_callback(&unblocked_delay);
	// Initialize RTC
	//start_clock(convert_to_Unix_time(&RTC_data));
	delay_ms(500);
	// Initialize display (3x8x8 matrices)
	_i2c_init();
	delay_ms(100);
	DS3231_init();
	delay_ms(10);
	init_display();
	DS3231_read_time((void*)&RTC_data);
	DS3231_read_date((void*)&RTC_data);
	DS3231_read_alarm((void*)&RTC_data);
	
	//Set-up B4 pin to Input, Pull-Up, Interrupt;
	CLEAR_REG_PART(GPIOB, CRL, (0xf << 16));			//Clear CRL for PIN4
	SET_REG_PART(GPIOB, CRL, (INPUT_PULL_UP << 16));	//Set PIN4 to Input, pp mode
	SET_PIN(GPIOB,4);									//Pull-up PIN4
	
	//Enable interrupt for PINB4 for front and back edges
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->EXTICR[1] |= (AFIO_EXTICR2_EXTI4_PB);
	EXTI->PR &=~ EXTI_PR_PR4;
	EXTI->IMR |= EXTI_IMR_MR4;
	EXTI->RTSR |= EXTI_RTSR_TR4;
	EXTI->FTSR |= EXTI_FTSR_TR4;
	//Enable EXTI4 interrupt
	NVIC->ISER[0] |= 0x00000400;
	
	// Init GPIOB (SPI2NSS, SPI2SCK, SPI2MOSI, SPI2MISO) to work with SD card
	// Enable GPIOB clocking
	ENABLE_PORT(B);
	// Clear CRH value (because reset value is 0x4444)
	CLEAR_REG_PART(GPIOB, CRH, ((0xf << SPI2NSS_SHIFT_N)|
								(0xf << SPI2SCK_SHIFT_N)|
								(0xf << SPI2MOSI_SHIFT_N)|
								(0xf << SPI2MISO_SHIFT_N)));
	// Set pins in appropriate modes							
	SET_REG_PART(GPIOB, CRH, ((OUTPUT_PUSH_PULL_50M << SPI2NSS_SHIFT_N)|
							(ALTERN_PUSH_PULL_50M << SPI2SCK_SHIFT_N)|
							(ALTERN_PUSH_PULL_50M << SPI2MOSI_SHIFT_N)|
							(INPUT_FLOATING << SPI2MISO_SHIFT_N)));
	//Pull-up MISO pin
	SET_PIN(GPIOB, SPI2MISO);
	//Set high level on NSS pin (disable device)
	SET_PIN(GPIOB, SPI2NSS);
	//Init spi(can be removed because it's duplicated in SD card initialization procedure)
	_spi_init();
	device_data_g.sd_inserted = true;
	//Try to initialize SD card.
	/*for(uint8_t i = 0; i < 5; i++)
	{
		device_data_g.sd_mounted = pf_mount(&fs);
		if(device_data_g.sd_mounted == FR_OK)
		{
			break;
		}
		delay_ms(1000);
	}*/
	
    while(1)
    {
		//Now 2 interrupts per second to get 1Hz of dots blinking
		if(update_sec)
		{
			update_sec = false;
			device_data_g.dot = !device_data_g.dot;
			if(device_data_g.dot)	// 1 time per second
			{
				//Read actual time
				DS3231_read_time((void*)&RTC_data);
				//Decrement menu out timer
				if(device_data_g.menu_counter)
				{
					device_data_g.menu_counter--;
				}
				else	//Return to the main screen
				{
					//Save settings
					
					//Go to start state
					add_event(E_GO_START);
				}
			}
		}
		
		// Try to init SD card
		if(device_data_g.sd_inserted)
		{
			if(!device_data_g.sd_mounted && !device_data_g.sd_error)
			{
				sd_init(&device_data_g);
				sd_read(&device_data_g);
			}
			
			if(device_data_g.sd_ready && (device_data_g.sd_1st_last_song_num != device_data_g.sd_1st_song_num))
			{
				if(device_data_g.sd_1st_song_num > 127) device_data_g.sd_1st_song_num = device_data_g.sd_songs_amount-1;
				else if(device_data_g.sd_1st_song_num > device_data_g.sd_songs_amount-1) device_data_g.sd_1st_song_num = 0;
				sd_read(&device_data_g);
				device_data_g.sd_1st_last_song_num = device_data_g.sd_1st_song_num;
			}
		}
		else
		{
			device_data_g.sd_mounted = false;
			device_data_g.sd_error = false;
			device_data_g.sd_ready = false;
			device_data_g.sd_songs_amount = 0;
		}
		
		if(main_automata.curr_state < ST_MAX)
		{
			check_button(&button_up);
			check_button(&button_ok);
			check_button(&button_down);
		}
		
		if(get_event_len())
		{
			uint8_t test = get_event();
			device_data_g.menu_counter = 30;
			if(test > E_MAX)
			{
				switch(test)
				{
					case E_GO_BACK :main_automata.curr_state = main_automata.prev_state; break;
					case E_GO_START:main_automata.curr_state = ST_SHOW_HOURS; break;
				}
			}
			else
			{
				main_automata.prev_state = main_automata.curr_state;
				main_automata.curr_state = transition_table[main_automata.curr_state][test];
			}
		}
		state[main_automata.curr_state]();
		
		// Draw screen
		draw(&screen_draw);
		
		//clear_list();
		delay_ms(20);
    }
}

void unblocked_delay(void)
{
	if(device_data_g.sd_timer) device_data_g.sd_timer--; 
}
