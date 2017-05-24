#include "hw_fpga_Communicate.h"
extern void delay50ns(void);
/*
**
** stm32——fpga communication
**
*/
void data_Send(volatile uint8_t addr,volatile uint8_t senddata)
{	
	uint16_t data = senddata;
	uint16_t sendaddr = addr;
//	发送8位地址		
	sendaddr <<= 2;
	*((uint8_t*)&sendaddr + 1)<<=5;
	sendaddr <<= 1;	
//	发送8位数据	
	data <<= 2;
	*((uint8_t*)&data + 1)<<=5;
	data <<= 1;	
	
	DATA_RD_PORT->BSRRL |= DATA_RD;//1
	DATA_PORT-> ODR = sendaddr;	
	DATA_WR_PORT->BSRRL |= DATA_WR;//1		
	DATA_PORT-> ODR = data; 	
	DATA_WR_PORT->BSRRH  |= DATA_WR;//0	
	DATA_PORT-> ODR = 0x00;
	DATA_RD_PORT->BSRRH  |= DATA_RD;//0
}
void wait_dcm(void)
{
	int i = 0;	
	uint8_t rettmp;
//	发送8位地址		
	DATA_RD_PORT->BSRRL = DATA_RD;//1
	DATA_PORT-> ODR = 0x0960;//发送地址0xac
	DATA_WR_PORT->BSRRL = DATA_WR;//1	
	DATA_RD_PORT->BSRRH = DATA_RD;//0			
	DATA_GPIO_config(RD_FLAG);	
	while(1)
	{			
			DATA_RD_PORT->BSRRL = DATA_RD;//1			
			rettmp = GPIO_ReadInputData(DATA_PORT);	
			if(rettmp && 0x0180) break;
			i++;
			if(i >= 50000) //进行一次时序纠错 刷新一下网络
				break;
			DATA_RD_PORT->BSRRH = DATA_RD;//0		
	}
		DATA_RD_PORT->BSRRH = DATA_RD;//0
		DATA_WR_PORT->BSRRH = DATA_WR;//0	
}

void fpga_s6_reset(void)
{
	int time_j,time_i;
	GPIOA->BSRRH = S6_SET;
	//延时	
	for(time_i = 100000; time_i > 0; time_i--)
	{
		for(time_j = 40; time_j >0; time_j--);
	}
}

int wait_fifo(void) //如果返回值为1表明硬件工作正常，如果为0表明硬件工作不正常
{
	int i = 0,j = 0;	
	uint8_t rettmp;
	rettmp = GPIO_ReadInputData(ADDR_PORT);
	while((rettmp&0x02)!= 0x02)
	{
		if(i >= 50) 
		{
			i = 0;
			if(j++ >= 50000) 
			{
				fpga_s6_reset();
		//		SystemReset();	
				return 0;
			}				
			mxchipTick();	
			data_Send(FPGA_FIFO_RW_CTR_L,0x04);			
		}
		i++;
		rettmp = GPIO_ReadInputData(ADDR_PORT);
	}
	return 1;
}

void data_Receive(uint8_t addr, uint8_t* bufferaddr, int datalenght)
{
	int i;	
	uint16_t receivetmp = 0x0000;
	uint16_t sendaddr = addr;
//发送8位地址		
	sendaddr <<= 2;
	*((uint8_t*)&sendaddr + 1)<<=5;
	sendaddr <<= 1;
	
	DATA_RD_PORT->BSRRH = DATA_RD;//0
	DATA_WR_PORT->BSRRH = DATA_WR;//0	
	DATA_GPIO_config(WR_FLAG);//管脚数据方向发生变化
	DATA_PORT-> ODR = sendaddr;//发送地址
	DATA_RD_PORT->BSRRL = DATA_RD;//1
	DATA_WR_PORT->BSRRL = DATA_WR;//1	
	DATA_RD_PORT->BSRRH = DATA_RD;//0	
	DATA_GPIO_config(RD_FLAG);
//接收datalenght字节的数据	
	for(i = 0 ;i < datalenght;i++)
	{	
//		if(i == datalenght/2) mxchipTick();	//防止读数时间过长导致网络终端
		DATA_RD_PORT->BSRRL = DATA_RD;//1	
		receivetmp = GPIO_ReadInputData(DATA_PORT);				
		receivetmp >>=1;
		*((uint8_t*)&receivetmp+1)>>=5;
		receivetmp >>=2;		
 		*(bufferaddr+i) = (uint8_t)receivetmp;
		DATA_RD_PORT->BSRRH = DATA_RD;//0							
	}
	DATA_WR_PORT->BSRRH = DATA_WR;//0		
}
