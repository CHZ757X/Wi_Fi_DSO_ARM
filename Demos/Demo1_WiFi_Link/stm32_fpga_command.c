#include "stm32_fpga_command.h"
extern void delay_us(uint32_t uscount);
extern void delay50ns(void);

/************************************/
/*函数功能：给通道DAC发送数据		*/
/*参数1:要发送的的数据	 			*/
/*									*/
/************************************/
void hw_SendDAC_Cmd(short port, unsigned int value)
{
	int i;
	unsigned long data;
	i=value;
	i=i*819/1000;	//保证步进值为1mv（ref电压为5v）			
	data = 0x00030000ul| (port << 12);
	data |= (unsigned short) i;
	data <<= 8; 
	data_Send(DAC5628_CONFIG_H,(uint8_t)(data>>24));
	data_Send(DAC5628_CONFIG_M1,(uint8_t)(data>>16));
	data_Send(DAC5628_CONFIG_M2,(uint8_t)(data>>8));
	data_Send(DAC5628_CONFIG_L,(uint8_t)data);    


	delay_us(10);
	data_Send(CH_AD5628_START, 0x01);
	delay_us(1000);
	data_Send(CH_AD5628_START, 0x00);	
	delay_us(500);
}
/************************************/
/*函数功能：初始化ADC			*/
/*						*/
/************************************/
void hw_InitADC(void)
{
	data_Send(ADC_DS_CTRL,3);	
	delay_us((uint8_t)1000); 
	data_Send(ADC_DS_CTRL,0);
	delay_us((uint8_t)1000);    
}
/************************************/
/*函数功能：初始化DAC								*/
/*																	*/
/************************************/
void hw_InitDAC(void)
{
	data_Send(DAC5628_CONFIG_H,0x08);
	data_Send(DAC5628_CONFIG_M1,0xf0);
	data_Send(DAC5628_CONFIG_M2,0x00);
	data_Send(DAC5628_CONFIG_L,0x01); 
	data_Send(CH_AD5628_START, 0x00);
	delay_us(20);
	data_Send(CH_AD5628_START, 0x01);
	
	delay_us((uint32_t)100000);
	
	hw_SendDAC_Cmd(0X0004, 0x432);   //ch1偏置
	hw_SendDAC_Cmd(0x0007, 2500);	   //base1_adj
	hw_SendDAC_Cmd(0x0006, 0x929);   //初始化增益CH1
	
	hw_SendDAC_Cmd(0X0003, 1010);    //ch2偏置
	
	hw_SendDAC_Cmd(0X0000, 2600);	   //TRIG2
	hw_SendDAC_Cmd(0X0002, 2600);	   //TRIG1

	hw_SendDAC_Cmd(0x0001, 1210);   //初始化增益CH2：0x91a
	hw_SendDAC_Cmd(0x0005, 1210);   //CH2基线：0x91a
}

void hw_SmartStart(void)
{
  /*data_Send(DCM_RST,1); //不需要进行DCM复位操作
	delay_us(500000);
	data_Send(DCM_RST,0);

	wait_dcm();//等待DCM配置成功
	*/
	data_Send(REAL_OR_RANDOM, 0x01);//0 equ  1: real

	//配置ADC
	hw_InitADC();

	//初始化DAC
	hw_InitDAC();

}


