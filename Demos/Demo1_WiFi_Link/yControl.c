#include "yControl.h"


#define GET_CHANNEL_SCALE(X) 			(_mVPerDiv[(X)].Scale)
#define GET_CHANNEL_INC_FINE(X) 		(_mVPerDiv[(X)].IncFine)
#define GET_CHANNEL_DEC_FINE(X) 		(_mVPerDiv[(X)].DecFine)


/************************************/
/*�������ܣ��ṹ�幹�캯��  		*/
/*									*/
/************************************/
void YCtrl_CreateDefYSys(void)
{
	short i;

	for (i=0; i<CHANNEL_MAX_COUNT; i++)
	{
		gYSystem[i].ZeroPosition	= CHANNEL_MID_POS;
		gYSystem[i].Sensitivity		= LEV5V;
		gYSystem[i].Offset		= 0; 
		gYSystem[i].Fine			= 0;
	}

	gYSystem[CHANNEL_ID_CH1].pOps	=&_ChannelOps;	

	gYSystem[CHANNEL_ID_CH2].pOps	=&_ChannelOps;	

	hw_PushCommand(HWCMDID_YSYS_CH_STATUS);
}


/************************************/
/*�������ܣ��ֲ��������� 			*/
/*									*/
/************************************/

//��Ӧ��ֱ��λ�ĺ���ֵ(mv)    
static struct _YScale _mVPerDiv[]={
	{2,		150, 		-0},
	{5,		100, 		-150},
	{10,		100, 		-100},
	{20,		150, 		-100},
	{50,		100, 		-150},
	{100,		100, 		-100},	
	{200,		150, 		-100},
	{500,		100, 		-150},
	{1000,	100, 		-100},
	{2000,	150,		-100},
	{5000,	0,		-150},
	{10000,	0,		-0},
}; 

static int _mdBPerDiv[]={
	100,
	200,
	500,
	1000,
	2000,
	5000,
	10000,
	20000,
	50000,
	100000,
	200000,
};
//////////////////////////CH1/CH2ͨ�����ơ�״̬��ʾ////////////////////////////
float _GetChannelVolt(struct YSystem *ysystem)
{
	short probe;
	float y;

	if (ysystem->Fine >= 0)
		y = GET_CHANNEL_SCALE(ysystem->Sensitivity);
	else
		y = GET_CHANNEL_SCALE(ysystem->Sensitivity-1);

	y = ysystem->Fine * y / 100;
	y += GET_CHANNEL_SCALE(ysystem->Sensitivity);

	probe = ysystem->probe;
	while (probe--) y *= 10;
	return y;
}

struct YOperations _ChannelOps = {_GetChannelVolt};
//Yϵͳ�ı�������
struct _PhyChannel _oPhyChannel[CHANNEL_MAX_COUNT]=
{
	{
			(struct _TwinsData *) &CB_CH1,
			DA_CH1_BASE,
			DA_CH1_GAIN
	},

	{
			(struct _TwinsData *) &CB_CH2,
			DA_CH2_BASE,
			DA_CH2_GAIN
	}
};

struct YSystem gYSystem[CHANNEL_MAX_COUNT]=
{	
//CH1
	{			
			CHANNEL_MID_POS,	//��ֱ���ƽ��λ��
			0,					//��ֱ��ƽƫ��
			LEV100MV,			//��ֱƫתϵ�����ֵ���
			0,					//��ֱƫתϵ����ϸ����
			INVERT_CLOSE,		//�����־
			SS_CH_COUPLING_DC,	//ͨ����Ϸ�ʽ
			SS_CH_BANDWIDTH_OFF,//ͨ����������
			SS_CH_COAST,		//���ȵ��ڷ�ʽ 0:�ֵ� 1:ϸ��
			SS_CH_PROB_X1,		//̽ͷ���� 0:1x 1:10x 2:100x 3:1000x
			&_ChannelOps	
	},
//CH2
	{
			CHANNEL_MID_POS,	//��ֱ���ƽ��λ��
			0,					//��ֱ��ƽƫ��
			LEV100MV,			//��ֱƫתϵ�����ֵ���
			0,					//��ֱƫתϵ����ϸ����
			INVERT_CLOSE,		//�����־
			SS_CH_COUPLING_DC,	//ͨ����Ϸ�ʽ
			SS_CH_BANDWIDTH_OFF,//ͨ����������
			SS_CH_COAST,		//���ȵ��ڷ�ʽ 0:�ֵ� 1:ϸ��
			SS_CH_PROB_X1,		//̽ͷ���� 0:1x 1:10x 2:100x 3:1000x
			&_ChannelOps	
	},
};


