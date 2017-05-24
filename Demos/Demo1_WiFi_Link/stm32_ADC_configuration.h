#ifndef __ADC_CONFIG
#define __ADC_CONFIG

#define ADC_PORT		GPIOC
#define ADC_PIN   	GPIO_Pin_3
#define ADC_PIN1   	GPIO_Pin_4	
#define LEDCONTROL					GPIO_Pin_7
#define LEDCONTROL_PORT					GPIOC

void GPIO_config(void);
void ADC_config(void);
void PWM_config(void);
void setPuldutycyc(uint32_t period);

#endif
