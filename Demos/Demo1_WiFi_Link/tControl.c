#include "tControl.h"

//触发参数的全局变量
struct Trightvals TRIGHVAL = 
{
		SS_TRIGTYPE_EDGE,				//触发类型
		SS_TRIG_SOUR_CH1,				//触发源
		SS_TRIG_COUPLING_DC,			//触发耦合
		SS_TRIG_MODE_AUTO,				//触发方式
		SS_TRIG_EDGE_RISING,			//斜率类型 
		SS_TRIG_PW_POLARITY_NEG,		//脉宽极性
		SS_TRIG_PW_LT,					//脉宽条件
		PULSE_MIN_WIDTH,				//脉宽计数
		HOLD_MIN_CNT,					//触发释抑计数
		{
			{
				DA_TG_LINE1,				//DAC端口号														
				TRIGGER_MID_POS,			//触发位置
				0.0							//触发电压
			},								
			{
				DA_TG_LINE2,				//DAC端口号														
				TRIGGER_MID_POS,			//触发位置
				0.0							//触发电压
			}
		}
};	
/************************************/
/*函数功能：初始化触发系统			*/
/************************************/
void TCtrl_CreateDefTriggerSys(void)
{
	TRIGHVAL.pulseWidth = PULSE_MIN_WIDTH;
	TRIGHVAL.HoldCount = HOLD_MIN_CNT;
	//触发源
	TRIGHVAL.TrighSource = SS_TRIG_SOUR_CH1;

	TRIGHVAL.Level[0].Position = TRIGGER_MID_POS;
	TRIGHVAL.Level[1].Position = TRIGGER_MID_POS;

	TRIGHVAL.Level[0].Voltage = 0.0;
	TRIGHVAL.Level[1].Voltage = 0.0;

}
/************************************/
/*函数功能：设置触发源				*/
/************************************/
void setTriSou(char trisourcenum)		
{
	TRIGHVAL.TrighSource = trisourcenum;
}
/************************************/
/*函数功能：设置触发模式			*/
/************************************/
void setTriMode(char trimodenum)		
{
	TRIGHVAL.TrighMode = trimodenum;
}
/************************************/
/*函数功能：设置触发类型			*/
/************************************/
void setTriType(char tritypenum)		
{
	TRIGHVAL.TrighType = tritypenum;
}
/************************************/
/*函数功能：设置触发耦合			*/
/************************************/
void setTriCoup(char tricoupnum)		
{
	TRIGHVAL.TrighCoupling = tricoupnum;
}
/************************************/
/*函数功能：设置触发斜率			*/
/************************************/
void setTriSlope(char triedgeslopenum)	
{
	TRIGHVAL.EdgeSlope = triedgeslopenum;
}
/************************************/
/*函数功能：设置触发极性			*/
/************************************/
void setTriPol(char tripulpolnum)		
{
	TRIGHVAL.PulsePolarity = tripulpolnum;
}
/************************************/
/*函数功能：设置触发条件			*/
/************************************/
void setTriCond(char tricondnum)		
{
	TRIGHVAL.PulseCondition = tricondnum;
}
/************************************/
/*函数功能：设置触发脉宽			*/
/************************************/
void setTriPw(unsigned long tripulwidnum)		
{
	TRIGHVAL.pulseWidth = tripulwidnum;
}
/************************************/
/*函数功能：设置触发释抑			*/
/************************************/
void setTriHoldCount(unsigned long HoldCount)		
{
	TRIGHVAL.HoldCount = HoldCount;
}
/************************************/
/*函数功能：设置触发源				*/
/************************************/
void setTriLevel(long position, float voltage, short dacport)
{
	TRIGHVAL.Level[TRIGHVAL.TrighSource].Position = position;
	TRIGHVAL.Level[TRIGHVAL.TrighSource].Voltage = voltage;
	TRIGHVAL.Level[TRIGHVAL.TrighSource].DACPort = dacport;
}
/************************************/
/*函数功能：获取触发电平的单位电压mV/DIV				*/
/************************************/
void _GetTriggerVoltPerDiv(struct Trightvals *Tsystem, struct UnitLogicData *pLogicValue)
{
    switch ( TRIGHVAL.TrighSource)
    {
        case SS_TRIG_SOUR_CH1: 	
			YCtrl_GetVoltPerDiv(gYSystem+CHANNEL_ID_CH1,pLogicValue); 
			return;
        case SS_TRIG_SOUR_CH2:	
			YCtrl_GetVoltPerDiv(gYSystem+CHANNEL_ID_CH2,pLogicValue); 
			return;
        default:		
        	pLogicValue->Value=3000; 
        	break;				
    }
    pLogicValue->oUnit.Unit=UNT_VOLTAGE;
    pLogicValue->oUnit.Multiple=UNT_MILLI;
}
/************************************/
/*函数功能：更新触发电压			*/
/************************************/
void _GetTriggerVoltage(struct TriggerLevel *trilevel, struct UnitLogicData *pLogicValue) 
{
	short y;

	_GetTriggerVoltPerDiv(&TRIGHVAL, pLogicValue);

	//在CH1和CH2反相时的处理
	if ( gYSystem[TRIGHVAL.TrighSource].Invert== INVERT_OPEN )
		y = -trilevel->Position;
	else
		y = trilevel->Position;

	pLogicValue->Value *= y ;
	pLogicValue->Value /= YDOTS_PER_DIV;	
}

/************************************/
/*函数功能：调整触发位置			*/
/************************************/
void TCtrl_AdjustPosition(struct Trightvals *Tsystem, short LevelIndex, short Delta)
{
	struct UnitLogicData LogicValue;

	Tsystem->Level[Tsystem->TrighSource].Position += Delta;

	//更新触发电压
	_GetTriggerVoltage(&Tsystem->Level[Tsystem->TrighSource], &LogicValue);

	Tsystem->Level[Tsystem->TrighSource].Voltage  = LogicValue.Value;

	//SEND HARDWARE COMMMAND
	hw_PushCommand(HWCMDID_TSYS_VOLT);
}
/************************************/
/*函数功能：设置触发电平			*/
/************************************/
void TCtrl_SetPosition(struct Trightvals *Tsystem, short LevelIndex, short Pos)
{
	short level_index;

	level_index = TRIGHVAL.TrighSource;

	Tsystem->Level[level_index].Position = Pos;

	TCtrl_AdjustPosition(Tsystem, LevelIndex, 0);
}
