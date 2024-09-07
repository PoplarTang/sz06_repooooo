#include "Config.h"
#include "Delay.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"

#include "App.h"

#include "Light.h"
#include "Key.h"
#include "Battery.h"
#include "Buzzer.h"
#include "Ultrasonic.h"
#include "Motors.h"
#include "Tracker.h"

/******************
����ѭ��С��

- ���°��������г���ǰ��������


******************/

void GPIO_config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//��ʼ��
}

void UART_config(void) {
  // >>> �ǵ���� NVIC.c, UART.c, UART_Isr.c <<<
  COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	
	// ------------------------UART1------------------------
  COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
  COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
  COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
  UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4

  NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
  UART1_SW(UART1_SW_P30_P31);		// ����ѡ��, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44

	// ------------------------UART2------------------------
  COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
  COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
  COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
  UART_Configuration(UART2, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4

  NVIC_UART2_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
  UART2_SW(UART2_SW_P10_P11);		// ����ѡ��, UART2_SW_P10_P11,UART2_SW_P46_P47
}


void Key_on_up() {
  Light_off(LIGHT_ALL);
  printf("Key up\n");
//	Motors_stop();
}
float vcc = 0;

// F:Forwardǰ��
// B:Backward����
/**
duty:    0  --------------- 50 ---------------  100
speed: B_Max---------------  0 --------------- F_Max
**/

float duty = 50.0f;
int direction_step = 10;

u8 move_state = 0;
u8 is_forward = 1;


void Key_on_down() {
  Light_on(LIGHT_ALL);
  printf("Key down\n");

//	duty += direction_step;
//	if(duty >= 100){
//		direction_step = -10;
//	}else if(duty <= 0){
//		direction_step = 10;
//	}
//
//	printf("duty: %.2f\n", duty);
//	dutyA.PWM3_Duty = PERIOD * duty / 100;
//	UpdatePwm(PWM3, &dutyA);

  // 0ǰ����1���ˣ�2��ƽ�ƣ�3��ƽ��, defaultֹͣ
  // 4�ҵ�ͷ��5���ͷ
//	switch(move_state){
//		case 0:  Motors_forward(20); 		break;
//		case 1:  Motors_backward(20); 	break;
//		case 2:  Motors_left(30, 0); 		break;
//		case 3:  Motors_right(30, 0); 	break;
//		case 4:  Motors_around(40, 1); 	break;
//		case 5:  Motors_around(40, 0); 	break;
//		default: Motors_stop(); 				break;
//	}
//	move_state++;
//
//	if(move_state > 5) move_state = 0;
////	move_state %= 4; // 0,1,2,3...
//
//	// �ȴ�PWM���1000ms
//	os_wait2(K_TMO, 200); // 200 * 5ms = 1000ms
//	// ͣ��
//	Motors_stop();

	 is_tracking = !is_tracking;
	 if(is_tracking){
		 os_create_task(TASK_TRACK); // ��ʼѲ��
		 printf("Start tracking!\n");
	 }else {
		 os_delete_task(TASK_TRACK); // ֹͣѲ��
		 Motors_stop(); // ͣ��
		 printf("Stop tracking!\n");
	 }
}

void sys_init() {
  EAXSFR();

  GPIO_config();
  UART_config();

  Light_init();
  Key_init();
	Battery_init();
	Buzzer_init();
  Motors_init();

  EA = 1;
}
// RTX51���庯�����
void task_main() _task_ 0 {

  // ��ʼ������
  sys_init();

  // ������������
  os_create_task(TASK_KEY);
  // ����Ѳ������
//  os_create_task(TASK_TRACK);
  // ��������1��������
  os_create_task(TASK_UART1);
  // ��������2��������
  os_create_task(TASK_BLUETOOTH);
  // ���������������
  os_create_task(TASK_ULTRASONIC);
  // ������ص����������
  os_create_task(TASK_BATTERY);
  // �����Լ�
  os_delete_task(0);

}

void task_key() _task_ 1 {

  while(1) {
    Key_scan();
    os_wait2(K_TMO, 4);// 20ms
  }
}

void task_uart1() _task_ TASK_UART1{
  u8 i;
  // ����1����PC, ͨ������2��������
  while(1) {
    if(COM1.RX_TimeOut > 0 && --COM1.RX_TimeOut == 0) {
      //��ʱ����
      if(COM1.RX_Cnt > 0) {
//				printf("UART1: %d\n", (int)COM1.RX_Cnt);
        // ���ﴦ���յ������ݣ���������߼������Ե����Լ���on_uart1_recv
        for(i=0; i<COM1.RX_Cnt; i++)	{
          // RX1_Buffer[i]����ǽ��յ�ÿ���ֽڣ�д���� TX1_write2buff
          TX2_write2buff(RX1_Buffer[i]);
        }
      }
      COM1.RX_Cnt = 0;
    }

    // ��Ҫ�����̫��
    // delay_ms(10);
    os_wait2(K_TMO, 2);
  }
}
