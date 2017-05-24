#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include "stm32f2xx.h"
#include "stm32_GPIO_Config.h"
#include "mxchipWNET.h"
#include "FPGA_register.h"
#include "stm32f2xx_it.h"

//send data to fpga from stm32
void data_Send(volatile uint8_t addr,volatile uint8_t data);

//receive datalenght¡®s data from fpga 
void data_Receive(uint8_t addr,uint8_t* bufferaddr, int datalenght);


int wait_fifo(void);

void fpga_s6_reset(void);

void wait_dcm(void);
#endif
