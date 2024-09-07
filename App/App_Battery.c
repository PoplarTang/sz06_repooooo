#include "App.h"
#include "Battery.h"
#include "Buzzer.h"

#define LOW_POWER	10.5

float voltage;
// �ж��Ƿ��ǵ͵���
u8 isLowPower(){ // С�շ�д��
    return voltage < LOW_POWER;
}

// 2000ms��ȡһ�ε���

void task_battery() _task_ TASK_BATTERY {

	// ��ʼ�����ģ��
	Battery_init();
	
	// ѭ����ȡ���жϵ���
	while(1){
		voltage = Battery_get_voltage();
		printf("vol: %.2f V\n", voltage);
		
		if(isLowPower()){
			printf("Battery voltage is too low, please charge it: %.2f V\n", voltage);
			Buzzer_alarm3();
		}
	
		// �ȴ����� 2000ms
		os_wait2(K_TMO, 200);
		os_wait2(K_TMO, 200);
	}
	
}