#include "App.h"
#include "GPIO.h"
#include "UART.h"

#include "Motors.h"
#include "Light.h"
#include "Buzzer.h"

/************************************
 帧头  X  Y  A  B  C  D
DD 77 00 2F 01 00 00 00
DD 77 00 00 00 00 00 00

DD 77 00 00 01 00 00 00  A按下 亮灯，蜂鸣器, 超声测距
DD 77 00 00 00 01 00 00  B按下 左旋（左掉头）
DD 77 00 00 00 00 01 00  C按下 右旋（右掉头）
DD 77 00 00 00 00 00 01  D按下 启停巡线

超声测距：如果前方障碍物距离小于15cm，停车(蜂鸣器)
**************************************/
#define BTN_A  4
#define BTN_B  5
#define BTN_C  6
#define BTN_D  7
u8 light_state = 0; // 0关闭，1开启
u8 is_turning  = 0; // 0停止，1转


void do_control(u8 buffer[], u8 len) {
  char x, y;

//  if(RX2_Buffer[0] != 0xDD || RX2_Buffer[1] != 0x77) {
//    return;
//  }

  // -------------------------------------------------------按钮A
  if(buffer[BTN_A]) {
    light_state = !light_state;
    // 灯光开关
    light_state ? Light_on(LIGHT_ALL): Light_off(LIGHT_ALL);
    // 蜂鸣器
    light_state ? Buzzer_alarm(): Buzzer_alarm2();
    // 根据light_state状态, 启停超声测距功能
    is_ultrasonic_enable = light_state;

    return;
  }

  // -------------------------------------------------------按钮BC
  if (buffer[BTN_B]) { // 左旋
    Motors_around(30, 0);
    is_turning = TRUE;
    return;
  } else if (RX2_Buffer[BTN_C]) { // 右旋
    Motors_around(30, 1);
    is_turning = TRUE;
    return;
  } else {
    if(is_turning) {
      // 之前在旋转状态， 停止旋转
      Motors_stop();
      is_turning = FALSE;
      return;
    }
  }

  // -------------------------------------------------------按钮D, 启停巡线
  if(buffer[BTN_D]) { // 启停巡线
    is_tracking = !is_tracking;
    if(is_tracking) {
      os_create_task(TASK_TRACK); // 开始巡线
      printf("Start tracking!\n");
    } else {
      os_delete_task(TASK_TRACK); // 停止巡线
      Motors_stop(); // 停车
      printf("Stop tracking!\n");
    }
    return;
  }

  if(isLowPower()) { // 电量低
		printf("Can't Move.\n");
    // 不允许Move
    Motors_stop(); // 停车
    return;
  }

// 如果开启了巡线任务，则不执行以下代码
  if(is_tracking) {
    return;
  }

  // 打印x,y
  x = buffer[2];
  y = buffer[3];

  printf("%d, %d\n", (int)x, (int)y);

  Motors_move(x, y);
}

/******
进行控制之前的解析操作
0xF3A0

DD 77 00 2F 00 00 00 00 一帧数据

DD 77 00 2F 00 00 00 00 DD 77 00 00 00 00 00 00 前进指令+停止指令

DD 77 EF 50 01 00 01 00 DD 77 00 00 01 00 01 00							==>16 前进指令+停止指令
DD 77 EF 50 01 DC DD 77 13 77 00 00 01 00 01 00							==>16 前进指令+停止指令
DD 77 EF 50 01 00 01 00 DD 77 00 00  												==> 8 + 4 = 12
DD 77 EF 50 01 00 01 00 DD 77 00 00 01 00 01 00 DD 77 3A 4B	==> 16 + 4 = 20

*******/
void do_control_parse(){
	u8 i;
	// 进行长度判定
	if(COM2.RX_Cnt < 8) return;
	
//	printf("addr: %p\n", RX2_Buffer);
//	printf("addr: %p\n", &RX2_Buffer[0]);
//	printf("addr: %p\n", RX2_Buffer + 0);
	
//	printf("dat-> ");
//	for(i=0; i<COM2.RX_Cnt; i++)	{
//		// RX2_Buffer[i]存的是接收的每个字节，写出用 TX2_write2buff
//		printf("%02X ", (int)RX2_Buffer[i]);
//	}
//	printf("\n");
	
	// 循环拆包
	for(i = 0; i <= COM2.RX_Cnt - 8; i++){
		// 判断当前位置是否是DD,77
		if(RX2_Buffer[i] == 0xDD && RX2_Buffer[i + 1] == 0x77){
			do_control(&RX2_Buffer[i], 8);
			i += 7;
		}
	}
	
	
}

void task_bluetooth() _task_ TASK_BLUETOOTH{
  u8 i;
  // 串口2接收蓝牙, 通过串口1发给PC
  while(1) {
    if(COM2.RX_TimeOut > 0 && --COM2.RX_TimeOut == 0) {
      //超时计数
      if(COM2.RX_Cnt > 0) {
//        do_control();
				do_control_parse(); // Hello123
//				printf("UART2: %d\n", (int)COM2.RX_Cnt);
        // 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart2_recv
//        for(i=0; i<COM2.RX_Cnt; i++)	{
//          // RX2_Buffer[i]存的是接收的每个字节，写出用 TX2_write2buff
//          TX1_write2buff(RX2_Buffer[i]);
//        }
      }
      COM2.RX_Cnt = 0;
    }

    os_wait2(K_TMO, 2); // 10ms * 5 = 50ms
  }
}