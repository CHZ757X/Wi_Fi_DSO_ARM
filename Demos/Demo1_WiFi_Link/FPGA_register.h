#ifndef FPGAREGISTER_H
#define FPGAREGISTER_H

//��fifo�Ŀ���
#define FIFO_REST_CTRL		0x00 
#define CH_HC4094_START		0x01
#define CH_AD5628_START		0x02
//��ͨ��HC4094�Ŀ���
#define HC4094_CTRL_DATA_L	0x05	//����HC�����ֵ�8λ����Ӧ�ĵ�ַ
#define HC4094_CTRL_DATA_H	0x04	//����HC�����ָ�8λ����Ӧ�ĵ�ַ

//�ɱ����������ѹVgain λ�Ƶ�ѹVoffset
#define DAC5628_CONFIG_H      0x06
#define DAC5628_CONFIG_M1     0x07
#define DAC5628_CONFIG_M2	  0x08
#define DAC5628_CONFIG_L	  0x09
//����ϵͳ ����
#define FPGA_TRIG_SOURCE	0x0A	//00:CH1  01:CH2		
#define FPGA_TRIG_MODE		0x14	//00:�Զ� 01:���� 02:����
#define FPGA_TRIG_TYPE		0x0B	//00:���� 01:����
#define FPGA_TRIG_EDGE		0x11	//б������, ������
#define FPGA_TRIG_FLAG			//������־
#define TRI_LEVEL_STATUS  		//������ƽ״̬ bit0:Ready; bit1:TimeOver;bit2:Trig'ed
	
//����ϵͳ ����
#define FPGA_PW_COND_CTRL	0x0C				//��������
#define FPGA_PW_DATA_H		0x0D			
#define FPGA_PW_DATA_M1		0x0E
#define FPGA_PW_DATA_M2		0x0F			
#define FPGA_PW_DATA_L		0x10		
#define FPGA_FORCE_TRIG		0x13   	

//����ϵͳ  Ԥ�������	(20bit)	
#define FPGA_PRE_TRIG_H		0x16
#define FPGA_PRE_TRIG_M		0x17
#define FPGA_PRE_TRIG_L		0x18
//����ϵͳ	�ӳٴ������(37bit)		
#define FPGA_POS_TRIG_H		0x19	
#define FPGA_POS_TRIG_M1	0x1A
#define FPGA_POS_TRIG_M2	0x1B
#define FPGA_POS_TRIG_M3	0x1C
#define FPGA_POS_TRIG_L		0x1D
//����ϵͳ	��������(32bit)
#define FPGA_HOLDOFF_H		0x25
#define FPGA_HOLDOFF_M1		0x26
#define FPGA_HOLDOFF_M2		0x27
#define FPGA_HOLDOFF_L		0x28

//x System
//DCM��λ
#define DCM_RST			0x1E
//AD��ʱ��ѡ��
//Scanʱ=25M,ʵʱʱ=250M,���=200M
//  00    250M   
//  01    25M    
//  10    200M
//  11    20M (��ʱδ��)  
#define ADC_SCLK_SEL		0x1F
//��Ƶѡ��
#define DATA_FLOW_DIV		0x20
//AD����
#define ADC_DS_CTRL	      0x24
//ѡ�������ʽ
#define REAL_OR_RANDOM			0x12
//ʱ��ֵ���ɼ�ǰ�������������FIFOдʹ�ܷ�Ƶ��
#define FPGA_TIME_BASE_L		0x33
#define FPGA_TIME_BASE_M1		0x32
#define FPGA_TIME_BASE_M2		0x31
#define FPGA_TIME_BASE_H		0x30
//��ֵģʽ���ƣ�
#define FPGA_PEAK_MODE			0x26
//�ɼ�ϵͳ��������λ��	
#define FPGA_COUNT_CLR			0x27		//0����λ


//���η���	
#define FPGA_INV_WAVE			0x31		//1������,bit0:CH1,bit1:CH2.
//ͨ��ѡ��			
#define FPGA_CHANNEL_NUM		0x32		//1��ͨ��1	2��ͨ��2	3��˫ͨ��

//FPGA�ڲ����໷��λ��1����λ��0�����໷����
#define FPGA_PLL_RST			0x33

#define FPGA_EXP_PULSE_SEL		0x34
////FIFO����FIFO_RW_CTRL(��д����)
//bit 15: fifo1 ��ʹ��
//bit 8:  fifo2  ��ʹ��
//bit 3: ��ʱ��ѡ��,1:��FIFO��0:��Ԥ����
//bit 2: дʹ�� 
#define FPGA_FIFO_RW_CTR_H		0x21
#define FPGA_FIFO_RW_CTR_L		0x22

//fifo��λ 1����λfifo 0��fifo��������
#define FPGA_FIFO_RESET		0x23

//FIFO����bit4��FIFO�գ�bit3��ͨ��A��FIFO����bit2��ͨ��B��FIFO����bit1��ͨ��C��FIFO����bit0��ͨ��D��FIFO��
#define DMM_RX_OVER		
//bit 3:ͨ��2����־
//bit 2:ͨ��2�ձ�־
//bit 1:ͨ��1����־
//bit 0:ͨ��1�ձ�־
#define FPGA_FIFO_FLAG		

#define FPGA_FIFO_DAL 		
#define FPGA_FIFO_DAH		
#define FPGA_FIFO_DBL 		
#define FPGA_FIFO_DBH		
//����Ӳ�����
//#define FPGA_DATAFLOW_MODE	0x25		//0:	1:
	
#endif

