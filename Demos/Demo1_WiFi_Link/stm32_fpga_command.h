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

//HC4094����λ��־
#define HW4094BIT_CH2_BL1					0 //CH2 ��������
#define HW4094BIT_CH2_BL2					1
#define HW4094BIT_CH2_20MULTI_WEAKEN_DISABLE		3 //CH2 ˥��20�� 0:��Ч
#define HW4094BIT_CH2_COUPLING_DC				4 //CH2 ֱ����Ͽ��� 1:ֱ�� 0:����
#define HW4094BIT_CH2_AMP10					5 //CH2 ��10���Ŵ�
#define HW4094BIT_LOWF_DAMP_DISABLE				7 //��Ƶ���� 0:��Ч
#define HW4094BIT_CH1_BL1					8 //CH1 �������� 0:������ 1:�����
#define HW4094BIT_CH1_BL2					9 
#define HW4094BIT_CH1_20MULTI_WEAKEN_DISABLE		11 //CH1 ����˥������ 0:˥��20
#define HW4094BIT_CH1_COUPLING_DC				12 //CH1 ��ֱ����Ͽ��� 1:ֱ�� 0:����
#define HW4094BIT_CH1_AMP10					13 //ͨ��1 ��10���Ŵ�
#define HW4094BIT_HIGHF_DAMP_ENABLE 			14 //��Ƶ���� 1:��Ч 0:��Ч
#define HW4094BIT_TRIG_COUPLING_DC				15 //�����źŽ�ֱ����Ͽ��� 0:���� 1:ֱ�� 


//TaskID
//ͨ��
#define HWCMDID_YSYS_BASELINE 			0	//����ͨ�����ߵ�task
#define HWCMDID_YSYS_GAIN 				1	//ͨ�������漰����
#define HWCMDID_YSYS_INVERT				2	//���η���
#define HWCMDID_YSYS_CH_STATUS			3	//ͨ��ѡ��
#define HWCMDID_YSYS_ALL 				4	//��ֱϵͳ�����������ܺ�
//����
#define HWCMDID_TSYS_MODE				5	
#define HWCMDID_TSYS_TYPE				6	
#define HWCMDID_TSYS_SOURCE				7	
#define HWCMDID_TSYS_COUPLING				8	
#define HWCMDID_TSYS_HOLDOFF				9	
#define HWCMDID_TSYS_EDGE				10	
#define HWCMDID_TSYS_PW					11		
#define HWCMDID_TSYS_FORCE				12	
#define HWCMDID_TSYS_VOLT				13	
#define HWCMDID_TSYS_ALL				14	//����ϵͳ�����������ܺ�
//XSystem
#define HWCMDID_XSYS_DEEP_MASTER			15	
#define HWCMDID_XSYS_SAMPLE_MASTER			16	
#define HWCMDID_XSYS_SAMPLE_SLAVE			17	
#define HWCMDID_XSYS_DEEP_SLAVE			18
#define HWCMDID_SYS_ALL 				19	

////////////////ͨ��ϵͳ�����ݽṹ///////////////
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
*����hc4094��������hc4094command
*param:���͵�16λ�Ŀ�����
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
