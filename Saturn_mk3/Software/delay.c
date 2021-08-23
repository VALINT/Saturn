#include "delay.h"

static void(*callback)(void) = 0;
static volatile uint32_t delay = 0;

void SysTick_Handler(void)      //SysTick IRQ Handler
{
    delay -= (uint32_t)(delay > 0);
	if(callback != 0) callback();
}

void delay_set_callback(void(*clb)(void))
{
	callback = clb;
}

void initSysTick (void)
{
	// Initialization of SysTick timer
    SysTick->LOAD = SYSCLOCK/1000;
    SysTick->VAL  = SYSCLOCK/1000;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(uint32_t ms)      
{
    delay = ms;
    while(delay);
}
