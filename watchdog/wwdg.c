#include <stdint.h>
#include <stdbool.h>


typedef struct {
  uint8_t tick,
          window,
          prescale;
  bool trust;
}WWDVal_t;

uint32_t getApb(){
  uint32_t apbClock = SystemCoreClock;
  uint32_t apbPre = (RCC->CFGR & RCC_CFGR_PRE1_Msk);
  
  switch(apbPre){
    case 0b100:
      apbPre = 2;
      break;
    case 0b101:
      apbPre = 4;
      break;
    case 0b110:
      apbPre = 8;
      break;
    case 0b111:
      apbPre = 16;
      break;
    default:
      apbPre = 1;
  }
  return apbClock / apbPre;
}

WWDVal_t getWWDVal(uint32_t ms){
  uint32_t freq = (1000/ms);
  uint32_t apbFreq = getApb();//scaling it for ms
  WWDVal_t values = {0,0,1,true};
  do{
    values.tick = (freq * 4096 * (1<<values.prescale))/apbFreq;
    if(values.tick > (1<<6))
      values.prescale++;
  }while(values.tick > (1<<6));
  uint32_t wdgtb = 0;
  uint32_t div =  4096 *(1<<wdgtb);
}


void watchdogSetup(WWDVal_t val){
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

void updateTimer(WWDVal_t val){
  
  //WWDG->CR &= ~WWDG_CR_WDGA;
  WWDG->CFR &= ~WWDG_CFR_WDGTB_Msk;
  WWDG->CFR |= val.prescale << WWDG_CFR_WDGTB_Pos;

  WWDG->CFR &= ~WWDG_CFR_W_Msk;
  WWDG->CFR |= val.window<<WWDG_CFR_W_Pos;
  
  WWDG->CR &= ~WWDG_CR_T_Msk;
  WWDG->CR |= val.tick << WWDG_CR_T_Pos;
  WWDG->CR |= WWDG_CR_WDGA;
}
