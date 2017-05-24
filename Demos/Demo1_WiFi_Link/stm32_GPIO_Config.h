/************************************************************
*															*
*			AVEO Technology Corp.							*
*		Copyright (c) 2012.All rights Reserved				*
*															*
*	Project		:   stm32 BootCode					        *
*	Device		:   stm32 All Types						    *
*	File		:   stm32_SPI1_RxData.h					    *
*	Contents	:   Main FrameWork							*
*	Written by	:   Paul zhang								*
*	Date		:   2012-10-30								*
*	Release Ver	:   Version 001.000							*
*															*
*************************************************************/

// SPI Data I/O  define
#define WR_FLAG			1
#define RD_FLAG			0

//#define SPIDATA_CS_PIN		 			GPIO_Pin_12
//#define SPIDATA_CS_GPIO_PORT				GPIOA

#define ADDR_PORT						GPIOA
#define FIFO_FULL       GPIO_Pin_1
#define S6_SET          GPIO_Pin_10


#define DATA_WR	 					GPIO_Pin_2
#define FPGA_EN						GPIO_Pin_3
#define DATA_RD		 				GPIO_Pin_6
#define DATA_WR_PORT					GPIOC
#define DATA_RD_PORT					GPIOC


#define DATA_PORT						GPIOB
#define DATA0 						GPIO_Pin_3
#define DATA1 						GPIO_Pin_4
#define DATA2 						GPIO_Pin_5
#define DATA3 						GPIO_Pin_6
#define DATA4 						GPIO_Pin_7
#define DATA5 						GPIO_Pin_8
#define DATA6 						GPIO_Pin_14
#define DATA7 						GPIO_Pin_15
#define CH_EN 						GPIO_Pin_11

#define ADC_PORT		GPIOC
#define ADC_PIN   	GPIO_Pin_3
#define ADC_PIN1   	GPIO_Pin_4	
#define LEDCONTROL					GPIO_Pin_7
#define LEDCONTROL_PORT					GPIOC

#define CheckPin					GPIO_Pin_12
#define CheckPin_PORT					GPIOA

#define SPIDATA_Rx_DMA_FLAG				DMA_FLAG_TCIF0
#define SPIDATA_Rx_DMA_IT				DMA_IT_TC

#define SPIDATA_Rx_DMA_STREAM				DMA2_Stream0
#define SPIDATA_Rx_DMA_Channel			DMA_Channel_3
#define SPIDATA_Tx_DMA_STREAM				DMA2_Stream5
#define SPIDATA_Tx_DMA_Channel      		DMA_Channel_3

#define SPI_SLAVE_DR_Base             	0x4001300C


void TimeConfig(void);
void ADCLED_GPIO_config(void);
void ADC_config(void);
void PWM_config(void);
void setPuldutycyc(uint32_t period);

void Control_GPIO_config(void);
void DATA_GPIO_config(char flag);
void Check_GPIO_config(void);

