#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "Config.h"

#define GPIO_PORT		GPIO_P3
#define GPIO_PIN		GPIO_Pin_4

#define BUZZER_PIN	P34

// 初始化蜂鸣器
void Buzzer_init();

// 按照指定频率播放
void Buzzer_play(u16 hz_val);

void Buzzer_beep(u16 idx);

void Buzzer_alarm();

void Buzzer_alarm2();

void Buzzer_alarm3();

// 停止播放
void Buzzer_stop();


#endif