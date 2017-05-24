#ifndef Y_CONT
#define Y_CONT

#include <stdio.h>
#include "systemDataStruct.h"
#include "stm32_fpga_command.h"

////////////////Yϵͳ�����///////////////
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

//ͨ���������
#define INVERT_CLOSE				0
#define INVERT_OPEN					1	

//ͨ����Ϸ�ʽ
#define SS_CH_COUPLING_DC			0//ֱ�����
#define SS_CH_COUPLING_AC			1//�������
#define SS_CH_COUPLING_GND		    2//�ӵ�
//ͨ����������
#define SS_CH_BANDWIDTH_OFF			0
#define SS_CH_BANDWIDTH_ON			1
//ͨ������/div��
#define SS_CH_COAST			0 //�ֵ�
#define SS_CH_FINE			1 //ϸ��
//̽ͷ����
#define SS_CH_PROB_X1			0
#define SS_CH_PROB_X10			1
#define SS_CH_PROB_X100			2
#define SS_CH_PROB_X1000		3

#define ADJ_CHL_COARSE		0
#define ADJ_CHL_FINE		1



//////////////Yϵͳ�������Ͷ���////////////////

struct _YScale{
	short Scale;
	short IncFine;
	short DecFine;
};

struct YSystem
{
	short ZeroPosition;		//��ֱ���ƽλ��
	short Offset;			//��ֱ��ƽƫ��
	short Sensitivity;		//��ֱƫתϵ�����ֵ���
	short Fine;				//��ֱƫתϵ����ϸ����
	char Invert;			//����
	char chcoupling;		//ͨ����Ϸ�ʽ
	char bandwidth;			//��������
	char adjustMode;		//���ȵ��ڷ�ʽ
	char probe;				//̽ͷ���� 0:1x 1:10x 2:100x 3:1000x
	struct YOperations *pOps;			//��ֱϵͳ��������
};

struct YOperations
{
	float (*GetVoltPerDiv)(struct YSystem *);				//��ȡ��ֱͨ��ƫתֵ	
};
/************************************/
/*�������ܣ��ṹ�幹�캯��  		*/
/*									*/
/************************************/
void YCtrl_CreateDefYSys(void);

/************************************/
/*�������ܣ���ȡͨ������ֵ		*/
/************************************/
float _GetChannelVolt(struct YSystem *ysystem);

/************************************/
/*�������ܣ�ͨ����Ϻ���			*/
/************************************/
void setCh1Coupling(char *chcoupling);

void setCh2Coupling(char *chcoupling);

/************************************/
/*�������ܣ�ͨ���������ƺ���		*/
/************************************/
void setCh1BandWidth(char *bandwidth);

void setCh2BandWidth(char *bandwidth);

/************************************/
/*�������ܣ�ͨ����������			*/
/************************************/
void setCh1VoltsPerDiv(char *voltdiv);

void setCh2VoltsPerDiv(char *voltdiv);

/************************************/
/*�������ܣ�ͨ��̽ͷ����			*/
/************************************/
void setCh1Probe(char *probe);

void setCh2Probe(char *probe);

/************************************/
/*�������ܣ�ͨ������				*/
/************************************/
void setYsysInver(int channalnum,short *invnum);

/************************************/
/*�������ܣ�ͨ����λ����			*/
/************************************/
void YCtrl_SetYSensitivity(struct YSystem *ysystem, short Sensitivity);

void YCtrl_AdjustYSensitivity(struct YSystem *ysystem, short delta);

float YCtrl_GetVoltPerDiv(struct YSystem *ysystem, struct UnitLogicData *pdata);
/************************************/
/*�������ܣ�ͨ��ƫ�ƺ���			*/
/************************************/
void YCtrl_SetZeroPosition(struct YSystem * ysystem, short Pos);

//////////////Yϵͳȫ�ֱ���////////////////
extern struct _PhyChannel _oPhyChannel[];

extern struct YOperations _ChannelOps;

extern struct YSystem gYSystem[];


#endif
