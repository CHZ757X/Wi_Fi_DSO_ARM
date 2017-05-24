#include "commonfunc.h"
/*
**Delay(int t) 
**延时函数 us秒级的
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
