#include "systemDataStruct.h"



uint8_t peakCheck = PKCHECK_OFF;

uint8_t gAcqStatusFlag = PROC_ARMED;

/************************************/
/*函数功能：系统初始化函数		*/
/************************************/
void initSystem(void)
{
    YCtrl_CreateDefYSys();
    XCtrl_CreateDefXSys();
    TCtrl_CreateDefTriggerSys();
}
/************************************/
/*函数功能：数据采样			*/
/************************************/
void _AcqInReal(void)
{
	unsigned short Flag;
	unsigned short TRI_LEVEL_FLAG;

	if (gAcqStatusFlag != PROC_STOP)		//运行 
	{	

//		Flag=FPGA_TRIG_FLAG ;		//读取系统采集状态

//		TRI_LEVEL_FLAG = TRI_LEVEL_STATUS&0x03;

		if (Flag & 0x0001)					//判断是否为AUTO
			gAcqStatusFlag=PROC_AUTO;
		else if (Flag & 0x0002)				//判断是否已触发
			gAcqStatusFlag=PROC_TRIGED;
		else if (Flag & 0x0004)
			gAcqStatusFlag=PROC_READY;
		else
			gAcqStatusFlag=gAcqStatusFlag;    		

	//	Flag=FPGA_FIFO_FLAG;   		//读取FIFO数据

		if (((Flag & 0x0005) == 0x0005))	//ADC1 和ADC2的满标志
		{
			_ReadFifo();
			if ( TRIGHVAL.TrighMode == SS_TRIG_MODE_SINGLE && gAcqStatusFlag == PROC_TRIGED)
				gAcqStatusFlag = PROC_STOP;			
		}
	}	
}

/************************************/
/*函数功能：从fifo中读原始值		*/
/************************************/
void _ReadFifo(void)
{
	//数据采集
	if (gZoomXSystem[1].pXModule->Sensitivity < R500_MIN_SCALE)				//小于1uS档
	{
		Acq2Way();
//		if( gZoomXSystem[1].pXModule->Sensitivity < REAL_MIN_SCALE)	//小于最小时候档
//		DPX_AcqSinx();		//插值

	}
	else
	{
		Acq2Way();						//250MHz方式
	}

//	ACQ_UpdatePhyStatus(CHANNEL_ID_CH1);//保存状态
//	ACQ_UpdatePhyStatus(CHANNEL_ID_CH2);

	dpx_InitFifo(0x8804);							//重新初始化
}
/************************************/
/*函数功能：采集存储数据			*/
/************************************/
void Acq2Way(void)
{

}