/************************************/
/*函数功能：readfifo data			*/
/*参数1:将要接收的数据量 			*/
/*参数2:接收数据的缓存区			*/
/************************************/
void readData(int dataCount, uint8_t* buffer)
{
	DATA_RD_PORT->BSRRL = DATA_RD;//1
	while(GPIO_ReadInputDataBit(DATA_PORT,DATA0) == 0);//等待fifo满
	DATA_RD_PORT->BSRRH = DATA_RD;//0
	DATA_GPIO_config(WR_FLAG);
	data_Send(0x00,0x00); 					//暂停fifo工作
	data_Send(0x01,0x00);
	data_Send(0x02,0x01);
	DATA_GPIO_config(RD_FLAG);
	data_Receive(0x02,buffer,dataCount);
}
/************************************/
/*函数功能：初始化buffer			*/
/*									*/
/************************************/
void initBuffer( void )
{
	//初始化FIFO，这里注意要进行data总线的数据方向的转换 DATA_GPIO_config(WR_FLAG);
	data_Send(0x02,0x00);
	data_Send(0x00,0x01);			
	data_Send(0x01,0x01);
};
/************************************/
/*函数功能：向hc4094发送命令		*/
/*									*/
/************************************/
void sendhccommand(uint16_t hc4094command)
{
	uint8_t hc4094com_H = (uint8_t)(hc4094command >> 8);
	uint8_t hc4094com_L = (uint8_t)hc4094command;
	
	DATA_GPIO_config(WR_FLAG);
	data_Send(HC4094_CTRL_DATA_H, hc4094com_H);
	data_Send(HC4094_CTRL_DATA_L, hc4094com_L);
	

	delay_us(5);
  data_Send(CH_HC4094_START, 0x01);
	delay_us(1000);
	data_Send(CH_HC4094_START, 0x00);
	delay_us(500);
	
//	data_Send(CH_HC4094_START, 0x00);
//	delay_us(20);
//  data_Send(CH_HC4094_START, 0x01);
};
/************************************/
/*函数功能：对16bit数字进行置位		*/
/*									*/
/************************************/
void HC_SetBit(uint16_t *bitData, short bitNum)
{
	*bitData |=(1<<bitNum);
}
void HC_ResetBit(uint16_t *bitData, short bitNum)
{
	*bitData &=~(1<<bitNum);
}
/************************************/
/*函数功能：配置hc4094				*/
/*									*/
/************************************/
void hw_hc4094ctrl( void )
{
	uint16_t low16 = 0;
	//设置耦合方式
	if( gYSystem[CHANNEL_ID_CH1].chcoupling == SS_CH_COUPLING_DC)
		HC_SetBit(&low16, HW4094BIT_CH1_COUPLING_DC);
	if( gYSystem[CHANNEL_ID_CH2].chcoupling == SS_CH_COUPLING_DC)
		HC_SetBit(&low16, HW4094BIT_CH2_COUPLING_DC);
	//乘10放大
	if(gYSystem[CHANNEL_ID_CH1].Sensitivity <= LEV10MV || gYSystem[CHANNEL_ID_CH1].Sensitivity == LEV200MV) 
		HC_SetBit(&low16, HW4094BIT_CH1_AMP10);
	if(gYSystem[CHANNEL_ID_CH2].Sensitivity <= LEV10MV || gYSystem[CHANNEL_ID_CH2].Sensitivity == LEV200MV)
		HC_SetBit(&low16, HW4094BIT_CH2_AMP10);
	//20倍衰减
	if(gYSystem[CHANNEL_ID_CH1].Sensitivity < LEV200MV)
		HC_SetBit(&low16,HW4094BIT_CH1_20MULTI_WEAKEN_DISABLE);
	if(gYSystem[CHANNEL_ID_CH2].Sensitivity < LEV200MV)
		HC_SetBit(&low16,HW4094BIT_CH2_20MULTI_WEAKEN_DISABLE);
	//带宽限制
	if(gYSystem[CHANNEL_ID_CH1].bandwidth == SS_CH_BANDWIDTH_ON)
		HC_SetBit(&low16,HW4094BIT_CH1_BL1);
	if(gYSystem[CHANNEL_ID_CH2].bandwidth == SS_CH_BANDWIDTH_ON)
		HC_SetBit(&low16,HW4094BIT_CH2_BL1);

	switch( TRIGHVAL.TrighCoupling )
	{
		case SS_TRIG_COUPLING_DC:
			HC_SetBit(&low16, HW4094BIT_TRIG_COUPLING_DC); 	//开启触发直流耦合
			HC_SetBit(&low16, HW4094BIT_LOWF_DAMP_DISABLE); //关闭低频抑制，此时高频抑制也是关闭的
			break;
		case SS_TRIG_COUPLING_AC:
			HC_SetBit(&low16, HW4094BIT_LOWF_DAMP_DISABLE); //关闭低频抑制，此时高频抑制也是关闭的
			break;
		case SS_TRIG_COUPLING_LFR: 					//此时低频抑制是开启的			
			break;
		case SS_TRIG_COUPLING_HFR: 
			HC_SetBit(&low16, HW4094BIT_HIGHF_DAMP_ENABLE); //打开高频抑制
			break;
	}
//	printf("4094 : %x\n",low16);
	sendhccommand(low16);							//向硬件发送命令
	delay50ns();
}

