#include "systemDataStruct.h"



uint8_t peakCheck = PKCHECK_OFF;

uint8_t gAcqStatusFlag = PROC_ARMED;

/************************************/
/*�������ܣ�ϵͳ��ʼ������		*/
/************************************/
void initSystem(void)
{
    YCtrl_CreateDefYSys();
    XCtrl_CreateDefXSys();
    TCtrl_CreateDefTriggerSys();
}
/************************************/
/*�������ܣ����ݲ���			*/
/************************************/
void _AcqInReal(void)
{
	unsigned short Flag;
	unsigned short TRI_LEVEL_FLAG;

	if (gAcqStatusFlag != PROC_STOP)		//���� 
	{	

//		Flag=FPGA_TRIG_FLAG ;		//��ȡϵͳ�ɼ�״̬

//		TRI_LEVEL_FLAG = TRI_LEVEL_STATUS&0x03;

		if (Flag & 0x0001)					//�ж��Ƿ�ΪAUTO
			gAcqStatusFlag=PROC_AUTO;
		else if (Flag & 0x0002)				//�ж��Ƿ��Ѵ���
			gAcqStatusFlag=PROC_TRIGED;
		else if (Flag & 0x0004)
			gAcqStatusFlag=PROC_READY;
		else
			gAcqStatusFlag=gAcqStatusFlag;    		

	//	Flag=FPGA_FIFO_FLAG;   		//��ȡFIFO����

		if (((Flag & 0x0005) == 0x0005))	//ADC1 ��ADC2������־
		{
			_ReadFifo();
			if ( TRIGHVAL.TrighMode == SS_TRIG_MODE_SINGLE && gAcqStatusFlag == PROC_TRIGED)
				gAcqStatusFlag = PROC_STOP;			
		}
	}	
}

/************************************/
/*�������ܣ���fifo�ж�ԭʼֵ		*/
/************************************/
void _ReadFifo(void)
{
	//���ݲɼ�
	if (gZoomXSystem[1].pXModule->Sensitivity < R500_MIN_SCALE)				//С��1uS��
	{
		Acq2Way();
//		if( gZoomXSystem[1].pXModule->Sensitivity < REAL_MIN_SCALE)	//С����Сʱ��
//		DPX_AcqSinx();		//��ֵ

	}
	else
	{
		Acq2Way();						//250MHz��ʽ
	}

//	ACQ_UpdatePhyStatus(CHANNEL_ID_CH1);//����״̬
//	ACQ_UpdatePhyStatus(CHANNEL_ID_CH2);

	dpx_InitFifo(0x8804);							//���³�ʼ��
}
/************************************/
/*�������ܣ��ɼ��洢����			*/
/************************************/
void Acq2Way(void)
{

}
