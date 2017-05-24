#include "dpx.h"
///////////////////////////////////////////////////////////////////////////////////////////
///																						///	
///                                     ³õÊ¼»¯FIFO                                      ///
///																						///	
///////////////////////////////////////////////////////////////////////////////////////////
void dpx_InitFifo(short data)
{
	data_Send(FPGA_FIFO_RW_CTR_H,0x00);
	data_Send(FPGA_FIFO_RW_CTR_L,0x00);
	
	data_Send(FPGA_FIFO_RESET,0x01);
	data_Send(FPGA_FIFO_RESET,0x00);
	
	data_Send(FPGA_FIFO_RW_CTR_H,(data >>8));
	data_Send(FPGA_FIFO_RW_CTR_L,data);
}
