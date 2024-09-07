#include "App.h"
#include "GPIO.h"
#include "UART.h"

#include "Motors.h"
#include "Light.h"
#include "Buzzer.h"

/************************************
 ֡ͷ  X  Y  A  B  C  D
DD 77 00 2F 01 00 00 00
DD 77 00 00 00 00 00 00

DD 77 00 00 01 00 00 00  A���� ���ƣ�������, �������
DD 77 00 00 00 01 00 00  B���� ���������ͷ��
DD 77 00 00 00 00 01 00  C���� �������ҵ�ͷ��
DD 77 00 00 00 00 00 01  D���� ��ͣѲ��

������ࣺ���ǰ���ϰ������С��15cm��ͣ��(������)
**************************************/
#define BTN_A  4
#define BTN_B  5
#define BTN_C  6
#define BTN_D  7
u8 light_state = 0; // 0�رգ�1����
u8 is_turning  = 0; // 0ֹͣ��1ת


void do_control(u8 buffer[], u8 len) {
  char x, y;

//  if(RX2_Buffer[0] != 0xDD || RX2_Buffer[1] != 0x77) {
//    return;
//  }

  // -------------------------------------------------------��ťA
  if(buffer[BTN_A]) {
    light_state = !light_state;
    // �ƹ⿪��
    light_state ? Light_on(LIGHT_ALL): Light_off(LIGHT_ALL);
    // ������
    light_state ? Buzzer_alarm(): Buzzer_alarm2();
    // ����light_state״̬, ��ͣ������๦��
    is_ultrasonic_enable = light_state;

    return;
  }

  // -------------------------------------------------------��ťBC
  if (buffer[BTN_B]) { // ����
    Motors_around(30, 0);
    is_turning = TRUE;
    return;
  } else if (RX2_Buffer[BTN_C]) { // ����
    Motors_around(30, 1);
    is_turning = TRUE;
    return;
  } else {
    if(is_turning) {
      // ֮ǰ����ת״̬�� ֹͣ��ת
      Motors_stop();
      is_turning = FALSE;
      return;
    }
  }

  // -------------------------------------------------------��ťD, ��ͣѲ��
  if(buffer[BTN_D]) { // ��ͣѲ��
    is_tracking = !is_tracking;
    if(is_tracking) {
      os_create_task(TASK_TRACK); // ��ʼѲ��
      printf("Start tracking!\n");
    } else {
      os_delete_task(TASK_TRACK); // ֹͣѲ��
      Motors_stop(); // ͣ��
      printf("Stop tracking!\n");
    }
    return;
  }

  if(isLowPower()) { // ������
		printf("Can't Move.\n");
    // ������Move
    Motors_stop(); // ͣ��
    return;
  }

// ���������Ѳ��������ִ�����´���
  if(is_tracking) {
    return;
  }

  // ��ӡx,y
  x = buffer[2];
  y = buffer[3];

  printf("%d, %d\n", (int)x, (int)y);

  Motors_move(x, y);
}

/******
���п���֮ǰ�Ľ�������
0xF3A0

DD 77 00 2F 00 00 00 00 һ֡����

DD 77 00 2F 00 00 00 00 DD 77 00 00 00 00 00 00 ǰ��ָ��+ָֹͣ��

DD 77 EF 50 01 00 01 00 DD 77 00 00 01 00 01 00							==>16 ǰ��ָ��+ָֹͣ��
DD 77 EF 50 01 DC DD 77 13 77 00 00 01 00 01 00							==>16 ǰ��ָ��+ָֹͣ��
DD 77 EF 50 01 00 01 00 DD 77 00 00  												==> 8 + 4 = 12
DD 77 EF 50 01 00 01 00 DD 77 00 00 01 00 01 00 DD 77 3A 4B	==> 16 + 4 = 20

*******/
void do_control_parse(){
	u8 i;
	// ���г����ж�
	if(COM2.RX_Cnt < 8) return;
	
//	printf("addr: %p\n", RX2_Buffer);
//	printf("addr: %p\n", &RX2_Buffer[0]);
//	printf("addr: %p\n", RX2_Buffer + 0);
	
//	printf("dat-> ");
//	for(i=0; i<COM2.RX_Cnt; i++)	{
//		// RX2_Buffer[i]����ǽ��յ�ÿ���ֽڣ�д���� TX2_write2buff
//		printf("%02X ", (int)RX2_Buffer[i]);
//	}
//	printf("\n");
	
	// ѭ�����
	for(i = 0; i <= COM2.RX_Cnt - 8; i++){
		// �жϵ�ǰλ���Ƿ���DD,77
		if(RX2_Buffer[i] == 0xDD && RX2_Buffer[i + 1] == 0x77){
			do_control(&RX2_Buffer[i], 8);
			i += 7;
		}
	}
	
	
}

void task_bluetooth() _task_ TASK_BLUETOOTH{
  u8 i;
  // ����2��������, ͨ������1����PC
  while(1) {
    if(COM2.RX_TimeOut > 0 && --COM2.RX_TimeOut == 0) {
      //��ʱ����
      if(COM2.RX_Cnt > 0) {
//        do_control();
				do_control_parse(); // Hello123
//				printf("UART2: %d\n", (int)COM2.RX_Cnt);
        // ���ﴦ���յ������ݣ���������߼������Ե����Լ���on_uart2_recv
//        for(i=0; i<COM2.RX_Cnt; i++)	{
//          // RX2_Buffer[i]����ǽ��յ�ÿ���ֽڣ�д���� TX2_write2buff
//          TX1_write2buff(RX2_Buffer[i]);
//        }
      }
      COM2.RX_Cnt = 0;
    }

    os_wait2(K_TMO, 2); // 10ms * 5 = 50ms
  }
}