#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include "Config.h"

// TRIG 触发测距
#define TRIG	P47
// ECHO 输出回响
#define ECHO	P33

#define ULTRA_GPIO_INIT()	        \
	/* TRIG推挽输出 */              \
	P4_MODE_OUT_PP(GPIO_Pin_7);     \
	/* ECHO高阻输入 */              \
	P3_MODE_IN_HIZ(GPIO_Pin_3);     


void Ultrasonic_init();

/***
获取当前超声波距离

distance：在成功时，会自动填充为数据，单位cm
返回值：0成功，-1失败
*/
char Ultrasonic_get_distance(float * distance);

#endif