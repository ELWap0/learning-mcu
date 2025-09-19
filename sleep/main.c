#include <stdint.h>
#include <stdbool.h>

volatile bool state = false;
void setup();


int main(){
  setup();
  while(true){
    __WFI();
    GPIOA->BSRR = (state)?GPIO_BSRR_BS1:GPIO_BSRR_BR1;
  }
}

void setup(){
  //enable peripherals
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOA;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFG;
  RCC->APB2ENR |= RCC_APB1ENR_PWREN;

  //configure pins
  GPIOA->MODER &= ~(GPIO_MODER_MODE0_Msk| GPIO_MODER_MODE1_Msk);
  GPIOA->MODER |= 0b01 << GPIO_MODER_MODE1_Pos;
  GPIOA->PUPDR &= ~GPIO_PUPDR0_Msk;
  GPIOA->PUPDR |= 0b01 << GPIO_PUPDR0_Pos;

  //configure interrupts
  SYSCFG->EXTICR1 &= ~SYSCFG_EXTICR1_EXTI0_Msk;
  EXTI->IMR1 |= EXTI_IMR1_IM0;
  EXTI->RTSR1 |= EXTI_RTSR1_RT0;//wake up on release 
  EXTI->FTSR1 &= ~EXTI_FTSR1_FT0;
  EXTI->PR1 = EXTI_PR1_PIF0;


  //configure sleep mode 2
  PWR->CR1 &= ~PWR_CR1_LPMS_Msk;
  SCB->SCR |= SCB_SCR_SLEEPDEEP;
  //SCB->SCR |= SCB_SCR_SLEEPONEXIT;
  PWR->CR1 |= 0b010 << PWR_CR1_LPMS_Pos;
  NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(){
  EXTI->PR1 = EXTI_PR1_PIF0;
  state = !state;
}
