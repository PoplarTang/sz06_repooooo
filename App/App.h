#ifndef __APP_H__
#define __APP_H__

#include "Config.h"

#define TASK_KEY			    1
#define TASK_TRACK		    2
#define TASK_UART1		    3
#define TASK_BLUETOOTH		4
#define TASK_ULTRASONIC		5
#define TASK_BATTERY		  6


// ------------------------------------- �������ģ��
extern u8 is_ultrasonic_enable;
extern float global_distance_cm;

// ------------------------------------- Ѳ��ģ��
extern u8 is_tracking; //0�ر�Ѳ�� 1����Ѳ��

// -------------------------------------- �������ģ��
extern float voltage;
// �ж��Ƿ��ǵ͵���
extern u8 isLowPower();

#endif