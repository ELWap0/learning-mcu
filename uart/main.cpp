/*
 *will be sending hello through virtual comm to computer every second
 */

#include <stdint.h>

uint32_t freq = 4e6;

void setup(){
  //enable gate
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_Msk;
  RCC->CCIPR |= (0b01 << RCC_CCIPR_USART1SEL_Pos);
  //configure pin to use UART functionality
  GPIOA->MODER &= ~(GPIO_MODER_MODE9_Msk|GPIO_MODER_MODE10_Msk);
  GPIOA->MODER |= ((0b10U << GPIO_MODER_MODE9_Pos) | ( 0b10U << GPIO_MODER_MODE10_Pos));
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR9_Msk | GPIO_OSPEEDR10_Msk);
  GPIOA->OSPEEDR |= ((0b01U << GPIO_OSPEEDR9_Pos) | (0b01U << GPIO_OSPEEDR10_Pos));
  GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL9_Msk |GPIO_AFRH_AFSEL10_Msk);
  GPIOA->AFR[1] |= (0x7 << GPIO_AFRH_AFSEL9_Pos | 0x7 << GPIO_AFRH_AFSEL10_Pos);

  USART1->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);
  USART1->CR2 &= ~(USART_CR2_STOP_Msk);
  USART1->BRR = freq/9600;
  USART1->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
}

void delayMs(uint32_t ms){
  int32_t tick = freq/1000;
  for(uint32_t x = 0; x < ms; x++){
    for(uint32_t tickCounter = 0; tickCounter < tick; tickCounter++);
  }
}


int main(){
  setup();
  char test[] = "hello";
  while(true){
    for(int i = 0; i < sizeof(test)-1; i++){
      while((USART1->ISR & USART_ISR_TXE) != USART_ISR_TXE);
      USART1->TDR = test[i];
    }
    while((USART1->ISR & USART_ISR_TC) != USART_ISR_TC);
    delayMs(1000);
  }
}
