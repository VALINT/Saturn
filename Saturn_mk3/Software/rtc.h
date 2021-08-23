#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"
#include "stdint.h"

void _init_RTC(uint32_t start_time);
void _write_time(uint32_t time);

#endif
