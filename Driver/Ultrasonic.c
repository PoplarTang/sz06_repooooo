#include "Ultrasonic.h"
#include "GPIO.h"

void Ultrasonic_init(){
	ULTRA_GPIO_INIT();
	
	// ���������Ŷ�����
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
��ȡ��ǰ����������

distance���ڳɹ�ʱ�����Զ����Ϊ���ݣ���λcm
����ֵ��״̬ 0�ɹ���-1ʧ��
*/
char Ultrasonic_get_distance(float * distance){
	u16 cnt_10us = 0;
	// 1. ��TRIG����10us���ϣ����壩
	TRIG = 1;
	Delay10us();
	Delay10us();
	TRIG = 0;
	
	// Delayһ��ʱ�䣬ȷ��ECHO������
//	Delay10us();
	
	// 2. �ȴ�ECHO�����ߣ���һ��ʱ��������
	while(ECHO == 0 && cnt_10us <= 300){
		Delay10us(); // 10us
		cnt_10us++;  // 2200us -> 220 -> 300
	}
	
	if(cnt_10us >= 300){ // ����3000usû��Ӧ��3ms��
		printf("ECHO�����źų�ʱ: %d us,�˳����\n", (int) (cnt_10us * 10));
		return FAIL;
	}
	
//	printf("ECHO: %d us\n", (int)(cnt_10us * 10));
	// 3. ��¼ECHO�������ص��½��صļ��ʱ��
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
	
	if(*distance < 2.0){ // ����̫�̣���ֵ����֤
		return 1;
	}else if(*distance > 400){
		return 2;
	}
	
	return SUCCESS;
}