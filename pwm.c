#include <stdint.h>
#include <stdbool.h>

void setup();

int main(){

}


void setup(){
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;  
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  

  //pa8 -> TIM1_CH1
  GPIOA->MODER &= ~GPIO_MODER_MODE8_Msk;
  GPIOA->MODER |= 0b10 << GPIO_MODER_MODE8_Pos;
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8_Msk;
  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL8_Msk;
  GPIOA->AFR[1] |= 0x1 << GPIO_AFRH_AFSEL8_Pos;

  TIM1->CR1 &= ~TIM_CR1_DIR;
  TIM1->CR1 |= TIM_CR1_ARPE;
  TIM1->ARR = (SystemCoreClock/1000)-1;//1ms
  TIM1->CCR1 = 500-1;//50% duty cycle
  TIM1->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk | TIM_CCMR1_OC1M_Msk);
  TIM1->CCMR1 |=   (0b0110 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;
  TIM1->EGR |= TIM_EGR_UG;
  TIM1->BDTR |= TIM_BDTR_MOE;
  TIM1->CCER &= ~(TIM_CCER_CC1P);
  TIM1->CCER |= TIM_CCER_CC1E;
  TIM1->CR1 |=   TIM_CR1_CEN;
}


void updateDuty(uint8_t percent){
  percent = (percent < 100)?percent:100;
  uint16_t newDuty = (percent*(TIM1->ARR+1))/100;
  TIM1->CCR1 = newDuty;
}
