#include<stdint.h>
#include<stdbool.h>

void setup();

int main(){

}

void setup(){
  //RCC
  RCC->APB1ENR |= RCC_APB1ENR_I2C1ENR;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOA;
  RCC->CCIPR &= ~RCC_CCIPR_Msk;
  RCC->CCIPR |= 0b10 << RCC_CCIPR_Pos;

  //GPIO 
  GPIOA->MODER &= ~(GPIO_MODER_MODE9_Msk | GPIO_MODER_MODE10_Mks);
  GPIOA->MODER |= GPIO_MODER_MODE9_Msk | GPIO_MODER_MODE10_Mks;
  GPIOA->OSPEEDR &= (GPIO_OSPEEDR9_Msk | GPIO_OSPEEDR10_Msk);
  GPIOA->AFR[1]  &= ~(GPIO_AFR_AFSEL9_Msk | GPIO_AFR_AFSEL10_Msk);
  GPIOA->AFR[1]  |= (GPIO_AFR_AFSEL9_AF4 | GPIO_AFR_AFSEL10_AF4);

  //I2C
  I2C1->TIMINGR = 0;
  I2C1->TIMING |= ((15 << I2C_TIMING_PRESC_Pos) | ( 0 << I2C_TIMING_SCLDEL_Pos) |( 1 << I2C_TIMING_SCADEL_Pos) | (3 << I2C_TIMING_SCLH_Pos) | ( 4 << I2C_TIMING_SCLL_Pos))
  I2C1->CR1 &= ~I2C_CR1_ANFOFF;
  I2C1->CR2 &= ~I2C_CR2_ADD10;
}


void read(uint8_t address, uint8_t* buff, uint8_t len){
  //I2C1->RXDR
  I2C1->CR2 &= ~(I2C_CR2_RD_WRN|I2C_CR2_NBYTES_Msk|I2C_CR2_SADDR_Msk);
  I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos) | (address << I2C_CR2_SADDR_Pos) | I2C_CR2_RD_WRN ;
  I2C1->CR2 |= I2C_CR2_START;
  for(uint8_t x = 0; x < len; x++){
    while((I2C1->ISR & I2C_ISR_RXNE) != I2C_ISR_RXNE);
    buff[x] = I2C->RXDR;
  }

}

void write(uint8_t address, uint8_t* buff, uint8_t len){
  //I2C1->TXDR
  I2C1->CR2 &= ~(I2C_CR2_RD_WRN|I2C_CR2_NBYTES_Msk|I2C_CR2_SADDR_Msk);
  I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos) | (address << I2C_CR2_SADDR_Pos);
  I2C1->CR2 |= I2C_CR2_START;
  for(uint8_t x = 0; x < len; x++){
    while((I2C1->ISR & I2C_ISR_TXIS) != I2C_ISR_TXIS);
    I2C1->TXDR = buff[x];
  }
}