/************************************/
/*函数功能：设置触发类型			*/
/*									*/
/************************************/
void hw_setTriType( void )
{
	data_Send(FPGA_TRIG_TYPE, TRIGHVAL.TrighType);			//向硬件发送改变触发类型的命令
	
	switch(TRIGHVAL.TrighType)
	{
		case SS_TRIGTYPE_EDGE:
			hw_setTriEdge();						//重新设置触发沿
			break;
		case SS_TRIGTYPE_PULSEWIDTH:
			hw_setTriPw();
			break;
	}
}
/************************************/
/*函数功能：设置触发方式			*/
/*									*/
/************************************/
void hw_setTriMode( void )
{
	data_Send(FPGA_TRIG_MODE, TRIGHVAL.TrighMode);
	/////////这里还要设置一下采集状态的标志
}
/************************************/
/*函数功能：设置斜率类型			*/
/*									*/
/************************************/
void hw_setTriEdge( void )
{
	if(TRIGHVAL.TrighType != SS_TRIGTYPE_EDGE)
	{
		return;
	}
	data_Send(FPGA_TRIG_EDGE, TRIGHVAL.EdgeSlope);			//向硬件发送改变触发斜率类型的命令
}
/************************************/ 
/*函数功能：设置触发源				*/
/*									*/
/************************************/
void hw_setTriSource( void )
{
	data_Send(FPGA_TRIG_SOURCE,TRIGHVAL.TrighSource);
}
/************************************/
/*函数功能：脉宽设置				*/
/*									*/
/************************************/
void hw_setTriPw( void )
{
	int tmpTriPulWidth = TRIGHVAL.pulseWidth;
	if(TRIGHVAL.TrighType != SS_TRIGTYPE_PULSEWIDTH)
	{
		return;
	}
	//发送脉宽极性
	data_Send(FPGA_TRIG_EDGE, TRIGHVAL.PulsePolarity);
	//发送脉宽条件
	data_Send(FPGA_PW_COND_CTRL, TRIGHVAL.PulseCondition);
	//发送脉宽计数
	data_Send(FPGA_PW_DATA_H,(char)((tmpTriPulWidth>>24)&0xff));
	data_Send(FPGA_PW_DATA_M1,(char)((tmpTriPulWidth>>16)&0xff));
	data_Send(FPGA_PW_DATA_M2,(char)((tmpTriPulWidth>>8)&0xff));
	data_Send(FPGA_PW_DATA_L,(char)(tmpTriPulWidth&0xff));			
}
/************************************/
/*函数功能：设置水平触发位置		*/
/*									*/
/************************************/
void hw_setTrivolt()
{
	struct TriggerLevel trilevel= TRIGHVAL.Level[TRIGHVAL.TrighSource];
	short data;
	float y;
	data=CB_VALUE_TRIG_AC-10;//触发位置校正过程
	if (TRIGHVAL.TrighCoupling == SS_TRIG_COUPLING_DC || TRIGHVAL.TrighCoupling == SS_TRIG_COUPLING_HFR)
	{		
		data=CB_VALUE_TRIG_DC(TRIGHVAL.TrighSource);//触发位置校正过程
		////判断是否为反向，然后计算触发位置
		if (gYSystem[TRIGHVAL.TrighSource].Invert == INVERT_OPEN) //判断通道反向选项是否打开
			data=data+12*(gYSystem[TRIGHVAL.TrighSource].ZeroPosition-CHANNEL_MID_POS)/5;   //down
		else
			data=data-12*(gYSystem[TRIGHVAL.TrighSource].ZeroPosition-CHANNEL_MID_POS)/5;  //up
	}
	y = 45*trilevel.Position/8;
	data -=(short)y;
	if (data<0)
			data=0;
	else if (data>0x0fff) 
			data=0x0fff;
	CB_VALUE_TRIG_CUR=data;

	hw_SendDAC_Cmd(trilevel.DACPort,data);
}
/************************************/
/*函数功能：设置触发释抑			*/
/*									*/
/************************************/
void hw_setTriholdoff()
{
	unsigned long data = TRIGHVAL.HoldCount;
	data_Send(FPGA_HOLDOFF_H,(uint8_t)(data>>24));
	data_Send(FPGA_HOLDOFF_M1,(uint8_t)(data>>16));
	data_Send(FPGA_HOLDOFF_M2,(uint8_t)(data>>8));
	data_Send(FPGA_HOLDOFF_L,(uint8_t)data);
}

