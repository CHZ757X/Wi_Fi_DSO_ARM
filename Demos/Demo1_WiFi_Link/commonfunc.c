#include "commonfunc.h"
/*
**Delay(int t) 
**��ʱ���� us�뼶��
*/
void delay_us(uint32_t uscount)
{
	int time_i = uscount;
	int time_j;
	for(; time_i > 0; time_i--)
	{
		for(time_j = 40; time_j >0; time_j--);
	}
}
