#include "Motors.h"
#include "GPIO.h"
#include "STC8H_PWM.h"
#include "NVIC.h"
#include "Switch.h"

static void GPIO_config(void) {
  // Motor ������� LQ
  P1_MODE_OUT_PP(GPIO_Pin_4 | GPIO_Pin_5);
  // Motor ������� RQ
  P1_MODE_OUT_PP(GPIO_Pin_6 | GPIO_Pin_7);

  // Motor ������� LH
  P2_MODE_OUT_PP(GPIO_Pin_0 | GPIO_Pin_1);
  // Motor ������� RH
  P2_MODE_OUT_PP(GPIO_Pin_2 | GPIO_Pin_3);
}


void Motors_init() {
  EAXSFR();
  GPIO_config();
}

#define PERIOD (MAIN_Fosc / 1000)

PWMx_Duty dutyA;
/*****
����Χ��[-100, 100]�ٶ�ֵת�ɶ�Ӧ��ռ�ձ�ֵ[0,100]
speed: -100 -------------- 0 --------------- 100
duty:   0   -------------- 50 -------------- 100
*****/
float speed2duty(float speed){
	// [-100, 100] / 2      ==> [-50, 50]
	// [-100, 100] / 2 + 50 ==> [0, 100]
	return speed / 2 + 50;
}

// ����|�޸�ÿ�����ӵ�ռ�ձȣ�����ѡ��
static void	PWM_config(MotorCfg cfg)
{
	float LQ_duty = speed2duty(cfg.LQ_speed);
	float RQ_duty = speed2duty(cfg.RQ_speed);
	float LH_duty = speed2duty(cfg.LH_speed);
	float RH_duty = speed2duty(cfg.RH_speed);
	
	u8 is_LQ_enable = (cfg.LQ_speed != 0); // �ٶȲ���0������Ҫѡ���Ӧ������
	u8 is_RQ_enable = (cfg.RQ_speed != 0); // �ٶȲ���0������Ҫѡ���Ӧ������
	u8 is_LH_enable = (cfg.LH_speed != 0); // �ٶȲ���0������Ҫѡ���Ӧ������
	u8 is_RH_enable = (cfg.RH_speed != 0); // �ٶȲ���0������Ҫѡ���Ӧ������
	
  PWMx_InitDefine		PWMx_InitStructure;
  // ����PWM1 LH ���
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * LH_duty / 100.0f;	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect	= is_LH_enable ? (ENO1P | ENO1N) : 0;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM1, &PWMx_InitStructure);			//��ʼ��PWM
  // ����PWM2 RH �Һ�
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE2;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * RH_duty / 100.0f;	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_RH_enable ? (ENO2P | ENO2N) : 0;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM2, &PWMx_InitStructure);			//��ʼ��PWM
  // ����PWM3 LQ ��ǰ
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * LQ_duty / 100.0f;	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_LQ_enable ? (ENO3P | ENO3N) : 0;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM3, &PWMx_InitStructure);
  // ����PWM4 RQ ��ǰ
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE2;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * RQ_duty / 100.0f;	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_RQ_enable ? (ENO4P | ENO4N) : 0;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM4, &PWMx_InitStructure);

  // ����PWMA
  PWMx_InitStructure.PWM_Period   = PERIOD - 1;			//����ʱ��,   0~65535
  PWMx_InitStructure.PWM_DeadTime = 0;							//��������������, 0~255
  PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//�����ʹ��, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//ʹ�ܼ�����, ENABLE,DISABLE
  PWM_Configuration(PWMA, &PWMx_InitStructure);			//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

  // �л�PWMͨ��
  PWM1_SW(PWM1_SW_P20_P21);			//PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
  PWM2_SW(PWM2_SW_P22_P23);			//PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
  PWM3_SW(PWM3_SW_P14_P15);			//PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
  PWM4_SW(PWM4_SW_P16_P17);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

  // ��ʼ��PWMA���ж�
  NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
}
void Motors_forward(float speed) {
	MotorCfg cfg;
	cfg.LQ_speed = speed;
	cfg.RQ_speed = speed;
	cfg.LH_speed = speed;
	cfg.RH_speed = speed;
	PWM_config(cfg);
}

void Motors_backward(float speed) {
	MotorCfg cfg;
	cfg.LQ_speed = -speed;
	cfg.RQ_speed = -speed;
	cfg.LH_speed = -speed;
	cfg.RH_speed = -speed;
	PWM_config(cfg);
}

// 0�� 1��ǰ��-1���
void Motors_left(float speed, char dir){
	MotorCfg cfg = {0, 0, 0, 0};
	
	if(dir == 1 || dir == 0){
		cfg.RQ_speed = speed;
		cfg.LH_speed = speed;
	}
	
	if(dir == -1 || dir == 0){
		cfg.LQ_speed = -speed;
		cfg.RH_speed = -speed;
	}
	
	PWM_config(cfg);
}

// 0�ң� 1��ǰ��-1�Һ�
void Motors_right(float speed, char dir){
	MotorCfg cfg = {0, 0, 0, 0};
	
	if(dir == 1 || dir == 0){
		cfg.LQ_speed = speed;
		cfg.RH_speed = speed;		
	}
	
	if(dir == -1 || dir == 0){
		cfg.RQ_speed = -speed;
		cfg.LH_speed = -speed;		
	}
	
	PWM_config(cfg);
}

// ˳��ʱ�루ԭ�ص�ͷ��
// dir:1�ҵ�ͷ��˳ʱ�룩,0���ͷ����ʱ�룩
void Motors_around(float speed, char dir){
	MotorCfg cfg;
	if(dir == 1){ // 1�ҵ�ͷ��˳ʱ�룩
		cfg.LQ_speed = speed;
		cfg.RQ_speed = -speed;
		cfg.LH_speed = speed;
		cfg.RH_speed = -speed;
	}else {		// 0���ͷ����ʱ�룩
		cfg.LQ_speed = -speed;
		cfg.RQ_speed = speed;
		cfg.LH_speed = -speed;
		cfg.RH_speed = speed;
	}
	
	PWM_config(cfg);
	
}

float clip(float value, float min, float max){
	if(value < min){
		return min;
	}else if(value > max){
		return max;
	}
	return value;
}

/************************

����x, y�˶�
��		��Y+  ��
     
           
��	 	0 		��
X-					X+
		
��		��Y-	��

X    -100 ------- 0 ------- 100
Y		 -100 ------- 0 ------- 100
*************************/

float max_ratio = 0.6f;
void Motors_move(char x, char y){
	MotorCfg cfg = {0, 0, 0, 0};
	// ��ƽ�� y = 0, x = 100
	// ��ƽ�� y = 0, x = -100
	
	cfg.LQ_speed = y + x;
	cfg.RQ_speed = y - x;
	cfg.LH_speed = y - x; 
	cfg.RH_speed = y + x;
	
	// clip ǯλ
	cfg.LQ_speed = max_ratio * clip(cfg.LQ_speed, -100, 100);
	cfg.RQ_speed = max_ratio * clip(cfg.RQ_speed, -100, 100);
	cfg.LH_speed = max_ratio * clip(cfg.LH_speed, -100, 100);
	cfg.RH_speed = max_ratio * clip(cfg.RH_speed, -100, 100);
	
	PWM_config(cfg);
	
	
}

void Motors_stop() {
	MotorCfg cfg;
	cfg.LQ_speed = 0;
	cfg.RQ_speed = 0;
	cfg.LH_speed = 0;
	cfg.RH_speed = 0;
	PWM_config(cfg);
}