//////////////////////////////////////
//////////////垂直系统////////////////
//////////////////////////////////////
/************************************/
/*函数功能：增益变化				*/
/*									*/
/************************************/
void hw_Ysys_gain()
{
	uint8_t ChannelID;
	struct YSystem *pYSys;
	struct _TwinsData *pCurrentAdjData;
	struct _PhyChannel *pPhySys;

//	for(ChannelID=0; ChannelID < 2; ChannelID++)
	{
		ChannelID = 1;
		
		pYSys= &gYSystem[ChannelID];

		pPhySys= &_oPhyChannel[ChannelID];

		pCurrentAdjData = pPhySys->pCalibrateBuffer + pYSys->Sensitivity;

		hw_SendDAC_Cmd(pPhySys->Gain_DAC_Port, pCurrentAdjData->Gain);
		
//		printf("%d  %d\n",pPhySys->Gain_DAC_Port, pCurrentAdjData->Gain);
	}
}
/************************************/
/*函数功能：调整基线				*/
/*									*/
/************************************/
static void hw_Ysys_baseline(void)//基线移动
{

	short data,ChannelID;
	const struct YSystem *pYSys;
	const struct _TwinsData *pCurrentAdjData;
	const struct _PhyChannel *pPhySys;
	short DAValueUp3Div;
	short pre_offset;

//	for(ChannelID = 0; ChannelID < 2; ChannelID++)
//	{
		ChannelID = 1;//只控制通道2
	
		pYSys = &gYSystem[ChannelID];

		pPhySys = &_oPhyChannel[ChannelID];

		pCurrentAdjData = pPhySys->pCalibrateBuffer + pYSys->Sensitivity;
	
		hw_SendDAC_Cmd(pPhySys->Ofs_DAC_Port, pCurrentAdjData->Offset);
	
	//	printf("%d  %d\n",pPhySys->Ofs_DAC_Port, pCurrentAdjData->Offset);
		
	/*			
		pre_offset=(ChannelID==0) ? DA_CH1_OFFSET : DA_CH2_OFFSET;  //前级偏校正

		//偏置电压调节
		hw_SendDAC_Cmd(pre_offset, CB_VALUE_PRE_OFS(ChannelID));		*/
//	}
}
//////////////////////////////////////
/////////////////X系统////////////////
//////////////////////////////////////
/************************************/
/*函数功能：触发深度设置			*/
/*	（预触发和后触发）				*/
/************************************/
static void hw_xsys_deepMaster(void)
{
	struct XSystem xsystem = gXSystem[CHANNEL_ID_PRIMARY];
	if (xsystem.pDepth->Depth < 0)	//设置延迟触发
	{
		unsigned long long DelayTrigCnt;

		DelayTrigCnt = (unsigned long long)(-xsystem.pDepth->Depth);
		DelayTrigCnt = DelayTrigCnt * (_oPicoSecPerDiv[xsystem.Sensitivity].Scale/(4 * XDOTS_PER_DIV * 1000));

		DelayTrigCnt /= 2;

		data_Send(FPGA_POS_TRIG_H,(uint8_t)(DelayTrigCnt >> 32));
		data_Send(FPGA_POS_TRIG_M1,(uint8_t)(DelayTrigCnt >> 24));
		data_Send(FPGA_POS_TRIG_M2,(uint8_t)(DelayTrigCnt >> 16));
		data_Send(FPGA_POS_TRIG_M3,(uint8_t)(DelayTrigCnt >> 8));
		data_Send(FPGA_POS_TRIG_L,(uint8_t)DelayTrigCnt);

		data_Send(FPGA_PRE_TRIG_H, 0x00);
		data_Send(FPGA_PRE_TRIG_M,0x00);
		data_Send(FPGA_PRE_TRIG_L,0x00);
	}
	else							//设置预触发
	{
		unsigned long PreTrigCnt;

		PreTrigCnt=(unsigned long)xsystem.pDepth->Depth;

		PreTrigCnt *= _oPicoSecPerDiv[(xsystem.Sensitivity >= REAL_MIN_SCALE) ? xsystem.Sensitivity : REAL_MIN_SCALE].Factor;

		PreTrigCnt/=2;

		if(xsystem.Sensitivity>=REAL_MIN_SCALE)
		{
			PreTrigCnt -= 20;
		}

		if (xsystem.Sensitivity>=R500_MIN_SCALE)
		{
			PreTrigCnt+=10;
		}
		else
		{
			PreTrigCnt/=1;  
		}

		if(xsystem.Sensitivity<=LEV50NS)
		{
			PreTrigCnt = 2000;
		}

		data_Send(FPGA_PRE_TRIG_H,(uint8_t)(PreTrigCnt >> 24));
		data_Send(FPGA_PRE_TRIG_M,(uint8_t)(PreTrigCnt >> 16));
		data_Send(FPGA_PRE_TRIG_L,(uint8_t)(PreTrigCnt >> 8));

		data_Send(FPGA_POS_TRIG_H, 0x00);
		data_Send(FPGA_POS_TRIG_M1,0x00);
		data_Send(FPGA_POS_TRIG_M2,0x00);
		data_Send(FPGA_POS_TRIG_M3,0x00);
		data_Send(FPGA_POS_TRIG_L, 0x00);
	}
}
/************************************/
/*函数功能：设置采样率				*/
/*									*/
/************************************/
void hw_Setsample_Master(void)
{
	int temp = 0;
	struct XSystem xsystem = gXSystem[CHANNEL_ID_PRIMARY];

	//改变采样率
	if (xsystem.Sensitivity < LEV5US)//250MHz 采样率
      data_Send(ADC_SCLK_SEL, 0);			
	else//25MHz 采样率
		data_Send(ADC_SCLK_SEL,1);         	
	
	temp =  xsystem.H_samples - 1;
	data_Send(FPGA_TIME_BASE_H,(uint8_t)(temp>>24));//硬件抽点，对应表/
	data_Send(FPGA_TIME_BASE_M2,(uint8_t)(temp>>16));
	data_Send(FPGA_TIME_BASE_M1,(uint8_t)(temp>>8));
	data_Send(FPGA_TIME_BASE_L,(uint8_t) temp);
		
	//峰值检测
	if ( peakCheck == PKCHECK_ON)
	{
		if (xsystem.Sensitivity >= PEAK_MIN_SCALE)
			data_Send(FPGA_PEAK_MODE,1);			
		else
			data_Send(FPGA_PEAK_MODE,0);
	}
	else
		data_Send(FPGA_PEAK_MODE,0);

	data_Send(FPGA_COUNT_CLR,0);
	delay_us(50);
	data_Send(FPGA_COUNT_CLR,1);
}
/************************************/
/*函数功能：硬件操作函数转移表		*/
/*									*/
/************************************/
void (* _TaskDecomposeDefines[][12])(void)=
{
	{hw_Ysys_baseline,},								//HWCMDID_YSYS_BASELINE 
	{hw_Ysys_gain,},									//HWCMDID_YSYS_GAIN
	{0,},												//HWCMDID_YSYS_INVERT
	{0,},												//HWCMDID_YSYS_CH_STATUS
	{hw_hc4094ctrl,hw_Ysys_baseline,hw_Ysys_gain},		//HWCMDID_YSYS_ALL
		
	{hw_setTriMode,}, 									//HWCMDID_TSYS_MODE			
	{hw_setTriType,hw_setTrivolt}, 						//HWCMDID_TSYS_TYPE			
	{hw_setTriSource, hw_hc4094ctrl,hw_Ysys_gain},		//HWCMDID_TSYS_SOURCE			
	{hw_hc4094ctrl,hw_setTrivolt},						//HWCMDID_TSYS_COUPLING	
	{hw_setTriholdoff,},								//HWCMDID_TSYS_HOLDOFF
	{hw_setTriEdge,hw_setTrivolt},						//HWCMDID_TSYS_EDGE
	{hw_setTriPw},										//HWCMDID_TSYS_PW
	{0,},												//HWCMDID_TSYS_FORCE
	{hw_setTrivolt,},									//HWCMDID_TSYS_VOLT
	{hw_setTriMode, hw_setTriType, hw_setTriSource, hw_hc4094ctrl, hw_setTriEdge, hw_setTriPw},//HWCMDID_TSYS_ALL
	
	{hw_xsys_deepMaster,},								//HWCMDID_XSYS_DEEP_MASTER
	{hw_Setsample_Master,hw_xsys_deepMaster},			//HWCMDID_XSYS_SAMPLE_MASTER
	{0,}, 												//HWCMDID_XSYS_SAMPLE_SLAVE以下几个都是视窗模式的几个操作
	{0,},												//HWCMDID_XSYS_DEEP_SLAVE
	{0,},												//HWCMDID_SYS_ALL
};
/************************************/
/*函数功能：任务压栈操作			*/
/*									*/
/************************************/
#define HW_CMD_MAXCOUNT	20 //表明任务总数
unsigned short _TaskQueue[HW_CMD_MAXCOUNT+1];
unsigned short _TaskQueueLength=0;

