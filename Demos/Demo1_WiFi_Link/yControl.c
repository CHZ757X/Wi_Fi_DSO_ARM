#include "yControl.h"


#define GET_CHANNEL_SCALE(X) 			(_mVPerDiv[(X)].Scale)
#define GET_CHANNEL_INC_FINE(X) 		(_mVPerDiv[(X)].IncFine)
#define GET_CHANNEL_DEC_FINE(X) 		(_mVPerDiv[(X)].DecFine)


/************************************/
/*函数功能：结构体构造函数  		*/
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
/*函数功能：局部变量定义 			*/
/*									*/
/************************************/

//对应垂直档位的毫伏值(mv)    
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
//////////////////////////CH1/CH2通道控制、状态显示////////////////////////////
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
//Y系统的变量声明
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
			CHANNEL_MID_POS,	//垂直零电平的位置
			0,					//垂直电平偏差
			LEV100MV,			//垂直偏转系数（粗调）
			0,					//垂直偏转系数（细调）
			INVERT_CLOSE,		//反向标志
			SS_CH_COUPLING_DC,	//通道耦合方式
			SS_CH_BANDWIDTH_OFF,//通道带宽限制
			SS_CH_COAST,		//幅度调节方式 0:粗调 1:细调
			SS_CH_PROB_X1,		//探头比例 0:1x 1:10x 2:100x 3:1000x
			&_ChannelOps	
	},
//CH2
	{
			CHANNEL_MID_POS,	//垂直零电平的位置
			0,					//垂直电平偏差
			LEV100MV,			//垂直偏转系数（粗调）
			0,					//垂直偏转系数（细调）
			INVERT_CLOSE,		//反向标志
			SS_CH_COUPLING_DC,	//通道耦合方式
			SS_CH_BANDWIDTH_OFF,//通道带宽限制
			SS_CH_COAST,		//幅度调节方式 0:粗调 1:细调
			SS_CH_PROB_X1,		//探头比例 0:1x 1:10x 2:100x 3:1000x
			&_ChannelOps	
	},
};


/************************************/
/*函数功能：通道耦合函数			*/
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
/*函数功能：通道带宽限制函数		*/
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
/*函数功能：通道调整函数			*/
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
/*函数功能：通道探头函数			*/
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
/*函数功能：通道设置通道反相		*/
/************************************/
void setYsysInver(int channalnum,short *invnum)
{
	gYSystem[channalnum].Invert = *invnum;
}

//////////////////////////通道档位调整////////////////////////////
/************************************/
/*函数功能：检查参数的合法性		*/
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
/*函数功能：设置通道垂直偏转系数	*/
/************************************/
void YCtrl_SetYSensitivity(struct YSystem *ysystem, short Sensitivity)
{	
	ysystem ->Sensitivity = Sensitivity;
	ysystem ->Fine = 0;

	//合法性检测
	_IsSensitivityValid(ysystem);

	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_YSYS_ALL); 
}

/************************************/
/*函数功能：调整通道垂直偏转系数	*/
/************************************/
void YCtrl_AdjustYSensitivity(struct YSystem *ysystem, short delta)
{
	switch ( ysystem->adjustMode)
	{
	case ADJ_CHL_COARSE: //粗调
		ysystem ->Sensitivity += delta;
		ysystem ->Fine = 0;
		_IsSensitivityValid(ysystem);
		break;
	case ADJ_CHL_FINE:   //细调，留到后面继续完善
		ysystem ->Fine += delta;
		break;
	default: return;
	}
	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_YSYS_ALL); 
}

/************************************/
/*函数功能：获取垂直通道系统值		*/
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
/*函数功能：通道偏移函数			*/
/************************************/
void YCtrl_SetZeroPosition(struct YSystem * ysystem, short Pos)
{	
	ysystem->ZeroPosition = Pos;	
	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_YSYS_BASELINE); 
}

/************************************/
/*函数功能：调整水平电平位置		*/
/************************************/
void YCtrl_AdjustZeroPosition(struct YSystem *ysystem, short Delta)
{
	ysystem->ZeroPosition += Delta;

	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_YSYS_ALL);
/**************这里要更改触发电平和触发位置*******************/

}

//////////////////////该处有触发系统的调整/////////////////////
