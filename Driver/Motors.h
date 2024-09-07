#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "Config.h"

#define LQ_F	P14
#define LQ_B	P15

typedef struct {
  // -100 -------------- 0 --------------- 100
	float LQ_speed;	// ��ǰ
	float RQ_speed; // ��ǰ
	float LH_speed; // ���
	float RH_speed; // �Һ�
	
} MotorCfg; //Cfg->Config

void Motors_init();

// ǰ��[0, 100]
void Motors_forward(float speed);

// ����[0, 100]
void Motors_backward(float speed);

// ��ƽ��
void Motors_left(float speed, char dir);

// ��ƽ��
void Motors_right(float speed, char dir);

// ˳��ʱ�루ԭ�ص�ͷ��
// dir:0���ͷ����ʱ�룩��1�ҵ�ͷ��˳ʱ�룩
void Motors_around(float speed, char dir);

void Motors_move(char x, char y);
// ͣ��
void Motors_stop();

#endif