#include <stdint.h>
#include <stdbool.h>

bool state = false;
void setup();


int main(){
  setup();
  while(true);  
}


void setup(){
  
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

  GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
  GPIOA->MODER |= (0b01 << GPIO_MODER_MODE0_Pos);


  TIM1->ARR = (SystemCoreClock / 1000) - 1;
  TIM1->DIER |= TIM1_DIER_UIE;
  TIM1->CR1 |= TIM1_CR1_CEN;
  NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}
void TIM1_UP_TIM16_IRQHandler(){
  if((TIM1->SR & TIM1_SR_UIF) == TIM1_SR_UIF){
    TIM1->SR = 0;
    state ^= true;
  }
  GPIOA->BSRR = (state)?GPIO_BSRR_BS0:GPIO_BSRR_BR0;
}
