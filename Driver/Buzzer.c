#include "Buzzer.h"
#include "GPIO.h"
#include "STC8H_PWM.h"
#include "NVIC.h"
#include "Switch.h"

u16 code FREQS[] = {
	523 * 1, 587 * 1, 659 * 1, 698 * 1, 784 * 1, 880 * 1, 988 * 1, 
	523 * 2, 587 * 2, 659 * 2, 698 * 2, 784 * 2, 880 * 2, 988 * 2, 
	523 * 4, 587 * 4, 659 * 4, 698 * 4, 784 * 4, 880 * 4, 988 * 4, 
	523 * 8, 587 * 8, 659 * 8, 698 * 8, 784 * 8, 880 * 8, 988 * 8, 
};
//			 		 C`	   D`     E`   F`	   G`	   A`	   B`    C``
//u16 hz[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};

//			   		C	 	 D    E 	 F	  G	   A	  B	   C`
//u16 hz[] = {523, 587, 659, 698, 784, 880, 988, 1047};

static void GPIO_config(void) {
  GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
  GPIO_InitStructure.Pin  = GPIO_PIN;		//指定要初始化的IO,
  GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
  GPIO_Inilize(GPIO_PORT, &GPIO_InitStructure);//初始化
}
void	PWM_config(u16 hz_val)
{
  PWMx_InitDefine		PWMx_InitStructure;
	
	u16 period = (MAIN_Fosc / hz_val);	// 计算每个周期的计数值
	
  // 配置PWM5
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty   	 	= (u16)(period * 0.5);	//PWM占空比时间, 0~Period
  PWMx_InitStructure.PWM_EnoSelect    = ENO8P;			//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM8, &PWMx_InitStructure);			//初始化PWM,  PWMA,PWMB

  // 配置PWMB
  PWMx_InitStructure.PWM_Period   = period - 1;			//周期时间,   0~65535
  PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
  PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
  PWM_Configuration(PWMB, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

  // 切换PWM通道
  PWM8_SW(PWM8_SW_P34);					

  // 初始化PWMB的中断
  NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}
// 初始化蜂鸣器
void Buzzer_init(){
	// 扩展寄存器使能
	EAXSFR(); 
  GPIO_config();
	
	// 把引脚拉低，否则默认高电平，三极管导通发热
	BUZZER_PIN = 0;
}

// 按照指定频率播放
void Buzzer_play(u16 hz_val){
	PWM_config(hz_val);
}

// 根据索引取出对应的音调
void Buzzer_beep(u16 idx){
	u16 hz_value;
	if(idx == 0){	// 不发音
		Buzzer_stop();		
		return;
	}
	
	hz_value = FREQS[idx - 1];
	Buzzer_play(hz_value);
}

void Buzzer_alarm(){
	Buzzer_beep(10);
	os_wait2(K_TMO, 10);
	Buzzer_stop();
	os_wait2(K_TMO, 5);
	
	Buzzer_beep(9);
	os_wait2(K_TMO, 10);
	Buzzer_stop();
	os_wait2(K_TMO, 5);
	
	Buzzer_beep(8);
	os_wait2(K_TMO, 10);
	Buzzer_stop();
	os_wait2(K_TMO, 5);
}

void Buzzer_alarm2(){
	Buzzer_beep(8);
	os_wait2(K_TMO, 20);
	Buzzer_stop();
	os_wait2(K_TMO, 10);
	
	Buzzer_beep(9);
	os_wait2(K_TMO, 20);
	Buzzer_stop();
	os_wait2(K_TMO, 10);
	
	Buzzer_beep(10);
	os_wait2(K_TMO, 20);
	Buzzer_stop();
	os_wait2(K_TMO, 10);
}

void Buzzer_alarm3(){
	Buzzer_beep(14);
	os_wait2(K_TMO, 10);
	Buzzer_stop();
	os_wait2(K_TMO, 5);
	
	Buzzer_beep(14);
	os_wait2(K_TMO, 10);
	Buzzer_stop();
	os_wait2(K_TMO, 5);
	
	Buzzer_beep(14);
	os_wait2(K_TMO, 10);
	Buzzer_stop();
	os_wait2(K_TMO, 5);
}


// 停止播放
void Buzzer_stop(){
	
  PWMx_InitDefine		PWMx_InitStructure;
  PWMx_InitStructure.PWM_EnoSelect  = 0;			//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM8, &PWMx_InitStructure);			//初始化PWM,  PWMA,PWMB
}