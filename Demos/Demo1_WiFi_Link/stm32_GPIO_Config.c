/************************************************************
*															*
*							*
*		Copyright (c) 2015.All rights Reserved				*
*															*
*	Project		:   WIFILINK					        *
*	Device		:   EMW3162						    *
*	File		:   stm32_GPIO_config.c					    *
*	Contents	:   Main FrameWork							*
*	Written by	:   haozhe chang								*
*	Date		:   2015-11-25								*
*	Release Ver	:   Version 001.000							*
*															*
*************************************************************/
#include "stm32f2xx.h"
#include "stm32_GPIO_Config.h"
#include <stdio.h>
/************************************/
/*函数功能：数据总线GPIO口的配置	*/
/*						*/
/************************************/
void TimeConfig(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);			
}
/************************************/
/*函数功能：数据总线GPIO口的配置	*/
/*						*/
/************************************/

void DATA_GPIO_config(char flag)
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		
	if(flag == RD_FLAG)
	{
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	}
	else if(flag == WR_FLAG)
	{
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	}
	GPIO_InitStructure.GPIO_Pin   = DATA0|DATA1|DATA2|DATA3|DATA4|DATA5|DATA6|DATA7;
	GPIO_Init(DATA_PORT, &GPIO_InitStructure );
}
/************************************/
/*函数功能：控制总线GPIO口的配置	*/
/*						*/
/************************************/
void Control_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin   = DATA_WR|DATA_RD|FPGA_EN;
	GPIO_Init(GPIOC,  &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = CH_EN;
	GPIO_Init(GPIOB, &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure );
//	GPIO_InitStructure.GPIO_Pin   = S6_SET;
//	GPIO_Init(GPIOA, &GPIO_InitStructure );

		
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin   = FIFO_FULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure );

	GPIOB->BSRRL = CH_EN;
	GPIOC->BSRRL = FPGA_EN;//1
//	GPIOA->BSRRL = S6_SET;//1	
}

/************************************/
/*函数功能：观测GPIO口的配置		*/
/*						*/
/************************************/
void Check_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= CheckPin;
	GPIO_Init(CheckPin_PORT,  &GPIO_InitStructure );
	CheckPin_PORT->BSRRH = CheckPin;
}
/************************************/
/*函数功能:ADC&LED GPIO口的配置	*/
/*						*/
/************************************/
void ADCLED_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
			
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;	
	GPIO_InitStructure.GPIO_Pin   = ADC_PIN1;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure );
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin   = LEDCONTROL;
	GPIO_Init(LEDCONTROL_PORT, &GPIO_InitStructure );
	GPIO_PinAFConfig(LEDCONTROL_PORT, GPIO_PinSource7,  GPIO_AF_TIM8);
}
/************************************/
/*函数功能:ADC功能配置			*/
/*						*/
/************************************/
void ADC_config(void)
{
	ADC_InitTypeDef ADC_Initstruct;
	ADC_CommonInitTypeDef ADC_Commont;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);		
	ADC_Commont.ADC_Mode = ADC_Mode_Independent;			
	ADC_Commont.ADC_Prescaler = ADC_Prescaler_Div6;
	ADC_Commont.ADC_DMAAccessMode =ADC_DMAAccessMode_Disabled;
	ADC_Commont.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	
	ADC_Initstruct.ADC_Resolution = ADC_Resolution_8b;
	ADC_Initstruct.ADC_ScanConvMode = DISABLE;
	ADC_Initstruct.ADC_ContinuousConvMode = DISABLE;
	ADC_Initstruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_Initstruct.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_Initstruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Initstruct.ADC_NbrOfConversion = 1;
	
	ADC_CommonInit(&ADC_Commont);
	ADC_Init(ADC1, &ADC_Initstruct);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_15Cycles);	
	ADC_Cmd(ADC1, ENABLE);
}
/************************************/
/*函数功能:LED端口（pwm配置）		*/
/*						*/
/************************************/
void PWM_config(void)
{
	TIM_TimeBaseInitTypeDef	tim_pwm_struct;
	TIM_OCInitTypeDef tim_pwm_ocstruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	tim_pwm_struct.TIM_ClockDivision = TIM_CKD_DIV1;//120M
	tim_pwm_struct.TIM_CounterMode = TIM_CounterMode_Down;
	tim_pwm_struct.TIM_Period = 120;
	tim_pwm_struct.TIM_Prescaler = 0;
	tim_pwm_struct.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM8,&tim_pwm_struct);
	tim_pwm_ocstruct.TIM_OCMode = TIM_OCMode_PWM2;
	tim_pwm_ocstruct.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_pwm_ocstruct.TIM_OCNPolarity = TIM_OCPolarity_Low;
	tim_pwm_ocstruct.TIM_OutputState =TIM_OutputState_Enable;
	tim_pwm_ocstruct.TIM_OutputNState = TIM_OutputNState_Enable;
	tim_pwm_ocstruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	tim_pwm_ocstruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	tim_pwm_ocstruct.TIM_Pulse = 60;
	TIM_OC2Init(TIM8, &tim_pwm_ocstruct);
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM8,ENABLE);
	TIM_Cmd(TIM8,ENABLE);	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
}
/************************************/
/*函数功能:设置占空比			*/
/*						*/
/************************************/
void setPuldutycyc(uint32_t period)
{
	TIM_SetCompare2(TIM8,40);	
}
