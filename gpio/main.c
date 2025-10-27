#include <core/stm32l433xx.h>
#include <stdbool.h>

void setup();
void delayMs(uint32_t);

int main() {
  setup();
  bool led_status = true;
  while (true) {
    GPIOA->BSRR = led_status ? GPIO_BSRR_BS5 : GPIO_BSRR_BR5;
    led_status = !led_status;
    delayMs(1000);
  }
}

void setup() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
  GPIOA->MODER |= 0b01 << GPIO_MODER_MODE13_Pos;
}

void delayMs(uint32_t ms) {
  SysTick->LOAD = (SystemCoreClock / 1000) - 1;
  SysTick->VAL = 0; // reset current value
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

  for (uint32_t i = 0; i < ms; i++) {
    // Wait until the COUNTFLAG bit is set (reload happened)
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) {
      // spin
    }
  }
  SysTick->CTRL = 0; // stop SysTick
}
