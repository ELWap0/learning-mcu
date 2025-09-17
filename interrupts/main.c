#include<stdint.h>
#include<stdbool.h>

volatile bool state = false; 
void setup();
void delay_ms(uint32_t ms);

int main(){
  setup();
  while(true){
    if(state){
      GPIOA->BSRR = GPIO_BSRR_BS1;
    }else{
      GPIOA->BSRR = GPIO_BSRR_BR1;
    }
    delay_ms(1000);
  }
}

void setup(){
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOA;

  //setup pin
  GPIOA->MODER &= ~(GPIO_MODER_MODE0_Msk | GPIO_MODER_MODE1_Msk);
  GPIOA->MODER |= (0b01<<GPIO_MODER_MODE1_Pos);//connected to led
  GPIOA->PUPDR &= ~GPIO_PUPDR0_Msk;
  GPIOA->PUPDR |= (0b01<<GPIO_PUPDR0_Pos);

  SYSCFG->EXTICR1 &= ~SYSCFG_EXTICR1_EXTI0_Msk;

  EXTI->IMR1 |= EXTI_IMR1_IM0;
  EXTI->FTSR1 |= EXTI_FTSR1_FT0;
  EXTI->RTSR1 &= ~EXTI_FTSR1_RT0;
  EXTI->PR1 = EXTI_PR1_PIF0;
  NVIC_SetPriority(EXTI0_IRQn, 5); // pick a sensible priority for your system
  NVIC_EnableIRQ(EXTI0_IRQn);
}


void EXTI0_IRQHandler(void){
  EXTI->PR1 = EXTI_PR1_PIF0;
  state = state?false:true;
}

void delay_ms(uint32_t ms){
    // Configure SysTick to count down from SystemCoreClock/1000 each ms
    SysTick->LOAD  = (SystemCoreClock / 1000) - 1;
    SysTick->VAL   = 0; // reset current value
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (uint32_t i = 0; i < ms; i++) {
        // Wait until the COUNTFLAG bit is set (reload happened)
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) {
            // spin
        }
    }
    SysTick->CTRL = 0; // stop SysTick
}
