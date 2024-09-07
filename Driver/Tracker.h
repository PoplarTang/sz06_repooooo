#ifndef __TRACKER_H__
#define __TRACKER_H__

#include "Config.h"

#define TRACK_0		P00
#define TRACK_1		P01
#define TRACK_2		P02
#define TRACK_3		P03
#define TRACK_4		P04


#define TRACKER_GPIO_INIT() P0_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4)

void Tracker_init();

// 扫描所有灯状态, 把结果放到states里
void Tracker_scan(u8 states[]);

// 读取线位置
// [-64, -32, 0, 32, 64]
int Tracker_get_position();


#endif