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
麦轮循迹小车

- 按下按键：所有车轮前进，后退


******************/

void GPIO_config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//初始化
}

void UART_config(void) {
  // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
  COMx_InitDefine		COMx_InitStructure;					//结构定义
	
	// ------------------------UART1------------------------
  COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
  COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
  COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
  UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
  UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44

	// ------------------------UART2------------------------
  COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
  COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
  COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
  UART_Configuration(UART2, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  NVIC_UART2_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
  UART2_SW(UART2_SW_P10_P11);		// 引脚选择, UART2_SW_P10_P11,UART2_SW_P46_P47
}


void Key_on_up() {
  Light_off(LIGHT_ALL);
  printf("Key up\n");
//	Motors_stop();
}
float vcc = 0;

// F:Forward前进
// B:Backward后退
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

  // 0前进，1后退，2左平移，3右平移, default停止
  // 4右掉头，5左掉头
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
//	// 等待PWM输出1000ms
//	os_wait2(K_TMO, 200); // 200 * 5ms = 1000ms
//	// 停车
//	Motors_stop();

	 is_tracking = !is_tracking;
	 if(is_tracking){
		 os_create_task(TASK_TRACK); // 开始巡线
		 printf("Start tracking!\n");
	 }else {
		 os_delete_task(TASK_TRACK); // 停止巡线
		 Motors_stop(); // 停车
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
// RTX51定义函数入口
void task_main() _task_ 0 {

  // 初始化外设
  sys_init();

  // 创建按键任务
  os_create_task(TASK_KEY);
  // 创建巡线任务
//  os_create_task(TASK_TRACK);
  // 创建串口1接收任务
  os_create_task(TASK_UART1);
  // 创建串口2接收任务
  os_create_task(TASK_BLUETOOTH);
  // 创建超声测距任务
  os_create_task(TASK_ULTRASONIC);
  // 创建电池电量检测任务
  os_create_task(TASK_BATTERY);
  // 销毁自己
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
  // 串口1接收PC, 通过串口2发给蓝牙
  while(1) {
    if(COM1.RX_TimeOut > 0 && --COM1.RX_TimeOut == 0) {
      //超时计数
      if(COM1.RX_Cnt > 0) {
//				printf("UART1: %d\n", (int)COM1.RX_Cnt);
        // 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart1_recv
        for(i=0; i<COM1.RX_Cnt; i++)	{
          // RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
          TX2_write2buff(RX1_Buffer[i]);
        }
      }
      COM1.RX_Cnt = 0;
    }

    // 不要处理的太快
    // delay_ms(10);
    os_wait2(K_TMO, 2);
  }
}
