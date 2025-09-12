//This will show you how to change the system clock
//Changing from MSI (Default clock source) to HSI16 (high speed internal)
//and configure it to 4MHZ
/*
 *RCC_CFGR ->  configuration register
 *RCC_CR -> control register
 */

void setup_clock(void){
  RCC->CR |= RCC_CR_HSION;
  while((RCC->CR & RCC_CR_HSRDY) != RCC_CR_HSRDY);
  RCC->CFGR &= ~RCC_CFGR_SW_Msk;
  RCC->CFGR |= RCC_CFGR_SW_HSI16;
  while((RCC->CFGR & RCC_CFGR_SWS_HSI16) != RCC_CFGR_SWS_HSI16);
  RCC->CR &= ~RCC_CR_MSION;
  RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
  RCC->CFGR |= RCC_CFGR_HPRE_DIV4;
}
