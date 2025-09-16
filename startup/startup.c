#include <stdint.h>

extern int main();

__attribute__((naked, noreturn)) void _reset(void) {
  extern uint32_t _sbss, _ebss, _sdata, _edata, _sidata;
  for(uint32_t *dst = &_sbss; dst < &_ebss; _sbss++) 
    *dst = 0;
  for(uint32_t *dst = &_sdata, *src = &_sidata; dst < &_edata; _sdata++, _sidata++)
    *dst = *src;
  main();
  while(1);
}

extern void _estack(void);

__attribute__((section(".vectors"))) void (*const tab[16+85])(void) = {
  _estack,
  _reset,
};