void hw_PushCommand(unsigned short TaskID)
{
	int i;
	short exist= 0;
	
	if (TaskID==HWCMDID_SYS_ALL)
	{
		_TaskQueueLength=0;
		_TaskQueue[_TaskQueueLength++]=TaskID;
		return;
	}
	else if (_TaskQueueLength>0)
	{
		if (_TaskQueue[0]==HWCMDID_SYS_ALL)
			return;
	}	
	for(i=0; i<_TaskQueueLength; i++)//判断任务是否在任务队列中已经存在
	{
		if (_TaskQueue[i]==TaskID)
		{
			exist=1;
			break;
		}
	}
	if (exist == 1)
		return;
	_TaskQueue[_TaskQueueLength++]=TaskID;
	if (_TaskQueueLength==HW_CMD_MAXCOUNT)//表明程序出现错误
	{
		printf("critical Error!!!!!!!!!!!!!!!\r\n");
		_TaskQueueLength--;
	}
}
/************************************/
/*函数功能：执行任务操作		*/
/*						*/
/************************************/

void hw_ExpandSysAll(void)
{
	int TaskID;
	_TaskQueueLength=0;
	for (TaskID=0; TaskID<HWCMDID_SYS_ALL; TaskID++)//将所有任务压入任务队列
	{
		if (_TaskDecomposeDefines[TaskID][0])
		{
			_TaskQueue[_TaskQueueLength++]=TaskID;
		}
	}
}

