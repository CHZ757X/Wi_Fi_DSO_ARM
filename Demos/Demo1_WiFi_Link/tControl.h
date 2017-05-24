#ifndef T_CONT
#define T_CONT
#include "yControl.h"

#define UNT_UNKNOWN 	0
#define UNT_SECOND		1
#define UNT_HERTZ	 	2
#define UNT_VOLTAGE 	3
#define UNT_DECIBEL 	4
#define UNT_VVOLTAGE 	5
#define UNT_V2VOLTAGE	6
#define UNT_VSECOND		7
#define UNT_V2SECOND	8
#define UNT_DEGREE		9
#define UNT_PERCENT		10

#define UNT_PICO		0
#define UNT_NANO		1
#define UNT_MICRO		2
#define UNT_MILLI		3
#define UNT_NULL		4
#define UNT_KILO		5
#define UNT_MEGA		6
#define UNT_GIGA		7

#define MAXSTRBUFSIZE	100

#define TRIGGER_LEVEL_RANGE		8

#define WIN_DATA_X0		14 
#define WIN_DATA_Y0		10 
#define WIN_DATA_X1		315  
#define WIN_DATA_Y1		213  

#define WIN_YDIV		8
#define WIN_XDIV		12

#define WIN_DATA_YMID	112
#define WIN_DATA_XMID	157  

#define MAXDISPDOTS		300//600
#define MAXDISPLEN		300//600

#define YDOTS_PER_DIV	((WIN_DATA_Y1-WIN_DATA_Y0)/WIN_YDIV)   //25
#define XDOTS_PER_DIV	((WIN_DATA_X1-WIN_DATA_X0)/WIN_XDIV)   //25

#define TRIGGER_MID_POS			0
#define TRIGGER_MAX_LEVEL		(TRIGGER_LEVEL_RANGE * YDOTS_PER_DIV/2)
#define TRIGGER_MIN_LEVEL		-TRIGGER_MAX_LEVEL

#define PULSE_CLK			8
#define PULSE_MAX_WIDTH		(10000000000/PULSE_CLK)
#define PULSE_MIN_WIDTH		(20/PULSE_CLK)

#define HOLD_CLK			8
#define HOLD_MAX_CNT		(1500000000/HOLD_CLK)
#define HOLD_MIN_CNT		(100/HOLD_CLK)

/////////////////����ϵͳ�����///////////////
//��������
#define SS_TRIGTYPE_EDGE			0 //����
#define SS_TRIGTYPE_PULSEWIDTH		1 //����
//����Դ
#define SS_TRIG_SOUR_CH1			0 //CH1
#define SS_TRIG_SOUR_CH2			1 //CH2	
//������Ϸ�ʽ
#define SS_TRIG_COUPLING_DC			0//ֱ�����
#define SS_TRIG_COUPLING_AC			1//�������
#define SS_TRIG_COUPLING_LFR		2//��Ƶ����
#define SS_TRIG_COUPLING_HFR		3//��Ƶ����
//������ʽ
#define SS_TRIG_MODE_AUTO			0//�Զ�
#define SS_TRIG_MODE_NORMAL			1//����
#define SS_TRIG_MODE_SINGLE			2//����
//б������ 
#define SS_TRIG_EDGE_RISING			0//������
#define SS_TRIG_EDGE_FALL			1//�½���
//������
#define SS_TRIG_PW_POLARITY_NEG		0//�����ԣ���
#define SS_TRIG_PW_POLARITY_POS		1//�����ԣ���
//��������
#define SS_TRIG_PW_LT				0//С��
#define SS_TRIG_PW_GT				1//����
#define SS_TRIG_PW_EQUAL			2//����


///////////////����ϵͳ���ݽṹ///////////////
struct TriggerLevel
{
	short DACPort;			//DAC�˿ں�	
	long Position;			//����λ��
	float Voltage;			//������ѹ
};	

struct Trightvals
{
	char TrighType;						//��������
	char TrighSource;					//����Դ
	char TrighCoupling;					//�������
	char TrighMode;						//������ʽ
	char EdgeSlope;						//б������ 
	char PulsePolarity;					//������
	char PulseCondition;				//��������
	unsigned long pulseWidth;			//�������
	unsigned long HoldCount;			//�������ּ���
	struct TriggerLevel 	Level[2];	//������ƽ
};
/************************************/
/*�������ܣ���ʼ������ϵͳ			*/
/************************************/
void TCtrl_CreateDefTriggerSys(void);

/************************************/
/*�������ܣ����ô���Դ				*/
/************************************/
void setTriSou(char trisourcenum);		

/************************************/
/*�������ܣ����ô���ģʽ			*/
/************************************/
void setTriMode(char trimodenum);		

/************************************/
/*�������ܣ����ô�������			*/
/************************************/
void setTriType(char tritypenum);		

/************************************/
/*�������ܣ����ô������			*/
/************************************/
void setTriCoup(char tricoupnum);

/************************************/
/*�������ܣ����ô���б��			*/
/************************************/
void setTriSlope(char triedgeslopenum);

/************************************/
/*�������ܣ����ô�������			*/
/************************************/
void setTriPol(char tripulpolnum);		

/************************************/
/*�������ܣ����ô�������		*/
/************************************/
void setTriCond(char tricondnum);

/************************************/
/*�������ܣ����ô�������		*/
/************************************/
void setTriPw(unsigned long tripulwidnum);

/************************************/
/*�������ܣ����ô�������		*/
/************************************/
void setTriHoldCount(unsigned long HoldCount);

/************************************/
/*�������ܣ����ô���Դ			*/
/************************************/
void setTriLevel(long position, float voltage, short dacport);	

/************************************/
/*�������ܣ�����ϵͳ��ȫ�ֱ��� 	*/
/************************************/
extern struct Trightvals TRIGHVAL;
#endif
