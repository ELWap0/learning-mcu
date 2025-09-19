#include <stdint.h>


typedef struct {
  uint8_t tick,
          window,
          prescale;
}WWDVal_t;


WWDVal_t getWWDVal(WWDVal_t ms){
  
}


void watchdog_setup(WWDVal_t val){
  RCC->APB1ENR1 |= RCC_APB1ENR1_WWDGEN;
  WWDG->SR = 0; 
  WWDG->CFR |= WWDG_CFR_EWI; 

  WWDG->CFR &= ~WWDG_CFR_WDGTB_Msk;
  WWDG->CFR |= val.prescale << WWDG_CFR_WDGTB_Pos;

  WWDG->CFR &= ~WWDG_CFR_W_Msk;
  WWDG->CFR |= val.window<<WWDG_CFR_W_Pos;
  
  WWDG->CR &= ~WWDG_CR_T_Msk;
  WWDG->CR |= val.tick << WWDG_CR_T_Pos;
  WWDG->CR |= WWDG_CR_WDGA;
}

void changeTimer(WWDVal_t val){
  
  WWDG->CR &= ~WWDG_CR_WDGA;
  WWDG->CFR &= ~WWDG_CFR_WDGTB_Msk;
  WWDG->CFR |= val.prescale << WWDG_CFR_WDGTB_Pos;

  WWDG->CFR &= ~WWDG_CFR_W_Msk;
  WWDG->CFR |= val.window<<WWDG_CFR_W_Pos;
  
  WWDG->CR &= ~WWDG_CR_T_Msk;
  WWDG->CR |= val.tick << WWDG_CR_T_Pos;
  WWDG->CR |= WWDG_CR_WDGA;
}

void reset_watchdog(){
   
}

void start_watchdog(){
  WWDG->CR |= WWDG_CR_WDGA;
}

void stop_watchdog(){
  WWDG->CR &= ~WWDG_CR_WDGA;
}