void hw_ExecuteCommand(void)
{
	void (*ExecuteQueue[HW_CMD_MAXCOUNT*2])(void) = {0};	
	int QueueIndex,taskID,i,j;
	int ExecQueueLength=0;
	short found = 0;  
	
	DATA_GPIO_config(WR_FLAG);
	if (_TaskQueueLength !=0)
	{
		if ((_TaskQueueLength==1) && (_TaskQueue[0]==HWCMDID_SYS_ALL))//执行所有的操作
		{
			hw_ExpandSysAll();
		}	
		for(QueueIndex=0; QueueIndex<_TaskQueueLength; QueueIndex++) //任务队列出队列操作
		{
			i=0;
			taskID=_TaskQueue[QueueIndex];
			while(_TaskDecomposeDefines[taskID][i])
			{
				found=0;
				for(j=0; j<ExecQueueLength; j++) //判断该任务是否已经进入执行队列，这个操作可以避免重复执行某个动作
				{
					if (ExecuteQueue[j]==_TaskDecomposeDefines[taskID][i])
					{
						found=1;
						break;
					}
				}
				if ( found == 0)
				{
					ExecuteQueue[ExecQueueLength++]=_TaskDecomposeDefines[taskID][i];
				}
				i++;
			}
		}
		mxchipTick();		
		for(i=0;i<ExecQueueLength;i++) //开始执行任务
		(*ExecuteQueue[i])();
		_TaskQueueLength=0;
	}	
}
