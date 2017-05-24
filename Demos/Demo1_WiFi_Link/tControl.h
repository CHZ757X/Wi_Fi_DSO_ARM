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

/////////////////触发系统宏变量///////////////
//触发类型
#define SS_TRIGTYPE_EDGE			0 //边沿
#define SS_TRIGTYPE_PULSEWIDTH		1 //脉宽
//触发源
#define SS_TRIG_SOUR_CH1			0 //CH1
#define SS_TRIG_SOUR_CH2			1 //CH2	
//触发耦合方式
#define SS_TRIG_COUPLING_DC			0//直流耦合
#define SS_TRIG_COUPLING_AC			1//交流耦合
#define SS_TRIG_COUPLING_LFR		2//低频抑制
#define SS_TRIG_COUPLING_HFR		3//高频抑制
//触发方式
#define SS_TRIG_MODE_AUTO			0//自动
#define SS_TRIG_MODE_NORMAL			1//正常
#define SS_TRIG_MODE_SINGLE			2//单次
//斜率类型 
#define SS_TRIG_EDGE_RISING			0//上升沿
#define SS_TRIG_EDGE_FALL			1//下降沿
//脉宽极性
#define SS_TRIG_PW_POLARITY_NEG		0//脉宽极性：负
#define SS_TRIG_PW_POLARITY_POS		1//脉宽极性：正
//脉宽条件
#define SS_TRIG_PW_LT				0//小于
#define SS_TRIG_PW_GT				1//大于
#define SS_TRIG_PW_EQUAL			2//等于


///////////////触发系统数据结构///////////////
struct TriggerLevel
{
	short DACPort;			//DAC端口号	
	long Position;			//触发位置
	float Voltage;			//触发电压
};	

struct Trightvals
{
	char TrighType;						//触发类型
	char TrighSource;					//触发源
	char TrighCoupling;					//触发耦合
	char TrighMode;						//触发方式
	char EdgeSlope;						//斜率类型 
	char PulsePolarity;					//脉宽极性
	char PulseCondition;				//脉宽条件
	unsigned long pulseWidth;			//脉宽计数
	unsigned long HoldCount;			//触发释抑计数
	struct TriggerLevel 	Level[2];	//触发电平
};
/************************************/
/*函数功能：初始化触发系统			*/
/************************************/
void TCtrl_CreateDefTriggerSys(void);

/************************************/
/*函数功能：设置触发源				*/
/************************************/
void setTriSou(char trisourcenum);		

/************************************/
/*函数功能：设置触发模式			*/
/************************************/
void setTriMode(char trimodenum);		

/************************************/
/*函数功能：设置触发类型			*/
/************************************/
void setTriType(char tritypenum);		

/************************************/
/*函数功能：设置触发耦合			*/
/************************************/
void setTriCoup(char tricoupnum);

/************************************/
/*函数功能：设置触发斜率			*/
/************************************/
void setTriSlope(char triedgeslopenum);

/************************************/
/*函数功能：设置触发极性			*/
/************************************/
void setTriPol(char tripulpolnum);		

/************************************/
/*函数功能：设置触发条件		*/
/************************************/
void setTriCond(char tricondnum);

/************************************/
/*函数功能：设置触发脉宽		*/
/************************************/
void setTriPw(unsigned long tripulwidnum);

/************************************/
/*函数功能：设置触发释抑		*/
/************************************/
void setTriHoldCount(unsigned long HoldCount);

/************************************/
/*函数功能：设置触发源			*/
/************************************/
void setTriLevel(long position, float voltage, short dacport);	

/************************************/
/*函数功能：触发系统的全局变量 	*/
/************************************/
extern struct Trightvals TRIGHVAL;
#endif
