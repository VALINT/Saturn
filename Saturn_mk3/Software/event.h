#ifndef _EVENT_H_
#define _EVENT_H_

#include "stdint.h"

#define EVENT_LIST_MAX 10

typedef struct{
	uint8_t event_list[EVENT_LIST_MAX];
	uint8_t event_count;
}event_t;

//extern event_t event_s;

void add_event(uint8_t event_id);
uint8_t get_event_len(void);
uint8_t get_event(void);
void clear_list(void);

#endif
