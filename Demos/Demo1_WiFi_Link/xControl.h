#ifndef X_CON
#define X_CON

#include <stdlib.h>
#include "stm32_fpga_command.h"
////////////////X系统宏变量///////////////
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
#define TIMEBASE_MIN_SCALE		LEV10NS   //LEV1NS,最小时基档位
#define SINX_MIN_SCALE			TIMEBASE_MIN_SCALE
#define SINX_MAX_SCALE			LEV50NS  //插值档位
#define REAL_MIN_SCALE			LEV100NS //LEV20NS，最小实时档位
#define R500_MIN_SCALE			LEV1US
#define REAL_MAX_SCALE			LEV20MS  //最大实时档位
#define PEAK_MIN_SCALE			LEV50US

#define TIMEBASE_ZERO_POS	150
#define TIMEBASE_MAX_POS	300
#define TIMEBASE_MAX_ZOOM	6
#define FFT_ZERO_POS		0

#define SCANSAMPLES		50
#define SCAN_MAX_CNT	0x1000
#define FIVEG_MAX_SCALE	LEV200NS

////////////////x系统的数据结构///////////////
struct XDepth
{
	double Time;			//触发时间
	long long Depth;		//触发深度、预触发深度范围
};

struct XSystem
{
	short Sensitivity;		//时基
	short Samples;		      //软件抽点数
	int H_samples;			//硬件抽点数
	short Lenght;			//抽取数
	struct XDepth *pDepth;  	//关联触发（深度）
};
struct _XScale
{
	unsigned long long 	Scale;
	long   				MinDepth;
	short 				Factor;
	int                 H_Factor;
};
////////////////扩展x类型定义///////////////
struct ZoomOperations;

struct ZoomSystem
{
	short Sensitivity;					//时基
	long long Depth;						//展宽显示窗口下，触发点的位置

	short Center;						//展宽窗口中心
	short Width;						//展宽窗口宽度

	struct XSystem *pXModule;				//关联扩展系统

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
/*函数功能：硬件函数				*/
/************************************/
void XCtrl_Config(void);

/************************************/
/*函数功能：结构体构造函数			*/
/************************************/
void XCtrl_CreateDefXSys(void);

/************************************/
/*函数功能：调整主时基档位			*/
/************************************/
void _AdjMasterSensitivity(struct XSystem *xsystem, short Delta);

/************************************/
/*函数功能：根据触发时间调整触发深度*/
/************************************/
void _UpdateMasterPosition(struct XSystem *xsystem);

/************************************/
/*函数功能：调整触发位置			*/
/************************************/
void _AdjMasterPosition(struct XSystem *xsystem, short Delta);

/************************************/
/*函数功能：更新触发时间			*/
/************************************/
float gGetXDelay(short Sensitivity, short Depth);

/************************************/
/*根据档位获取对应的ms/DIV		    */
/************************************/
void gGetXLogicLevel(short Sensitivity, struct UnitLogicData *pLogicData);

////////////////变量声明///////////////
extern struct XSystem gXSystem[2];
extern struct ZoomSystem gZoomXSystem[2];

#endif
