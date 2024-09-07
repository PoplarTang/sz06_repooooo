#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "Config.h"

#define LQ_F	P14
#define LQ_B	P15

typedef struct {
  // -100 -------------- 0 --------------- 100
	float LQ_speed;	// 左前
	float RQ_speed; // 右前
	float LH_speed; // 左后
	float RH_speed; // 右后
	
} MotorCfg; //Cfg->Config

void Motors_init();

// 前进[0, 100]
void Motors_forward(float speed);

// 后退[0, 100]
void Motors_backward(float speed);

// 左平移
void Motors_left(float speed, char dir);

// 右平移
void Motors_right(float speed, char dir);

// 顺逆时针（原地调头）
// dir:0左掉头（逆时针），1右掉头（顺时针）
void Motors_around(float speed, char dir);

void Motors_move(char x, char y);
// 停车
void Motors_stop();

#endif