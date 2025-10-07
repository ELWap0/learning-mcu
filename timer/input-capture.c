#include <stdint.h>
#include <stdbool.h>

volatile uint32_t  capture;
void setup();

int main(){
  setup();
  while(true){
    __WFI();
  }
}

void setup(){
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  

  //pa8 -> TIM1_CH1
  GPIOA->MODER &= ~GPIO_MODER_MODE8_Msk;
  GPIOA->MODER |= 0b10 << GPIO_MODER_MODE8_Pos;
  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL8_Msk;
  GPIOA->AFR[1] |= 0x1 << GPIO_AFRH_AFSEL8_Pos;


  TIM1->CR1 &= ~TIM_CR1_DIR;
  //TIM1->ARR = (SystemCoreClock / 1000)-1;//1ms tick
  TIM1->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk | TIM_CCMR1_IC1F_Msk);
  TIM1->CCMR1 |= 0b01 << TIM_CCMR1_CC1S_Pos;
  TIM1->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
  TIM1->CCER |=   TIM_CCER_CC1E;
  TIM1->CR1 |= TIM_CR1_CEN; 
  TIM1->DIER |= TIM_DIER_CC1IE;
  NVIC_EnableIRQ(TIM1_CC_IRQn);
}


void TIM1_CC_IRQHandler(void){
  if((TIM1->SR & TIM_SR_CC1IF) == TIM_SR_CC1IF) {
    capture = TIM1->CCR1;
    TIM1->SR &= ~TIM_SR_CC1IF;
  }
}
