#include<stdint.h>
#include<stdbool.h>

bool majorError = false;
bool minorError = false;


void setup(){
  //RCC
  RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL_Msk;
  RCC->CCIPR |= 0b10 << RCC_CCIPR_I2C1SEL_Pos;

  //GPIO 
  GPIOA->MODER &= ~(GPIO_MODER_MODE9_Msk | GPIO_MODER_MODE10_Msk);
  GPIOA->MODER |= (0b10 << GPIO_MODER_MODE9_Pos) | (0b10 << GPIO_MODER_MODE10_Pos);
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR9_Msk | GPIO_OSPEEDR10_Msk);
  GPIOA->PUPDR &= ~(GPIO_PUPDR9_Msk | GPIO_PUPDR10_Msk);
  GPIOA->OTYPER |=  GPIO_OTYPER_OT9 | GPIO_OTYPER_OT10;
  GPIOA->AFR[1] &= ~(GPIO_AFSEL9_Msk | GPIO_AFSEL10_Msk);
  GPIOA->AFR[1] |= ( 4 << GPIO_AFSEL9_Pos | 4 << GPIO_AFSEL10_Pos);

  //I2C
  I2C1->TIMINGR = 0;
  I2C1->TIMINGR |= ((15 << I2C_TIMING_PRESC_Pos) | ( 0 << I2C_TIMING_SCLDEL_Pos) |( 1 << I2C_TIMING_SDADEL_Pos) | (3 << I2C_TIMING_SCLH_Pos) | ( 4 << I2C_TIMING_SCLL_Pos));
  I2C1->CR1 &= ~I2C_CR1_ANFOFF;
  I2C1->CR2 &= ~(I2C_CR2_ADD10 | I2C_CR2_AUTOEND);
  I2C1->CR1 |= I2C_CR1_PE;
}

void sendRP(uint8_t address, uint8_t regPtr){

  I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk|I2C_CR2_SADD_Msk|I2C_CR2_RD_WRN);
  I2C1->CR2 |= (1 << I2C_CR2_NBYTES_Pos) | (address << I2C_CR2_SADD_Pos)  ;
  I2C1->CR2 |= I2C_CR2_START;
  while((I2C1->ISR & I2C_ISR_TXIS) != I2C_ISR_TXIS);
  I2C1->TXDR = regPtr;
  while((I2C1->ISR & I2C_ISR_TC) != I2C_ISR_TC);
}

void read(uint8_t address, uint8_t rp, uint8_t* buff, uint8_t len){
  //I2C1->RXDR
  sendRP(address, rp);

  I2C1->CR2 &= ~(I2C_CR2_RD_WRN|I2C_CR2_NBYTES_Msk|I2C_CR2_SADD_Msk);
  I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos) | (address << I2C_CR2_SADD_Pos) | I2C_CR2_RD_WRN;
  I2C1->CR2 |= I2C_CR2_START | I2C_CR2_AUTOEND; 
  for(uint8_t x = 0; x < len; x++){
    while((I2C1->ISR & I2C_ISR_RXNE) != I2C_ISR_RXNE);
    if((I2C1->ISR & I2C_ISR_BERR) == I2C_ISR_BERR){
      majorError = true;
      I2C1->CR1 &= ~I2C_CR1_PE;
      return;
    }else if((I2C1->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF){
      minorError = true;
      I2C1->CR1 &= ~I2C_CR1_PE;
      return;
    }    
    buff[x] = I2C1->RXDR;
  }
  while((I2C1->ISR & I2C_ISR_STOPF)  != I2C_ISR_STOPF);
  I2C1->ICR |= I2C_ICR_STOPCF;
}

void write(uint8_t address, uint8_t rp,uint8_t* buff, uint8_t len){
  //I2C1->TXDR
  sendRP(address, rp);
  I2C1->CR2 &= ~(I2C_CR2_RD_WRN|I2C_CR2_NBYTES_Msk|I2C_CR2_SADD_Msk);
  I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos) | (address << I2C_CR2_SADD_Pos);
  I2C1->CR2 |= I2C_CR2_START | I2C_CR2_AUTOEND; 
  for(uint8_t x = 0; x < len; x++){
    while((I2C1->ISR & I2C_ISR_TXIS) != I2C_ISR_TXIS);
    if((I2C1->ISR & I2C_ISR_BERR) == I2C_ISR_BERR){
      majorError = true;
      I2C1->CR1 &= ~I2C_CR1_PE;
      return;
    }else if((I2C1->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF){
      minorError = true;
      I2C1->CR1 &= ~I2C_CR1_PE;
      return;
    }else if((I2C1->ISR & I2C_ISR_ARLO) == I2C_ISR_ARLO) {
      minorError = true;
      I2C1->CR1 &= ~I2C_CR1_PE;
      return;
    }
    I2C1->TXDR = buff[x];
  }
  while((I2C1->ISR & I2C_ISR_STOPF)  != I2C_ISR_STOPF);
  I2C1->ICR |= I2C_ICR_STOPCF;
}
