#include "App.h"
#include "Battery.h"
#include "Buzzer.h"

#define LOW_POWER	10.5

float voltage;
// 判断是否是低电量
u8 isLowPower(){ // 小驼峰写法
    return voltage < LOW_POWER;
}

// 2000ms获取一次电量

void task_battery() _task_ TASK_BATTERY {

	// 初始化电池模块
	Battery_init();
	
	// 循环获取并判断电量
	while(1){
		voltage = Battery_get_voltage();
		printf("vol: %.2f V\n", voltage);
		
		if(isLowPower()){
			printf("Battery voltage is too low, please charge it: %.2f V\n", voltage);
			Buzzer_alarm3();
		}
	
		// 等待函数 2000ms
		os_wait2(K_TMO, 200);
		os_wait2(K_TMO, 200);
	}
	
}