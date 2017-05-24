#include "tControl.h"

//����������ȫ�ֱ���
struct Trightvals TRIGHVAL = 
{
		SS_TRIGTYPE_EDGE,				//��������
		SS_TRIG_SOUR_CH1,				//����Դ
		SS_TRIG_COUPLING_DC,			//�������
		SS_TRIG_MODE_AUTO,				//������ʽ
		SS_TRIG_EDGE_RISING,			//б������ 
		SS_TRIG_PW_POLARITY_NEG,		//������
		SS_TRIG_PW_LT,					//��������
		PULSE_MIN_WIDTH,				//�������
		HOLD_MIN_CNT,					//�������ּ���
		{
			{
				DA_TG_LINE1,				//DAC�˿ں�														
				TRIGGER_MID_POS,			//����λ��
				0.0							//������ѹ
			},								
			{
				DA_TG_LINE2,				//DAC�˿ں�														
				TRIGGER_MID_POS,			//����λ��
				0.0							//������ѹ
			}
		}
};	
/************************************/
/*�������ܣ���ʼ������ϵͳ			*/
/************************************/
void TCtrl_CreateDefTriggerSys(void)
{
	TRIGHVAL.pulseWidth = PULSE_MIN_WIDTH;
	TRIGHVAL.HoldCount = HOLD_MIN_CNT;
	//����Դ
	TRIGHVAL.TrighSource = SS_TRIG_SOUR_CH1;

	TRIGHVAL.Level[0].Position = TRIGGER_MID_POS;
	TRIGHVAL.Level[1].Position = TRIGGER_MID_POS;

	TRIGHVAL.Level[0].Voltage = 0.0;
	TRIGHVAL.Level[1].Voltage = 0.0;

}
/************************************/
/*�������ܣ����ô���Դ				*/
/************************************/
void setTriSou(char trisourcenum)		
{
	TRIGHVAL.TrighSource = trisourcenum;
}
/************************************/
/*�������ܣ����ô���ģʽ			*/
/************************************/
void setTriMode(char trimodenum)		
{
	TRIGHVAL.TrighMode = trimodenum;
}
/************************************/
/*�������ܣ����ô�������			*/
/************************************/
void setTriType(char tritypenum)		
{
	TRIGHVAL.TrighType = tritypenum;
}
/************************************/
/*�������ܣ����ô������			*/
/************************************/
void setTriCoup(char tricoupnum)		
{
	TRIGHVAL.TrighCoupling = tricoupnum;
}
/************************************/
/*�������ܣ����ô���б��			*/
/************************************/
void setTriSlope(char triedgeslopenum)	
{
	TRIGHVAL.EdgeSlope = triedgeslopenum;
}
/************************************/
/*�������ܣ����ô�������			*/
/************************************/
void setTriPol(char tripulpolnum)		
{
	TRIGHVAL.PulsePolarity = tripulpolnum;
}
/************************************/
/*�������ܣ����ô�������			*/
/************************************/
void setTriCond(char tricondnum)		
{
	TRIGHVAL.PulseCondition = tricondnum;
}
/************************************/
/*�������ܣ����ô�������			*/
/************************************/
void setTriPw(unsigned long tripulwidnum)		
{
	TRIGHVAL.pulseWidth = tripulwidnum;
}
/************************************/
/*�������ܣ����ô�������			*/
/************************************/
void setTriHoldCount(unsigned long HoldCount)		
{
	TRIGHVAL.HoldCount = HoldCount;
}
/************************************/
/*�������ܣ����ô���Դ				*/
/************************************/
void setTriLevel(long position, float voltage, short dacport)
{
	TRIGHVAL.Level[TRIGHVAL.TrighSource].Position = position;
	TRIGHVAL.Level[TRIGHVAL.TrighSource].Voltage = voltage;
	TRIGHVAL.Level[TRIGHVAL.TrighSource].DACPort = dacport;
}
/************************************/
/*�������ܣ���ȡ������ƽ�ĵ�λ��ѹmV/DIV				*/
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
/*�������ܣ����´�����ѹ			*/
/************************************/
void _GetTriggerVoltage(struct TriggerLevel *trilevel, struct UnitLogicData *pLogicValue) 
{
	short y;

	_GetTriggerVoltPerDiv(&TRIGHVAL, pLogicValue);

	//��CH1��CH2����ʱ�Ĵ���
	if ( gYSystem[TRIGHVAL.TrighSource].Invert== INVERT_OPEN )
		y = -trilevel->Position;
	else
		y = trilevel->Position;

	pLogicValue->Value *= y ;
	pLogicValue->Value /= YDOTS_PER_DIV;	
}

/************************************/
/*�������ܣ���������λ��			*/
/************************************/
void TCtrl_AdjustPosition(struct Trightvals *Tsystem, short LevelIndex, short Delta)
{
	struct UnitLogicData LogicValue;

	Tsystem->Level[Tsystem->TrighSource].Position += Delta;

	//���´�����ѹ
	_GetTriggerVoltage(&Tsystem->Level[Tsystem->TrighSource], &LogicValue);

	Tsystem->Level[Tsystem->TrighSource].Voltage  = LogicValue.Value;

	//SEND HARDWARE COMMMAND
	hw_PushCommand(HWCMDID_TSYS_VOLT);
}
/************************************/
/*�������ܣ����ô�����ƽ			*/
/************************************/
void TCtrl_SetPosition(struct Trightvals *Tsystem, short LevelIndex, short Pos)
{
	short level_index;

	level_index = TRIGHVAL.TrighSource;

	Tsystem->Level[level_index].Position = Pos;

	TCtrl_AdjustPosition(Tsystem, LevelIndex, 0);
}
