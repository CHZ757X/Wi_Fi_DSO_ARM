#ifndef FPGAREGISTER_H
#define FPGAREGISTER_H

//对fifo的控制
#define FIFO_REST_CTRL		0x00 
#define CH_HC4094_START		0x01
#define CH_AD5628_START		0x02
//对通道HC4094的控制
#define HC4094_CTRL_DATA_L	0x05	//发送HC控制字低8位所对应的地址
#define HC4094_CTRL_DATA_H	0x04	//发送HC控制字高8位所对应的地址

//可变增益调整电压Vgain 位移电压Voffset
#define DAC5628_CONFIG_H      0x06
#define DAC5628_CONFIG_M1     0x07
#define DAC5628_CONFIG_M2	  0x08
#define DAC5628_CONFIG_L	  0x09
//触发系统 边沿
#define FPGA_TRIG_SOURCE	0x0A	//00:CH1  01:CH2		
#define FPGA_TRIG_MODE		0x14	//00:自动 01:正常 02:单次
#define FPGA_TRIG_TYPE		0x0B	//00:边沿 01:脉宽
#define FPGA_TRIG_EDGE		0x11	//斜率类型, 脉宽极性
#define FPGA_TRIG_FLAG			//触发标志
#define TRI_LEVEL_STATUS  		//触发电平状态 bit0:Ready; bit1:TimeOver;bit2:Trig'ed
	
//触发系统 脉宽
#define FPGA_PW_COND_CTRL	0x0C				//脉宽条件
#define FPGA_PW_DATA_H		0x0D			
#define FPGA_PW_DATA_M1		0x0E
#define FPGA_PW_DATA_M2		0x0F			
#define FPGA_PW_DATA_L		0x10		
#define FPGA_FORCE_TRIG		0x13   	

//触发系统  预触发深度	(20bit)	
#define FPGA_PRE_TRIG_H		0x16
#define FPGA_PRE_TRIG_M		0x17
#define FPGA_PRE_TRIG_L		0x18
//触发系统	延迟触发深度(37bit)		
#define FPGA_POS_TRIG_H		0x19	
#define FPGA_POS_TRIG_M1	0x1A
#define FPGA_POS_TRIG_M2	0x1B
#define FPGA_POS_TRIG_M3	0x1C
#define FPGA_POS_TRIG_L		0x1D
//触发系统	触发释抑(32bit)
#define FPGA_HOLDOFF_H		0x25
#define FPGA_HOLDOFF_M1		0x26
#define FPGA_HOLDOFF_M2		0x27
#define FPGA_HOLDOFF_L		0x28

//x System
//DCM复位
#define DCM_RST			0x1E
//AD的时钟选择
//Scan时=25M,实时时=250M,随机=200M
//  00    250M   
//  01    25M    
//  10    200M
//  11    20M (暂时未用)  
#define ADC_SCLK_SEL		0x1F
//分频选择
#define DATA_FLOW_DIV		0x20
//AD控制
#define ADC_DS_CTRL	      0x24
//选择采样方式
#define REAL_OR_RANDOM			0x12
//时基值，采集前抽点间隔，即控制FIFO写使能分频数
#define FPGA_TIME_BASE_L		0x33
#define FPGA_TIME_BASE_M1		0x32
#define FPGA_TIME_BASE_M2		0x31
#define FPGA_TIME_BASE_H		0x30
//峰值模式控制：
#define FPGA_PEAK_MODE			0x26
//采集系统计数器复位：	
#define FPGA_COUNT_CLR			0x27		//0：复位


//波形反向：	
#define FPGA_INV_WAVE			0x31		//1：反向,bit0:CH1,bit1:CH2.
//通道选择：			
#define FPGA_CHANNEL_NUM		0x32		//1：通道1	2：通道2	3：双通道

//FPGA内部锁相环复位，1：复位；0：锁相环工作
#define FPGA_PLL_RST			0x33

#define FPGA_EXP_PULSE_SEL		0x34
////FIFO控制FIFO_RW_CTRL(读写控制)
//bit 15: fifo1 读使能
//bit 8:  fifo2  读使能
//bit 3: 读时钟选择,1:读FIFO，0:读预触发
//bit 2: 写使能 
#define FPGA_FIFO_RW_CTR_H		0x21
#define FPGA_FIFO_RW_CTR_L		0x22

//fifo复位 1：复位fifo 0：fifo正常工作
#define FPGA_FIFO_RESET		0x23

//FIFO满，bit4：FIFO空；bit3：通道A的FIFO满；bit2：通道B的FIFO满；bit1：通道C的FIFO满；bit0：通道D的FIFO满
#define DMM_RX_OVER		
//bit 3:通道2满标志
//bit 2:通道2空标志
//bit 1:通道1满标志
//bit 0:通道1空标志
#define FPGA_FIFO_FLAG		

#define FPGA_FIFO_DAL 		
#define FPGA_FIFO_DAH		
#define FPGA_FIFO_DBL 		
#define FPGA_FIFO_DBH		
//控制硬件抽点
//#define FPGA_DATAFLOW_MODE	0x25		//0:	1:
	
#endif

