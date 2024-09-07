#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Config.h"

#define GPIO_L	GPIO_P0, GPIO_Pin_7
#define GPIO_R	GPIO_P5, GPIO_Pin_2

#define LED_L		P07
#define LED_R		P52

typedef enum {

	LIGHT_LEFT,
	LIGHT_RIGHT,
	LIGHT_ALL,
	
} Light;

void Light_init();

// ���ƣ����ң�ȫ��
void Light_on(Light light);

// �صƣ����ң�ȫ��
void Light_off(Light light);

#endif