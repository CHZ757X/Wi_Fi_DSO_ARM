#ifndef DPX_H
#define DPX_H
#include "FPGA_register.h"
#include "stm32f2xx.h"
///////////////////////////////////////////////////////////////////////////////////////////
///																						///	
///                                     ≥ı ºªØFIFO                                      ///
///																						///	
///////////////////////////////////////////////////////////////////////////////////////////
extern void delay_us(uint32_t uscount);

extern void data_Send(volatile uint8_t addr,volatile uint8_t senddata);

void dpx_InitFifo(short data);
#endif

