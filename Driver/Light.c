#include "Light.h"
#include "GPIO.h"


static void GPIO_config(u8 port, u8 pin) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	
	// LED init
	GPIO_InitStructure.Pin  = pin;		//ָ��Ҫ��ʼ����IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(port, &GPIO_InitStructure);//��ʼ��
}


void Light_init(){
	GPIO_config(GPIO_L);
	GPIO_config(GPIO_R);
	
	// Ĭ��ȫ��Ϩ��
	Light_off(LIGHT_ALL);
}

// ���ƣ����ң�ȫ��
void Light_on(Light light){
	if(light == LIGHT_ALL){
		LED_L = 1;
		LED_R = 1;
	}else if(light == LIGHT_LEFT){
		LED_L = 1;
	}else if(light == LIGHT_RIGHT){
		LED_R = 1;
	}
}

// �صƣ����ң�ȫ��
void Light_off(Light light){
	switch(light){
		case LIGHT_LEFT: 
			LED_L = 0;
			break;
		case LIGHT_RIGHT: 
			LED_R = 0;
			break;
		case LIGHT_ALL: 
			LED_L = 0;
			LED_R = 0;
			break;
		default:
			break;
	}
}