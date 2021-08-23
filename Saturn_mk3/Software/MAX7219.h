#ifndef _MAX7219_H_
#define _MAX7219_H_

#include "stm32f10x.h"
#include "stdint.h"

#define STM32F103

#define CS_GPIO     GPIOA
#define CS_PIN      4

#define CLK_GPIO    GPIOA
#define CLK_PIN     5 

#define MOSI_GPIO   GPIOA
#define MOSI_PIN    7

#define CS_HIGH     CS_GPIO->BSRR   = (0x00000001 << CS_PIN)
#define CS_LOW      CS_GPIO->BSRR   = (0x00010000 << CS_PIN)

#define CLK_HIGH    CLK_GPIO->BSRR  = (0x00000001 << CLK_PIN)
#define CLK_LOW     CLK_GPIO->BSRR  = (0x00010000 << CLK_PIN)

#define MOSI_HIGH   MOSI_GPIO->BSRR = (0x00000001 << MOSI_PIN)
#define MOSI_LOW    MOSI_GPIO->BSRR = (0x00010000 << MOSI_PIN)

#define MATRIXES    3

#define USE_HARD_SPI
//#define USE_PROGRAM_SPI
#define USE_SPI1
//#define USE_SPI2

void _init_SPI (void);
void _SPI_sent_byte(uint16_t byte);
void init_display(void);
void sent_data (uint16_t word);
void sent_string (uint8_t addres, uint8_t* data, uint8_t amount);

#endif
