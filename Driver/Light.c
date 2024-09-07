#include "Light.h"
#include "GPIO.h"


static void GPIO_config(u8 port, u8 pin) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	
	// LED init
	GPIO_InitStructure.Pin  = pin;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(port, &GPIO_InitStructure);//初始化
}


void Light_init(){
	GPIO_config(GPIO_L);
	GPIO_config(GPIO_R);
	
	// 默认全部熄灭
	Light_off(LIGHT_ALL);
}

// 开灯：左，右，全部
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

// 关灯：左，右，全部
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