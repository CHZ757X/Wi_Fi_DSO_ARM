#include "instructionSet.h"

int explainCommand(char *readbuffer, int lenght)
{
	int retval = 1;
	char typenum = readbuffer[2];
	char subtypenum = (readbuffer[3]-1);
	if(lenght <= 0)
	{
		return 0;
	}
	switch(typenum)
	{
		case 0://设置触发方式
			SETTRIMODE(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_MODE);
			break;
		case 1://设置触发源
			SETTRISOUR(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_SOURCE);
			break;
		case 2://设置触发耦合方式
			SETTRICOUP(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_COUPLING);
			break;
		case 3://设置触发类型
			SETTRITYPE(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_TYPE);
			break;
		case 4://设置触发斜率
			SETTRISLOP(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_EDGE);
			break;
		case 5://设置触发极性
			SETTRIPOL(subtypenum);
		case 6://设置触发条件
			SETTRICOND(subtypenum);
		case 7://设置脉宽触发的脉宽计数
			SETTRIPW(*(unsigned long *)(&readbuffer[3]));
			hw_PushCommand(HWCMDID_TSYS_PW);
			break;
		case 8://设置反相
//			SETYINVER(chx,choiceid);
			hw_PushCommand(HWCMDID_YSYS_INVERT);
			break;
		case 9://设置通道1耦合方式
			setCh1Coupling(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 10://设置通道2耦合方式
			setCh2Coupling(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 11://设置通道1的带宽限制
			setCh1BandWidth(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 12://设置通道2的带宽限制
			setCh2BandWidth(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 13://设置通道1的幅度/div
			setCh1VoltsPerDiv(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 14://设置通道2的幅度/div
			setCh2VoltsPerDiv(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 15://设置通道1的探头
			setCh1VoltsPerDiv(&subtypenum);
			break;
		case 16://设置通道2的探头
			setCh2VoltsPerDiv(&subtypenum);
			break;
		case 28://设置CH1的垂直档位
		YCtrl_SetYSensitivity(&gYSystem[0],readbuffer[3]);
			break;
		case 29://设置CH2的垂直档位
		YCtrl_SetYSensitivity(&gYSystem[1],readbuffer[3]);
			break;
		case 30://设置时基档
			_AdjMasterSensitivity(&gXSystem[CHANNEL_ID_PRIMARY], readbuffer[3]);
			break;
		case 50://硬件初始化					
			DATA_GPIO_config(WR_FLAG);		
			hw_SmartStart();	
		  break;
		default:
			retval = 0;
			break;
	}
	mxchipTick();	
	return retval;
}
