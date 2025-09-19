#include <stdint.h>
#include <stdbool.h>
#define MAX (1<<12)-1

typedef struct {
  uint16_t rlr;
  uint8_t pr;
  bool valid;
}IWDVal_t;

IWDVal_t getWDVALS(uint32_t ms) {
  if(ms == 0) {
    return (IWDVal_t){0,0,false};
  }
  const int  pr_map[] = {4,8,16,32,64,128,256}; 
  IWDVal_t res = {0,0};
  res.valid = true;
  do {
    if(res.pr > 6){
      res.valid = false;
      break;
    }   
    uint32_t bPre = ms * (LSI_VALUE/1000);
    res.rlr = ( bPre/pr_map[res.pr] )-1;
    if(res.rlr > MAX){
      res.pr++;
    } 
  }while(res.rlr > MAX);
  return res;
}

void watchdog_setup(IWDVal_t values){
  IWDG->KR = 0x5555;
  IWDG->PR = values.pr;//wont work needs to be enum that maps true value
  IWDG->RLR = values.rlr;
  while((IWDG->SR & (IWDG_SR_PVU|IWDG_SR_RVU) ) != 0)
  IWDG->KR = 0xCCCC;
}

void changeTimer(IWDVal_t values){
  IWDG->KR = 0x5555;
  IWDG->RLR = values.rlr;
  IWDG->KR = 0xAAAA;
}
void reset_wd(){
  IWDG->KR = 0xAAAA;
}
