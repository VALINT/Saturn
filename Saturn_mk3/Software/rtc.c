#include "rtc.h"

void _init_RTC(uint32_t start_time)
{	
	//If RTC not enabled
	if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN)
    {
		// Enable PWR and Backup clocking
		RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
		// Allow access to Backup registers
		PWR->CR |= PWR_CR_DBP;
		// Clear Backup register
		RCC->BDCR |= RCC_BDCR_BDRST;
		RCC->BDCR &= ~RCC_BDCR_BDRST;
		// Enable RTC, choose low speed external oscillator
		RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE;
		// Activate low speed external oscillator
		RCC->BDCR |= RCC_BDCR_LSEON;
		// Wail till LSE be ready
		while (!(RCC->BDCR & RCC_BDCR_LSERDY)){}
		// Wait till all RTC registers written
		while (!(RTC->CRL & RTC_CRL_RTOFF));
		// Allow write to the RTC registers
		RTC->CRL  |=  RTC_CRL_CNF;
		// Set PRL reg to 32767 (+ 1) for using with 32768Hz crystal
		RTC->PRLL  = 0x7FFF;
		// Set minimat date and time	
		RTC->CNTL = (uint16_t)start_time;
		RTC->CNTH = (uint16_t)(start_time >> 16);	
		// Disallow write to the RTC registers
		RTC->CRL  &=  ~RTC_CRL_CNF;
		// Wait till all RTC registers written
		while (!(RTC->CRL & RTC_CRL_RTOFF));
		// Clear RSF flag
		RTC->CRL &= (uint16_t)~RTC_CRL_RSF;
		// Wait till RTC registers be synchronized
		while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF){}
		//Disallow access to Backup registers
		PWR->CR &= ~PWR_CR_DBP;
    }
	
	// Enable RTC interrupt in NVIC
	NVIC->ISER[0] |= 0x00000008;
	// Enable RTC second interrupt
	RTC->CRH |= RTC_CRH_SECIE;
}

void _write_time(uint32_t time)
{
	
	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
	PWR->CR |= PWR_CR_DBP;
	while (!(RTC->CRL & RTC_CRL_RTOFF));
	RTC->CRL |= RTC_CRL_CNF;
	RTC->CNTH = time>>16;
	RTC->CNTL = time;
	RTC->CRL &= ~RTC_CRL_CNF;
	while (!(RTC->CRL & RTC_CRL_RTOFF));
	PWR->CR &= ~PWR_CR_DBP;  
}
