#include "Battery.h"
#include "GPIO.h"
#include "ADC.h"
#include "NVIC.h"

static void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
    GPIO_InitStructure.Pin  = GPIO_Pin_3;		//ָ��Ҫ��ʼ����IO,
    GPIO_InitStructure.Mode = GPIO_HighZ;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//��ʼ��
}
/******************* AD���ú��� *******************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����

	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X1T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);				//��ʼ��
	ADC_PowerControl(ENABLE);								//ADC��Դ����, ENABLE��DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);			//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}
void Battery_init(){
	GPIO_config();
	ADC_config();
}

float Battery_get_voltage(){
	u16 result = Get_ADCResult(ADC_CH3);// chn: ADC_CH0, ADC_CH1, ADC_CH2 .... ADC_CH15

	// result / 4096 = vol / 2.5
	float Vadc = result * 2.5 / 4096;
	
	// Vcc / (51k�� + 10k��) = Vadc / 10k��
	return (Vadc * 6.1) * 1.005; 
	
}