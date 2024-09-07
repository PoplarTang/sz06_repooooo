#include "App.h"
#include "Ultrasonic.h"
#include "Buzzer.h"

u8 is_ultrasonic_enable = 0; // 0关闭，1开启
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
      printf("当前距离: %.2f cm\n", distance);
      global_distance_cm = distance;

      // 如果距离小于20cm，蜂鸣器响
      Buzzer_alarm();
    }else {
      printf("获取距离失败\n");
    }

    // 200ms
    os_wait2(K_TMO, 40); // 200ms = 40 * 5ms
  }

}