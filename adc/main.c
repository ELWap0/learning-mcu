#include <core/stm32l433xx.h>
#include <stdint.h>
uint32_t freq = 4e6;

void delayMs(uint32_t ms) {
  uint32_t tick = freq / 1000;
  for (uint32_t x = 0; x < ms; x++) {
    for (uint32_t tickCounter = 0; tickCounter < tick; tickCounter++)
      ;
  }
}

void setup() {
  RCC->CCIPR |= (0b11 << RCC_CCIPR_ADCSEL_Pos);
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_ADCEN;
  GPIOA->MODER &=
      ~GPIO_MODER_MODE0_Msk; // restart state is analog mode but lets be safe
  GPIOA->MODER |= (0b11 << GPIO_MODER_MODE0_Pos);
  // calibrate adc
  ADC1->CR &= ~(ADC_CR_DEEPPWD | ADC_CR_ADCALDIF);
  ADC1->CR |= ADC_CR_ADVREGEN;
  delayMs(1);
  ADC1->CR |= ADC_CR_ADCAL;
  while ((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL)
    ;
  // single mode
  ADC1->CFGR = 0;
  // channel 5
  ADC1->SQR1 = 0;
  ADC1->SQR1 |= (5 << ADC_SQR1_SQ1_Pos);

  // sample time
  ADC1->SMPR1 |= (0b111 << ADC_SMPR1_SMP5_Pos);

  // enable
  ADC1->CR |= ADC_CR_ADEN;
  while ((ADC1->ISR & ADC_ISR_ADRDY) != ADC_ISR_ADRDY)
    ;
}

int main() {
  setup();
  uint32_t sum = 0;
  while (1) {
    ADC1->CR |= ADC_CR_ADSTART;
    while ((ADC1->ISR & ADC_ISR_EOC) != ADC_ISR_EOC)
      ;
    volatile uint32_t data = ADC1->DR;
    sum += data;
    delayMs(1000);
  }
}
