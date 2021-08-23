#include "event.h"

static event_t event_s;

void add_event(uint8_t event_id)
{
	if(event_s.event_count < EVENT_LIST_MAX)
	{
		event_s.event_list[event_s.event_count++] = event_id;
	}
}

uint8_t get_event_len(void)
{
	return event_s.event_count;
}

uint8_t get_event(void)
{
	if(event_s.event_count)
		return event_s.event_list[--event_s.event_count];
	return 0;
}

void clear_list(void)
{
	event_s.event_count = 0;
}
