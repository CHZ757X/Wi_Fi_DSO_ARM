#include "hw_fpga_Communicate.h"
extern void delay50ns(void);
/*
**
** stm32����fpga communication
**
*/
void data_Send(volatile uint8_t addr,volatile uint8_t senddata)
{	
	uint16_t data = senddata;
	uint16_t sendaddr = addr;
//	����8λ��ַ		
	sendaddr <<= 2;
	*((uint8_t*)&sendaddr + 1)<<=5;
	sendaddr <<= 1;	
//	����8λ����	
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
//	����8λ��ַ		
	DATA_RD_PORT->BSRRL = DATA_RD;//1
	DATA_PORT-> ODR = 0x0960;//���͵�ַ0xac
	DATA_WR_PORT->BSRRL = DATA_WR;//1	
	DATA_RD_PORT->BSRRH = DATA_RD;//0			
	DATA_GPIO_config(RD_FLAG);	
	while(1)
	{			
			DATA_RD_PORT->BSRRL = DATA_RD;//1			
			rettmp = GPIO_ReadInputData(DATA_PORT);	
			if(rettmp && 0x0180) break;
			i++;
			if(i >= 50000) //����һ��ʱ����� ˢ��һ������
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
	//��ʱ	
	for(time_i = 100000; time_i > 0; time_i--)
	{
		for(time_j = 40; time_j >0; time_j--);
	}
}

int wait_fifo(void) //�������ֵΪ1����Ӳ���������������Ϊ0����Ӳ������������
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
//����8λ��ַ		
	sendaddr <<= 2;
	*((uint8_t*)&sendaddr + 1)<<=5;
	sendaddr <<= 1;
	
	DATA_RD_PORT->BSRRH = DATA_RD;//0
	DATA_WR_PORT->BSRRH = DATA_WR;//0	
	DATA_GPIO_config(WR_FLAG);//�ܽ����ݷ������仯
	DATA_PORT-> ODR = sendaddr;//���͵�ַ
	DATA_RD_PORT->BSRRL = DATA_RD;//1
	DATA_WR_PORT->BSRRL = DATA_WR;//1	
	DATA_RD_PORT->BSRRH = DATA_RD;//0	
	DATA_GPIO_config(RD_FLAG);
//����datalenght�ֽڵ�����	
	for(i = 0 ;i < datalenght;i++)
	{	
//		if(i == datalenght/2) mxchipTick();	//��ֹ����ʱ��������������ն�
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
