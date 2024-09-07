#include "Motors.h"
#include "GPIO.h"
#include "STC8H_PWM.h"
#include "NVIC.h"
#include "Switch.h"

static void GPIO_config(void) {
  // Motor 推挽输出 LQ
  P1_MODE_OUT_PP(GPIO_Pin_4 | GPIO_Pin_5);
  // Motor 推挽输出 RQ
  P1_MODE_OUT_PP(GPIO_Pin_6 | GPIO_Pin_7);

  // Motor 推挽输出 LH
  P2_MODE_OUT_PP(GPIO_Pin_0 | GPIO_Pin_1);
  // Motor 推挽输出 RH
  P2_MODE_OUT_PP(GPIO_Pin_2 | GPIO_Pin_3);
}


void Motors_init() {
  EAXSFR();
  GPIO_config();
}

#define PERIOD (MAIN_Fosc / 1000)

PWMx_Duty dutyA;
/*****
将范围在[-100, 100]速度值转成对应的占空比值[0,100]
speed: -100 -------------- 0 --------------- 100
duty:   0   -------------- 50 -------------- 100
*****/
float speed2duty(float speed){
	// [-100, 100] / 2      ==> [-50, 50]
	// [-100, 100] / 2 + 50 ==> [0, 100]
	return speed / 2 + 50;
}

// 设置|修改每个轮子的占空比，引脚选择
static void	PWM_config(MotorCfg cfg)
{
	float LQ_duty = speed2duty(cfg.LQ_speed);
	float RQ_duty = speed2duty(cfg.RQ_speed);
	float LH_duty = speed2duty(cfg.LH_speed);
	float RH_duty = speed2duty(cfg.RH_speed);
	
	u8 is_LQ_enable = (cfg.LQ_speed != 0); // 速度不是0，才需要选择对应的引脚
	u8 is_RQ_enable = (cfg.RQ_speed != 0); // 速度不是0，才需要选择对应的引脚
	u8 is_LH_enable = (cfg.LH_speed != 0); // 速度不是0，才需要选择对应的引脚
	u8 is_RH_enable = (cfg.RH_speed != 0); // 速度不是0，才需要选择对应的引脚
	
  PWMx_InitDefine		PWMx_InitStructure;
  // 配置PWM1 LH 左后
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * LH_duty / 100.0f;	//PWM占空比时间, 0~Period
  PWMx_InitStructure.PWM_EnoSelect	= is_LH_enable ? (ENO1P | ENO1N) : 0;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM1, &PWMx_InitStructure);			//初始化PWM
  // 配置PWM2 RH 右后
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE2;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * RH_duty / 100.0f;	//PWM占空比时间, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_RH_enable ? (ENO2P | ENO2N) : 0;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM2, &PWMx_InitStructure);			//初始化PWM
  // 配置PWM3 LQ 左前
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * LQ_duty / 100.0f;	//PWM占空比时间, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_LQ_enable ? (ENO3P | ENO3N) : 0;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM3, &PWMx_InitStructure);
  // 配置PWM4 RQ 右前
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE2;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * RQ_duty / 100.0f;	//PWM占空比时间, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_RQ_enable ? (ENO4P | ENO4N) : 0;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM4, &PWMx_InitStructure);

  // 配置PWMA
  PWMx_InitStructure.PWM_Period   = PERIOD - 1;			//周期时间,   0~65535
  PWMx_InitStructure.PWM_DeadTime = 0;							//死区发生器设置, 0~255
  PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
  PWM_Configuration(PWMA, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

  // 切换PWM通道
  PWM1_SW(PWM1_SW_P20_P21);			//PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
  PWM2_SW(PWM2_SW_P22_P23);			//PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
  PWM3_SW(PWM3_SW_P14_P15);			//PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
  PWM4_SW(PWM4_SW_P16_P17);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

  // 初始化PWMA的中断
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

// 0左， 1左前，-1左后
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

// 0右， 1右前，-1右后
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

// 顺逆时针（原地调头）
// dir:1右掉头（顺时针）,0左掉头（逆时针）
void Motors_around(float speed, char dir){
	MotorCfg cfg;
	if(dir == 1){ // 1右掉头（顺时针）
		cfg.LQ_speed = speed;
		cfg.RQ_speed = -speed;
		cfg.LH_speed = speed;
		cfg.RH_speed = -speed;
	}else {		// 0左掉头（逆时针）
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

根据x, y运动
┏		↑Y+  ┓
     
           
←	 	0 		→
X-					X+
		
┗		↓Y-	┛

X    -100 ------- 0 ------- 100
Y		 -100 ------- 0 ------- 100
*************************/

float max_ratio = 0.6f;
void Motors_move(char x, char y){
	MotorCfg cfg = {0, 0, 0, 0};
	// 右平移 y = 0, x = 100
	// 左平移 y = 0, x = -100
	
	cfg.LQ_speed = y + x;
	cfg.RQ_speed = y - x;
	cfg.LH_speed = y - x; 
	cfg.RH_speed = y + x;
	
	// clip 钳位
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