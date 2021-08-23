#include "MAX7219.h"

#define ENABLE_PORT(num) RCC->APB2ENR |= RCC_APB2ENR_IOP##num##EN

void _init_SPI (void)
{
    #if defined (USE_HARD_SPI)
        #if defined (USE_SPI1)
            //RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable PORTA clocking
            ENABLE_PORT(A);
            // Set 5th (SPI1_SCK) and 7th (SPI1 MOSI) to AF_PP mode, speed 10MHz
            // Set GPIOA4 (SPI_CS) to GP_PP mode, speed 10MHz
            GPIOA->CRL   |= GPIO_CRL_MODE5_0 | GPIO_CRL_CNF5_1 |  
                            GPIO_CRL_MODE7_0 | GPIO_CRL_CNF7_1 | 
                            GPIO_CRL_MODE4_0;
            GPIOA->CRL   &=~GPIO_CRL_CNF7_0; 
            GPIOA->CRL   &=~GPIO_CRL_CNF5_0;
            GPIOA->CRL   &=~GPIO_CRL_CNF4_0;

            //SPI_CS Set high
            GPIOA->BSRR = GPIO_BSRR_BS4;
            
            //Enable SPI1 Clocking
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 
            // Set-up SPI1 to Master, without MISO, 16bit, soft NSS.
            SPI1->CR1  = SPI_CR1_BIDIMODE | 
                        SPI_CR1_BIDIOE | 
                        SPI_CR1_DFF | 
                        SPI_CR1_BR_2 | 
                        SPI_CR1_BR_1 | 
                        SPI_CR1_SSM | 
                        SPI_CR1_SSI | 
                        SPI_CR1_MSTR;
            SPI1->CR1 |= SPI_CR1_SPE;
            
        #elif defined (USE_SPI2)
            #error "Sasamba, This function not realized. Use SPI1."
        #endif
    #elif defined (USE_SOFT_SPI)
        #error "I'm too lazy to make this functionality"
    #endif
}

void _SPI_sent_byte(uint16_t byte)
{
    SPI1->DR = byte;
    while(!SPI1->SR & SPI_SR_TXE);
    while(SPI1->SR & SPI_SR_BSY);
}

void sent_data (uint16_t word)
{
    CS_LOW;
    _SPI_sent_byte(word);
    CS_HIGH;
}

//void send_string (uint8_t addres, uint8_t* data, uint8_t amonth);*/

void init_display(void)
{
    _init_SPI();
    //Init. MAX7219 as 8x8 matrices x 3 display
    //Decode mode - no decode
	sent_data(0x0900);
	
    //Intensity - 3/32
	sent_data(0x0A01);
	
    //Scan limit - 7
	sent_data(0x0B07);
	
    //Turn-on display
	sent_data(0x0C01);

    //Normal operation mode
	sent_data(0x0F00);

    for(uint16_t i = 0; i < 8*MATRIXES; i++)
        sent_data( (((i%8)+1) << 8) | 0);

}
