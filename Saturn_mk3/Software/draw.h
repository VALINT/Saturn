#ifndef _DRAW_H_
#define _DRAW_H_

#include "mytypes.h"
#include "MAX7219.h"

void _SPI_send_byte(uint16_t);
void _SPI_send_byte_in(uint16_t);

void draw(Screen *screen)
{
	for(uint16_t i = 1; i < 9; i++)
	{
		//SPI_CS Set low
		GPIOA->BSRR = GPIO_BSRR_BR4;    
		_SPI_send_byte_in((i<<8)| screen->out[i-1][2]);
		_SPI_send_byte_in((i<<8)| screen->out[i-1][1]);
		_SPI_send_byte_in((i<<8)| screen->out[i-1][0]);
		//SPI_CS Set high
		GPIOA->BSRR = GPIO_BSRR_BS4;
	}
	
}

void _SPI_send_byte_in(uint16_t data)
{
    SPI1->DR = data;
    while(!SPI1->SR & SPI_SR_TXE);
    while(SPI1->SR & SPI_SR_BSY);
}

#endif
