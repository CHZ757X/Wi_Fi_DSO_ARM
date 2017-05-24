#ifndef ACQ_PROCE
#define ACQ_PROCE

#include "FPGA_register.h"
#include "hw_fpga_Communicate.h"
#include "systemDataStruct.h"
#include "xControl.h"

void GetRandCpMaxMin(void);
/******************************************************/
/*                   类型定义		                  */
/*									*/
/******************************************************/
struct AcqWaveStatus;

struct AcqSystem
{
	short 	Identity;							//标识
	struct  	ZoomSystem 		*pZoomModule;			//水平系统

};
/******************************************************/
/*                    x函数声明                       */
/*									*/
/******************************************************/

void XCtrl_AdjustXSensitivity(struct AcqSystem* acsystem,short delta);

#endif
