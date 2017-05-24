#ifndef ACQ_PROCE
#define ACQ_PROCE

#include "FPGA_register.h"
#include "hw_fpga_Communicate.h"
#include "systemDataStruct.h"
#include "xControl.h"

void GetRandCpMaxMin(void);
/******************************************************/
/*                   ���Ͷ���		                  */
/*									*/
/******************************************************/
struct AcqWaveStatus;

struct AcqSystem
{
	short 	Identity;							//��ʶ
	struct  	ZoomSystem 		*pZoomModule;			//ˮƽϵͳ

};
/******************************************************/
/*                    x��������                       */
/*									*/
/******************************************************/

void XCtrl_AdjustXSensitivity(struct AcqSystem* acsystem,short delta);

#endif
