#ifndef Y_CONT
#define Y_CONT

#include <stdio.h>
#include "systemDataStruct.h"
#include "stm32_fpga_command.h"

////////////////Y系统宏变量///////////////
#define LEV2MV		0
#define LEV5MV		1
#define LEV10MV		2
#define LEV20MV		3
#define LEV50MV		4
#define LEV100MV		5
#define LEV200MV		6
#define LEV500MV		7
#define LEV1V		8
#define LEV2V		9
#define LEV5V		10

#define CHANNEL_MAX_SCALE		LEV5V
#define CHANNEL_MIN_SCALE		LEV2MV
#define CHANNEL_LEVEL_RANGE		8
#define CHANNEL_MID_POS			0 
#define TIMEBASE_ZERO_POS		150
#define TIMEBASE_MAX_POS		300

//通道反相设计
#define INVERT_CLOSE				0
#define INVERT_OPEN					1	

//通道耦合方式
#define SS_CH_COUPLING_DC			0//直流耦合
#define SS_CH_COUPLING_AC			1//交流耦合
#define SS_CH_COUPLING_GND		    2//接地
//通道带宽限制
#define SS_CH_BANDWIDTH_OFF			0
#define SS_CH_BANDWIDTH_ON			1
//通道幅度/div档
#define SS_CH_COAST			0 //粗调
#define SS_CH_FINE			1 //细调
//探头比例
#define SS_CH_PROB_X1			0
#define SS_CH_PROB_X10			1
#define SS_CH_PROB_X100			2
#define SS_CH_PROB_X1000		3

#define ADJ_CHL_COARSE		0
#define ADJ_CHL_FINE		1



//////////////Y系统数据类型定义////////////////

struct _YScale{
	short Scale;
	short IncFine;
	short DecFine;
};

struct YSystem
{
	short ZeroPosition;		//垂直零电平位置
	short Offset;			//垂直电平偏差
	short Sensitivity;		//垂直偏转系数（粗调）
	short Fine;				//垂直偏转系数（细调）
	char Invert;			//反相
	char chcoupling;		//通道耦合方式
	char bandwidth;			//带宽限制
	char adjustMode;		//幅度调节方式
	char probe;				//探头比例 0:1x 1:10x 2:100x 3:1000x
	struct YOperations *pOps;			//垂直系统操作函数
};

struct YOperations
{
	float (*GetVoltPerDiv)(struct YSystem *);				//获取垂直通道偏转值	
};
/************************************/
/*函数功能：结构体构造函数  		*/
/*									*/
/************************************/
void YCtrl_CreateDefYSys(void);

/************************************/
/*函数功能：获取通道幅度值		*/
/************************************/
float _GetChannelVolt(struct YSystem *ysystem);

/************************************/
/*函数功能：通道耦合函数			*/
/************************************/
void setCh1Coupling(char *chcoupling);

void setCh2Coupling(char *chcoupling);

/************************************/
/*函数功能：通道带宽限制函数		*/
/************************************/
void setCh1BandWidth(char *bandwidth);

void setCh2BandWidth(char *bandwidth);

/************************************/
/*函数功能：通道调整函数			*/
/************************************/
void setCh1VoltsPerDiv(char *voltdiv);

void setCh2VoltsPerDiv(char *voltdiv);

/************************************/
/*函数功能：通道探头函数			*/
/************************************/
void setCh1Probe(char *probe);

void setCh2Probe(char *probe);

/************************************/
/*函数功能：通道反相				*/
/************************************/
void setYsysInver(int channalnum,short *invnum);

/************************************/
/*函数功能：通道档位函数			*/
/************************************/
void YCtrl_SetYSensitivity(struct YSystem *ysystem, short Sensitivity);

void YCtrl_AdjustYSensitivity(struct YSystem *ysystem, short delta);

float YCtrl_GetVoltPerDiv(struct YSystem *ysystem, struct UnitLogicData *pdata);
/************************************/
/*函数功能：通道偏移函数			*/
/************************************/
void YCtrl_SetZeroPosition(struct YSystem * ysystem, short Pos);

//////////////Y系统全局变量////////////////
extern struct _PhyChannel _oPhyChannel[];

extern struct YOperations _ChannelOps;

extern struct YSystem gYSystem[];


#endif
