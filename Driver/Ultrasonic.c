#include "Ultrasonic.h"
#include "GPIO.h"

void Ultrasonic_init(){
	ULTRA_GPIO_INIT();
	
	// 把两个引脚都拉低
	TRIG = 0;
	ECHO = 0;
}

void Delay10us(void)	//@24.000MHz
{
	unsigned char data i;

	i = 75; // 78
	while (--i);
}

/***
获取当前超声波距离

distance：在成功时，会自动填充为数据，单位cm
返回值：状态 0成功，-1失败
*/
char Ultrasonic_get_distance(float * distance){
	u16 cnt_10us = 0;
	// 1. 把TRIG拉高10us以上（脉冲）
	TRIG = 1;
	Delay10us();
	Delay10us();
	TRIG = 0;
	
	// Delay一段时间，确保ECHO被拉低
//	Delay10us();
	
	// 2. 等待ECHO被拉高，在一定时间内拉高
	while(ECHO == 0 && cnt_10us <= 300){
		Delay10us(); // 10us
		cnt_10us++;  // 2200us -> 220 -> 300
	}
	
	if(cnt_10us >= 300){ // 超过3000us没响应（3ms）
		printf("ECHO拉高信号超时: %d us,退出测距\n", (int) (cnt_10us * 10));
		return FAIL;
	}
	
//	printf("ECHO: %d us\n", (int)(cnt_10us * 10));
	// 3. 记录ECHO从上升沿到下降沿的间隔时间
	cnt_10us = 0;
	while(ECHO == 1){
		Delay10us();
		cnt_10us++;
	}
	// 3000us -> 3ms
	// us -> ms
	// printf("ECHO: %.2f ms\n", cnt_10us * 0.01);
	
	// dis = ((cnt_10us * 0.01)ms * 340m/s) / 2
	// dis = ((cnt_10us * 0.01)ms * (34000/1000) cm/ms) / 2
	// dis = ((cnt_10us * 0.01)ms * 34 cm/ms) / 2
	
	*distance = ((cnt_10us * 0.01) * 34) * 0.5;
//	*distance = (cnt_10us * 0.17);
	
	if(*distance < 2.0){ // 距离太短，数值不保证
		return 1;
	}else if(*distance > 400){
		return 2;
	}
	
	return SUCCESS;
}