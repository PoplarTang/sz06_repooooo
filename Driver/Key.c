#include "Key.h"
#include "GPIO.h"

#define KEY		P05

void Key_init(){
	P0_MODE_IO_PU(GPIO_Pin_5);
}


void Key_scan(){
	static u8 pre_state = 1;
	
	// ���״̬�����仯
	if(KEY != pre_state){
		
		if(KEY){ // ̧��
			Key_on_up();
		}else {	 // ����
			Key_on_down();
		}
		
		pre_state = KEY;
	}
}