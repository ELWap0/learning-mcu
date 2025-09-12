void setup(){
  RCC->CCIPR |= (0b11 << RCC_CCIRP_ADCSEL_Pos);
  RCC->AHB2ENR |= RCC_APB2ENR_GPIOAEN | RCC_APB2ENR_ADCEN;
  GPIOA->MODER &= ~GPIOA_MODER_MODE0_msk;//restart state is analog mode but lets be safe
  GPIOA->MODER |= (0b11 <<GPIOA_MODER_MODE0_Pos);
  //calibrate adc
  ADC1->CR &= ~(ADC_CR_DEEPPWD | ADC_CR_ADCALDIF);
  ADC1->CR |=ADC_CR_ADVREGEN | ADC_CR_ADCAL;
  while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL);
  ADC1->CFGR &= ~ADC_CFGR_CONT;
  ADC1->SQR1 = 0;
  ADC1->SQR1 |= (5 << ADC_SQR1_SQ1_Pos);

}


void main(){
  setup();
  while(1){
    
  }
}
