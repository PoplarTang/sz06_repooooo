#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include "Config.h"

// TRIG �������
#define TRIG	P47
// ECHO �������
#define ECHO	P33

#define ULTRA_GPIO_INIT()	        \
	/* TRIG������� */              \
	P4_MODE_OUT_PP(GPIO_Pin_7);     \
	/* ECHO�������� */              \
	P3_MODE_IN_HIZ(GPIO_Pin_3);     


void Ultrasonic_init();

/***
��ȡ��ǰ����������

distance���ڳɹ�ʱ�����Զ����Ϊ���ݣ���λcm
����ֵ��0�ɹ���-1ʧ��
*/
char Ultrasonic_get_distance(float * distance);

#endif