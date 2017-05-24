#ifndef COMMAND_H
#define COMMAND_H

#include "stm32f2xx.h"
#include "stm32_GPIO_Config.h"
#include "hw_fpga_Communicate.h"
#include "FPGA_register.h"
#include "systemDataStruct.h"
#include "yControl.h"
#include "xControl.h"
#include "dpx.h"

//HC4094控制位标志
#define HW4094BIT_CH2_BL1					0 //CH2 带宽限制
#define HW4094BIT_CH2_BL2					1
#define HW4094BIT_CH2_20MULTI_WEAKEN_DISABLE		3 //CH2 衰减20倍 0:有效
#define HW4094BIT_CH2_COUPLING_DC				4 //CH2 直流耦合控制 1:直流 0:交流
#define HW4094BIT_CH2_AMP10					5 //CH2 乘10倍放大
#define HW4094BIT_LOWF_DAMP_DISABLE				7 //低频抑制 0:有效
#define HW4094BIT_CH1_BL1					8 //CH1 带宽限制 0:满带宽 1:半带宽
#define HW4094BIT_CH1_BL2					9 
#define HW4094BIT_CH1_20MULTI_WEAKEN_DISABLE		11 //CH1 幅度衰减控制 0:衰减20
#define HW4094BIT_CH1_COUPLING_DC				12 //CH1 交直流耦合控制 1:直流 0:交流
#define HW4094BIT_CH1_AMP10					13 //通道1 乘10倍放大
#define HW4094BIT_HIGHF_DAMP_ENABLE 			14 //高频抑制 1:有效 0:无效
#define HW4094BIT_TRIG_COUPLING_DC				15 //触发信号交直流耦合控制 0:交流 1:直流 


//TaskID
//通道
#define HWCMDID_YSYS_BASELINE 			0	//调节通道基线的task
#define HWCMDID_YSYS_GAIN 				1	//通道的增益及带宽
#define HWCMDID_YSYS_INVERT				2	//波形反相
#define HWCMDID_YSYS_CH_STATUS			3	//通道选择
#define HWCMDID_YSYS_ALL 				4	//垂直系统的所有任务总和
//触发
#define HWCMDID_TSYS_MODE				5	
#define HWCMDID_TSYS_TYPE				6	
#define HWCMDID_TSYS_SOURCE				7	
#define HWCMDID_TSYS_COUPLING				8	
#define HWCMDID_TSYS_HOLDOFF				9	
#define HWCMDID_TSYS_EDGE				10	
#define HWCMDID_TSYS_PW					11		
#define HWCMDID_TSYS_FORCE				12	
#define HWCMDID_TSYS_VOLT				13	
#define HWCMDID_TSYS_ALL				14	//触发系统的所用任务总和
//XSystem
#define HWCMDID_XSYS_DEEP_MASTER			15	
#define HWCMDID_XSYS_SAMPLE_MASTER			16	
#define HWCMDID_XSYS_SAMPLE_SLAVE			17	
#define HWCMDID_XSYS_DEEP_SLAVE			18
#define HWCMDID_SYS_ALL 				19	

////////////////通道系统的数据结构///////////////
struct _TwinsData
{
	short Offset;
	short Gain;
}; 

struct _PhyChannel
{
	struct _TwinsData * pCalibrateBuffer;
	const short Ofs_DAC_Port;
	const short Gain_DAC_Port;
};

/*
*
*发送hc4094控制命令hc4094command
*param:发送的16位的控制字
*/
void sendhccommand(uint16_t hc4094command);

void hw_setTriPw( void );

void hw_setTriSource( void );                     

void hw_setTriEdge( void );

void hw_setTriMode( void );

void hw_setTriType( void );

void hw_hc4094ctrl( void );

void hw_PushCommand(unsigned short TaskID);

void hw_ExpandSysAll(void);
	
void hw_ExecuteCommand(void);

void hw_Setsample_Master(void);

void hw_SmartStart(void);

#endif