/************************************/
/*�������ܣ�ͨ����Ϻ���			*/
/************************************/
void setCh1Coupling(char *chcoupling)
{
	gYSystem[CHANNEL_ID_CH1].chcoupling = *chcoupling;
}
void setCh2Coupling(char *chcoupling)
{
	gYSystem[CHANNEL_ID_CH2].chcoupling = *chcoupling;
}

/************************************/
/*�������ܣ�ͨ���������ƺ���		*/
/************************************/
void setCh1BandWidth(char *bandwidth)
{
	gYSystem[CHANNEL_ID_CH1].bandwidth = *bandwidth;
}
void setCh2BandWidth(char *bandwidth)
{
	gYSystem[CHANNEL_ID_CH2].bandwidth = *bandwidth;
}

/************************************/
/*�������ܣ�ͨ����������			*/
/************************************/
void setCh1VoltsPerDiv(char *voltdiv)
{
	gYSystem[CHANNEL_ID_CH1].adjustMode = *voltdiv;
}
void setCh2VoltsPerDiv(char *voltdiv)
{
	gYSystem[CHANNEL_ID_CH2].adjustMode = *voltdiv;
}

/************************************/
/*�������ܣ�ͨ��̽ͷ����			*/
/************************************/
void setCh1Probe(char *probe)
{
	gYSystem[CHANNEL_ID_CH1].probe = *probe;
}
void setCh2Probe(char *probe)
{
	gYSystem[CHANNEL_ID_CH2].probe = *probe;
}

/************************************/
/*�������ܣ�ͨ������ͨ������		*/
/************************************/
void setYsysInver(int channalnum,short *invnum)
{
	gYSystem[channalnum].Invert = *invnum;
}

//////////////////////////ͨ����λ����////////////////////////////
/************************************/
/*�������ܣ��������ĺϷ���		*/
/************************************/
void _IsSensitivityValid(struct YSystem *ysystem)
{
	if (ysystem->Sensitivity > CHANNEL_MAX_SCALE)
	{
		ysystem->Sensitivity = CHANNEL_MAX_SCALE;
	}
	else if (ysystem->Sensitivity < CHANNEL_MIN_SCALE)
	{
		ysystem->Sensitivity = CHANNEL_MIN_SCALE;
	}
}

/************************************/
/*�������ܣ�����ͨ����ֱƫתϵ��	*/
/************************************/
void YCtrl_SetYSensitivity(struct YSystem *ysystem, short Sensitivity)
{	
	ysystem ->Sensitivity = Sensitivity;
	ysystem ->Fine = 0;

	//�Ϸ��Լ��
	_IsSensitivityValid(ysystem);

	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_YSYS_ALL); 
}

/************************************/
/*�������ܣ�����ͨ����ֱƫתϵ��	*/
/************************************/
void YCtrl_AdjustYSensitivity(struct YSystem *ysystem, short delta)
{
	switch ( ysystem->adjustMode)
	{
	case ADJ_CHL_COARSE: //�ֵ�
		ysystem ->Sensitivity += delta;
		ysystem ->Fine = 0;
		_IsSensitivityValid(ysystem);
		break;
	case ADJ_CHL_FINE:   //ϸ�������������������
		ysystem ->Fine += delta;
		break;
	default: return;
	}
	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_YSYS_ALL); 
}

/************************************/
/*�������ܣ���ȡ��ֱͨ��ϵͳֵ		*/
/************************************/
float YCtrl_GetVoltPerDiv(struct YSystem *ysystem, struct UnitLogicData *pdata)
{
	float y;
	y = (ysystem->pOps->GetVoltPerDiv)(ysystem);
	if (pdata)
	{
		pdata->Value = y;
	}
	return y;
}

/************************************/
/*�������ܣ�ͨ��ƫ�ƺ���			*/
/************************************/
void YCtrl_SetZeroPosition(struct YSystem * ysystem, short Pos)
{	
	ysystem->ZeroPosition = Pos;	
	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_YSYS_BASELINE); 
}

/************************************/
/*�������ܣ�����ˮƽ��ƽλ��		*/
/************************************/
void YCtrl_AdjustZeroPosition(struct YSystem *ysystem, short Delta)
{
	ysystem->ZeroPosition += Delta;

	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_YSYS_ALL);
/**************����Ҫ���Ĵ�����ƽ�ʹ���λ��*******************/

}

//////////////////////�ô��д���ϵͳ�ĵ���/////////////////////
