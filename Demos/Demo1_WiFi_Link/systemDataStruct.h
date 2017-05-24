#ifndef SYSTEMSTRUCT_H
#define SYSTEMSTRUCT_H
#include "channal.h"
#include "stm32f2xx.h"
struct Unit
{
	short Unit;
	short Multiple;
};
struct UnitLogicData
{
	float Value;
	struct Unit oUnit;
};

#include "tControl.h"



//////////////////////////////////////////////////////////////////////////
///									 									
///                                   ״ֵ̬�궨��                        
///																		
//////////////////////////////////////////////////////////////////////////  
#define CHANNEL_MAX_COUNT 2 //���ͨ����
#define CHANNEL_ID_CH1 0
#define CHANNEL_ID_CH2 1

//////////////ͨ�õĺ궨��////////////////

#define PKCHECK_ON				1   //��ֵ��⿪��
#define PKCHECK_OFF				0   //��ֵ���ر�
#define CHANNEL_ID_PRIMARY		      1
#define CHANNEL_ID_SLAVE		      0
#define MAXDISPDOTS				300 //600
#define MAXDISPLEN				300 //600
#define SETTRIMODE(x) setTriMode(x);
#define SETTRITYPE(x) setTriType(x);
#define SETTRISOUR(x) setTriSou(x);
#define SETTRICOUP(x) setTriCoup(x);
#define SETTRISLOP(x) setTriSlope(x);
#define SETTRIPOL(x)  setTriPol(x);
#define SETTRICOND(x) setTriCond(x);
#define SETTRIPW(x)   setTriPw(x);
#define SETYINVER(chx,choiceid) setYsysInver(chx,choiceid);

///////////////�ɼ�״̬�ĺ궨��/////////////////
#define PROC_STOP 1
#define PROC_ARMED 2
#define PROC_READY 3
#define PROC_TRIGED 4
#define PROC_AUTO 5
#define ROC_REPLAY 6
#define PROC_SCAN 7
#define PROC_RESET 8


/************************************/
/*�������ܣ���fifo�ж�ԭʼֵ		*/
/************************************/
void _ReadFifo(void);

/************************************/
/*�������ܣ��ɼ��洢����			*/
/************************************/
void Acq2Way(void);

/************************************/
/*�������ܣ�ϵͳ��ʼ������		*/
/************************************/
void initSystem(void);

extern struct _XScale _oPicoSecPerDiv[];

extern uint8_t peakCheck;

extern uint8_t gAcqStatusFlag;

#endif

