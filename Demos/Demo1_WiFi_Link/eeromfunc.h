#ifndef __STM322xG_EVAL_H
#define __STM322xG_EVAL_H

#include "stm32f2xx_it.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_i2c.h"
#include "commonfunc.h"

#define USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  I2C EEPROM Interface pins
  */  
#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
#define sEE_I2C_SCL_PIN                  GPIO_Pin_9                  /* PB.08 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource9
#define sEE_I2C_SDA_PIN                  GPIO_Pin_11             /* PB.06 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource11
#define PAGELENGHT 32
/**
	*@brief  向外部提供的I2C EEPROM 操作函数
	*/
int swi2c_writepage(uint8_t* i2caddr, uint16_t subaddr, uint8_t* pagesize);
int swi2c_write(uint8_t i2caddr, uint16_t subaddr, uint8_t data);
int swi2c_blkwrite(uint8_t i2caddr, uint16_t subaddr, uint8_t *data, uint32_t blksize);
int swi2c_read(uint8_t i2caddr, uint16_t subaddr, uint8_t *data);
int swi2c_blkread(uint8_t i2caddr, uint16_t subaddr, uint8_t *data, uint32_t blksize);
void swi2c_init(void);
/**
	*@brief  文件内使用的操作函数
	*/
void swi2c_scl_low(void);
void swi2c_scl_high(void);
void swi2c_sda_input(void);
void swi2c_sda_output(void);            
void swi2c_sda_low(void);                
void swi2c_sda_high(void);            
int swi2c_sda(void);    
#endif //__STM322xG_EVAL_H
