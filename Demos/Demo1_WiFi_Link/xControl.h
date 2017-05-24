#ifndef X_CON
#define X_CON

#include <stdlib.h>
#include "stm32_fpga_command.h"
////////////////Xϵͳ�����///////////////
#define LEV10NS		  0 
#define LEV20NS		  1 
#define LEV50NS		  2 
#define LEV100NS		3 
#define LEV200NS	  4
#define LEV500NS	  5
#define LEV1US		  6
#define LEV2US		  7
#define LEV5US		  8
#define LEV10US		  9
#define LEV20US		  10
#define LEV50US		  11
#define LEV100US	  12
#define LEV200US	  13
#define LEV500US	  14
#define LEV1MS		  15
#define LEV2MS		  16
#define LEV5MS		  17
#define LEV10MS		  18
#define LEV20MS		  19
#define LEV50MS		  20
#define LEV100MS	  21
#define LEV200MS	  22
#define LEV500MS	  23
#define LEV1S		  24
#define LEV2S		  25
#define LEV5S		  26
#define LEV10S		  27
#define LEV20S		  28
#define LEV50S		  29


#define TIMEBASE_MAX_SCALE		LEV50S
#define TIMEBASE_MIN_SCALE		LEV10NS   //LEV1NS,��Сʱ����λ
#define SINX_MIN_SCALE			TIMEBASE_MIN_SCALE
#define SINX_MAX_SCALE			LEV50NS  //��ֵ��λ
#define REAL_MIN_SCALE			LEV100NS //LEV20NS����Сʵʱ��λ
#define R500_MIN_SCALE			LEV1US
#define REAL_MAX_SCALE			LEV20MS  //���ʵʱ��λ
#define PEAK_MIN_SCALE			LEV50US

#define TIMEBASE_ZERO_POS	150
#define TIMEBASE_MAX_POS	300
#define TIMEBASE_MAX_ZOOM	6
#define FFT_ZERO_POS		0

#define SCANSAMPLES		50
#define SCAN_MAX_CNT	0x1000
#define FIVEG_MAX_SCALE	LEV200NS

////////////////xϵͳ�����ݽṹ///////////////
struct XDepth
{
	double Time;			//����ʱ��
	long long Depth;		//������ȡ�Ԥ������ȷ�Χ
};

struct XSystem
{
	short Sensitivity;		//ʱ��
	short Samples;		      //��������
	int H_samples;			//Ӳ�������
	short Lenght;			//��ȡ��
	struct XDepth *pDepth;  	//������������ȣ�
};
struct _XScale
{
	unsigned long long 	Scale;
	long   				MinDepth;
	short 				Factor;
	int                 H_Factor;
};
////////////////��չx���Ͷ���///////////////
struct ZoomOperations;

struct ZoomSystem
{
	short Sensitivity;					//ʱ��
	long long Depth;						//չ����ʾ�����£��������λ��

	short Center;						//չ��������
	short Width;						//չ���ڿ��

	struct XSystem *pXModule;				//������չϵͳ

	const struct ZoomOperations *pOps;

};

struct ZoomOperations
{
	void (*GetTimePerDiv)(const struct ZoomSystem *, struct UnitLogicData *);
	long long (*GetXDepth)(const struct ZoomSystem *);

	void (*AdjXSensitivity)(struct ZoomSystem* , short);
	void (*AdjustXPosition)(struct ZoomSystem* , short);
};
/************************************/
/*�������ܣ�Ӳ������				*/
/************************************/
void XCtrl_Config(void);

/************************************/
/*�������ܣ��ṹ�幹�캯��			*/
/************************************/
void XCtrl_CreateDefXSys(void);

/************************************/
/*�������ܣ�������ʱ����λ			*/
/************************************/
void _AdjMasterSensitivity(struct XSystem *xsystem, short Delta);

/************************************/
/*�������ܣ����ݴ���ʱ������������*/
/************************************/
void _UpdateMasterPosition(struct XSystem *xsystem);

/************************************/
/*�������ܣ���������λ��			*/
/************************************/
void _AdjMasterPosition(struct XSystem *xsystem, short Delta);

/************************************/
/*�������ܣ����´���ʱ��			*/
/************************************/
float gGetXDelay(short Sensitivity, short Depth);

/************************************/
/*���ݵ�λ��ȡ��Ӧ��ms/DIV		    */
/************************************/
void gGetXLogicLevel(short Sensitivity, struct UnitLogicData *pLogicData);

////////////////��������///////////////
extern struct XSystem gXSystem[2];
extern struct ZoomSystem gZoomXSystem[2];

#endif
