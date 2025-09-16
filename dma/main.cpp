#include<stdint.h>
#include<stdbool.h>

uint32_t freq = 4e6;
char buff[10] = {0};
volatile bool ready = false;

void setup();
void delayMs(uint32_t ms);


int main(){
  setup();
  while(1){
    if(ready){
      for(uint8_t sent = 0; sent < 10; sent++){
        while((USART1->ISR & USART_ISR_TXE) != USART_ISR_TXE);
        USART1->TDR = buff[sent];
      }
      ready = false;
      DMA1_Channel5->CNDTR  = 10;
      DMA1_Channel5->CCR   |= DMA1_CCR1_EN;
        //reset DMA
    }
  }
}

void setup(){
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
  RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_Msk;
  RCC->CCIPR |= (0b01U << RCC_CCIPR_USART1SEL_Pos);

  GPIOA->MODER &= ~(GPIO_MODER_MODE9_Msk | GPIO_MODER_MODE10_Msk);
  GPIOA->MODER |= ((0b10U << GPIO_MODER_MODE9_Pos) | (0b10U << GPIO_MODER_MODE10_Pos));
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR9_Msk | GPIO_OSPEEDR10_Msk);
  GPIOA->OSPEEDR |= ((0b01U << GPIO_OSPEEDR9_Pos) | (0b01U << GPIO_OSPEEDR10_Pos));
  GPIOA->AFR[1] &=~ (GPIO_AFRH_AFSEL9_Msk | GPIO_AFRH_AFSEL10_Msk);
  GPIOA->AFR[1] |= ( (0x7 << GPIO_AFRH_AFSEL9_Pos) | (0x7 <<GPIO_AFRH_AFSEL10_Msk));

  //DMA config
  DMA1_Channel5->CCR   &=~DMA1_CCR1_DIR;
  DMA1_Channel5->CCR   |= DMA1_CCR1_MINC;
  DMA1_Channel5->CNDTR  = 10;
  DMA1_Channel5->CPAR   = (uint32_t)&USART1->RDR;
  DMA1_Channel5->CMAR   = (uint32_t)buff;
  DMA1_CSELR->CSELR     =  (0b0010 << DMA1_CSELR1_C5S_Pos);
  DMA1_Channel5->CCR   |= DMA1_CCR1_EN | DMA1_CCR1_TCIE;

  //UART config
  USART1->CR1 &=~(USART_CR1_M0 | USART_CR1_M1);
  USART1->CR2 &=~ (USART_CR2_STOP_Msk);
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
  USART1->CR3 |= USART_CR3_DMAR;
  USART1->BRR = freq/9600;
  USART1->CR1 |= USART_CR1_UE;
  NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

void delayMs(uint32_t ms){
  int32_t tick = freq/1000;
  for(uint32_t x = 0; x < ms; x++){
    for(uint32_t tickCounter = 0; tickCounter < tick; tickCounter++);
  }
}


void DMA1_Channel5_IRQHandler(void){
  if((DMA1->ISR & DMA_ISR_TCIF5)){
    ready = true;
    DMA1->IFCR = DMA_IFCR_CTCIF5;
  }
}
