#ifndef _CHL_H
#define _CHL_H


#define CALI_DATA_NUM	128   //64
#define CB_CH1			Calibrate[0]	
#define CB_CH2			Calibrate[22]
#define CB_CH_FIRST		0
#define CB_CH_LAST		10
#define CB_CH_LENS		11

#define CB_INDEX_TRIG				44	
#define CB_INDEX_TRIG_CUR			CB_INDEX_TRIG		//44
#define CB_INDEX_TRIG_UNKNOWN		(CB_INDEX_TRIG+1)	//45
#define CB_INDEX_TRIG_DC1			(CB_INDEX_TRIG+2)	//46
#define CB_INDEX_TRIG_DC2			(CB_INDEX_TRIG+3)	//47
#define CB_INDEX_TRIG_AC			(CB_INDEX_TRIG+4)	//48

#define CB_VALUE_TRIG_CUR		Calibrate[CB_INDEX_TRIG]		
#define CB_VALUE_TRIG_UNKNOWN	Calibrate[CB_INDEX_TRIG_UNKNOWN]
#define CB_VALUE_TRIG_DC(X)		Calibrate[CB_INDEX_TRIG_DC1+(X)]	
#define CB_VALUE_TRIG_DC1		Calibrate[CB_INDEX_TRIG_DC1]
#define CB_VALUE_TRIG_DC2		Calibrate[CB_INDEX_TRIG_DC2]
#define CB_VALUE_TRIG_AC		Calibrate[CB_INDEX_TRIG_AC]

#define CB_INDEX_PRE_OFS		61
#define CB_VALUE_PRE_OFS_CH1	Calibrate[CB_INDEX_PRE_OFS+0]
#define CB_VALUE_PRE_OFS_CH2	Calibrate[CB_INDEX_PRE_OFS+1]
#define CB_VALUE_PRE_OFS(X)		Calibrate[CB_INDEX_PRE_OFS+X]
#define CB_INDEX_RANDOM 63

#define CB_INDEX_BASEFACTOR		64 //������DAֵ
#define CB_INDEX_TRIG_LEVEL     86 //������ƽУ��ֵ

#define DA_CH1_OFFSET		7	//Ch1 ǰ��ƫ��
#define DA_CH1_BASE			4	//Ch1 ��ƫ��
#define DA_CH1_GAIN			6	//Ch1 ����

#define DA_CH2_OFFSET		5	//Ch2 ǰ��ƫ��
#define DA_CH2_BASE			3	//Ch2 ��ƫ��
#define DA_CH2_GAIN			1	//Ch2 ����

#define DA_TG_LINE2			0	//������ƽ��CH2������ƽ
#define DA_TG_LINE1			2	//��������ƽ��CH1������ƽ

extern short Calibrate[CALI_DATA_NUM];


#endif
