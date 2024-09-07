#ifndef __APP_H__
#define __APP_H__

#include "Config.h"

#define TASK_KEY			    1
#define TASK_TRACK		    2
#define TASK_UART1		    3
#define TASK_BLUETOOTH		4
#define TASK_ULTRASONIC		5
#define TASK_BATTERY		  6


// ------------------------------------- 超声测距模块
extern u8 is_ultrasonic_enable;
extern float global_distance_cm;

// ------------------------------------- 巡线模块
extern u8 is_tracking; //0关闭巡线 1开启巡线

// -------------------------------------- 电量检测模块
extern float voltage;
// 判断是否是低电量
extern u8 isLowPower();

#endif