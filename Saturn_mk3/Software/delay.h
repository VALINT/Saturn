#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f10x.h"
#include "stdint.h"

#ifndef SYSCLOCK
	#define SYSCLOCK 72000000ULL
#endif

void SysTick_Handler(void);      //SysTick IRQ Handler

void initSysTick (void);

void delay_set_callback(void(*clb)(void));

void delay_ms(uint32_t ms);

#endif
