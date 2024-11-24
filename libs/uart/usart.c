#include "usart.h"
#include "stm32f10x.h"

void usart_init(const uint32_t baud_rate)
{
  // RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
  // GPIOA->CRH &= ~GPIO_CRH_CNF9;
  // GPIOA->CRH |= GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;

  // RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  // USART1->BRR = SystemCoreClock / baud_rate;
  // USART1->CR1 = 0;
  // USART1->CR1 |= USART_CR1_UE | USART_CR1_TE;
  // Enable clocks for GPIOB and alternate function (AFIO)
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
  
  // // Enable remapping of USART1 to PB6 (TX) and PB7 (RX)
  AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;

  // Configure PB6 (TX) as Alternate function push-pull, 50 MHz
  GPIOB->CRL &= ~GPIO_CRL_CNF6; // Clear CNF bits for PB6
  GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_CNF6_1; // Set MODE6 (50 MHz) and CNF6_1 (AF push-pull)

  // Configure PB7 (RX) as Input floating
  GPIOB->CRL &= ~GPIO_CRL_CNF7; // Clear CNF bits for PB7
  GPIOB->CRL |= GPIO_CRL_CNF7_0; // Set CNF7_0 (input floating)
  
  // Enable USART1 clock
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // Set baud rate
  USART1->BRR = SystemCoreClock / baud_rate;

  // Enable USART1 and transmitter
  USART1->CR1 = 0;
  USART1->CR1 |= USART_CR1_UE | USART_CR1_TE;
}

void usart_tx(const char *msg)
{
  while(*msg)
  {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = *msg;
    msg++;
  }
}

void usart_txln(const char *msg)
{
  while(*msg)
  {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = *msg;
    msg++;
  }

  const char nlcr[] = "\r\n";
  for(uint8_t i = 0; i < 2; i++)
  {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = nlcr[i];
  }
}
