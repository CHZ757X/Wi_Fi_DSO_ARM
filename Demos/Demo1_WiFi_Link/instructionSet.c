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
		case 0://���ô�����ʽ
			SETTRIMODE(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_MODE);
			break;
		case 1://���ô���Դ
			SETTRISOUR(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_SOURCE);
			break;
		case 2://���ô�����Ϸ�ʽ
			SETTRICOUP(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_COUPLING);
			break;
		case 3://���ô�������
			SETTRITYPE(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_TYPE);
			break;
		case 4://���ô���б��
			SETTRISLOP(subtypenum);
			hw_PushCommand(HWCMDID_TSYS_EDGE);
			break;
		case 5://���ô�������
			SETTRIPOL(subtypenum);
		case 6://���ô�������
			SETTRICOND(subtypenum);
		case 7://�������������������
			SETTRIPW(*(unsigned long *)(&readbuffer[3]));
			hw_PushCommand(HWCMDID_TSYS_PW);
			break;
		case 8://���÷���
//			SETYINVER(chx,choiceid);
			hw_PushCommand(HWCMDID_YSYS_INVERT);
			break;
		case 9://����ͨ��1��Ϸ�ʽ
			setCh1Coupling(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 10://����ͨ��2��Ϸ�ʽ
			setCh2Coupling(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 11://����ͨ��1�Ĵ�������
			setCh1BandWidth(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 12://����ͨ��2�Ĵ�������
			setCh2BandWidth(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 13://����ͨ��1�ķ���/div
			setCh1VoltsPerDiv(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 14://����ͨ��2�ķ���/div
			setCh2VoltsPerDiv(&subtypenum);
			hw_PushCommand(HWCMDID_YSYS_ALL);
			break;
		case 15://����ͨ��1��̽ͷ
			setCh1VoltsPerDiv(&subtypenum);
			break;
		case 16://����ͨ��2��̽ͷ
			setCh2VoltsPerDiv(&subtypenum);
			break;
		case 28://����CH1�Ĵ�ֱ��λ
		YCtrl_SetYSensitivity(&gYSystem[0],readbuffer[3]);
			break;
		case 29://����CH2�Ĵ�ֱ��λ
		YCtrl_SetYSensitivity(&gYSystem[1],readbuffer[3]);
			break;
		case 30://����ʱ����
			_AdjMasterSensitivity(&gXSystem[CHANNEL_ID_PRIMARY], readbuffer[3]);
			break;
		case 50://Ӳ����ʼ��					
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
