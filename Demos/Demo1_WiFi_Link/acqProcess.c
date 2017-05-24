#include "acqProcess.h"

extern void delay50ns(void);
extern void delay_us(uint32_t uscount);
short RandValidFlag; //��������·�Ƿ����
short Rand_Cp1, Rand_Cp2;

/************************************/
/*�������ܣ����cp1��cp2��Ƶ		*/
/************************************/
void GetRandCpMaxMin(void)
{			
	RandValidFlag=0;
	
	//#ifdef ENABLE_RAND
	data_Send(FPGA_PLL_RST,1);
	delay50ns();
	data_Send(FPGA_PLL_RST,0);
	
	delay_us((uint8_t)1000);
    
	data_Send(FPGA_COUNT_CLR,0);
	delay50ns();
        
	//��CP2
	data_Send(FPGA_EXP_PULSE_SEL,0x02);
	delay50ns();
	data_Send(FPGA_EXP_PULSE_SEL,0x00);
	delay50ns();
	data_Send(FPGA_COUNT_CLR,0x01);
	delay_us((uint8_t)100);

	//����
//	Rand_Cp2=DELTA_T_CNT & 0x0fff;
	    
	//��CP1
	data_Send(FPGA_EXP_PULSE_SEL,0x01);
	delay50ns();
	data_Send(FPGA_COUNT_CLR,0x00);
	delay50ns();
	data_Send(FPGA_COUNT_CLR,0x01);
	delay_us((uint8_t)10);
	
//	Rand_Cp1=DELTA_T_CNT & 0x0fff;
	
	data_Send(FPGA_EXP_PULSE_SEL,0x03);
	data_Send(FPGA_COUNT_CLR,0x00);
	delay50ns();
	data_Send(FPGA_COUNT_CLR,0x01);

	RandValidFlag=1;    
}

/******************************************************/
/*                    ȫ�ֱ�������                    */
/*									*/
/******************************************************/
//��ǰͨ��
short gAcqCurrentChannel=CHANNEL_ID_CH1;
//
struct AcqSystem gAcqSystem[CHANNEL_MAX_COUNT]=
{
	{
	    CHANNEL_ID_CH1,	  //  Identity
	    gZoomXSystem+CHANNEL_ID_PRIMARY,//pZoomModule	  
	},
	{
	    CHANNEL_ID_CH2,	//   Identity
	    gZoomXSystem+CHANNEL_ID_PRIMARY, //pZoomModule		
	},
	    
};
/******************************************************/
/*                    x��������                       */
/*									*/
/******************************************************/
void XCtrl_AdjustXSensitivity(struct AcqSystem* acsystem,short delta)
{
	(*acsystem->pZoomModule->pOps->AdjXSensitivity)(acsystem->pZoomModule, delta);
};
