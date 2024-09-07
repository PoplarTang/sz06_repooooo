#include "App.h"
#include "Ultrasonic.h"
#include "Buzzer.h"

u8 is_ultrasonic_enable = 0; // 0�رգ�1����
float global_distance_cm = -1;

void task_ultrasonic() _task_ TASK_ULTRASONIC {
  float distance = 0;

  Ultrasonic_init();

  while(1){

    if (!is_ultrasonic_enable) {
      os_wait2(K_TMO, 40); // 200ms
      continue;
    }

    if(Ultrasonic_get_distance(&distance) == 0){
      printf("��ǰ����: %.2f cm\n", distance);
      global_distance_cm = distance;

      // �������С��20cm����������
      Buzzer_alarm();
    }else {
      printf("��ȡ����ʧ��\n");
    }

    // 200ms
    os_wait2(K_TMO, 40); // 200ms = 40 * 5ms
  }

}