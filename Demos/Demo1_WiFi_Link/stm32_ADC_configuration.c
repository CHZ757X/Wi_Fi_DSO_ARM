#include "stm32f2xx.h"
#include "stm32_ADC_configuration.h"
#include "stm32f2xx_adc.h"
#include <stdio.h>

void GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	
	
	
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

void ADC_config(void)
{
	ADC_InitTypeDef ADC_Initstruct;
	ADC_CommonInitTypeDef ADC_Commont;
	
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
void PWM_config(void)
{
	TIM_TimeBaseInitTypeDef	tim_pwm_struct;
	TIM_OCInitTypeDef tim_pwm_ocstruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	tim_pwm_struct.TIM_ClockDivision = TIM_CKD_DIV1;
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
void setPuldutycyc(uint32_t period)
{
	TIM_SetCompare2(TIM8,40);	
}

