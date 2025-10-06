#include <stdint.h>
#include <stdbool.h>

#define WINDOW  83 

void watchdogSetup(){
  RCC->APB1ENR1 |= RCC_APB1ENR1_WWDGEN;
  WWDG->SR = 0; 
  WWDG->CFR |= WWDG_CFR_EWI; 

  WWDG->CFR &= ~WWDG_CFR_WDGTB_Msk;
  WWDG->CFR |= 3 << WWDG_CFR_WDGTB_Pos;

  WWDG->CFR &= ~WWDG_CFR_W_Msk;
  WWDG->CFR |= WINDOW<<WWDG_CFR_W_Pos;
  
  WWDG->CR &= ~WWDG_CR_T_Msk;
  WWDG->CR |= 100 << WWDG_CR_T_Pos;
  WWDG->CR |= WWDG_CR_WDGA;
}

bool test(){
  uint8_t counter = (WWDG->CR & WWDG_CR_T_Msk) >> WWDG_CR_T_Pos ; 
  return (counter > WINDOW)?false:true;
}
void updateTimer(){
  WWDG->CR &= ~WWDG_CR_T_Msk;
  WWDG->CR |= 100 << WWDG_CR_T_Pos;
}
