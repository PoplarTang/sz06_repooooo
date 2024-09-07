#include "App.h"
#include "Tracker.h"
#include "Motors.h"

u8 is_tracking = 0; //0关闭巡线 1开启巡线

void task_tracker() _task_ TASK_TRACK {
  u8 states[5] = {0};
//  u8 i;
  int pos = 0, last_pos = -10000;
  Tracker_init();
  while(1) {
    pos = Tracker_get_position();

    if(isLowPower()) { // 电量低，停车
      Motors_stop();
      os_wait2(K_TMO, 4);// 20ms
      continue;
    }

    // 如果启用了超声测距，则判断障碍物距离，如果小于20cm，则停车
    if(is_ultrasonic_enable && global_distance_cm != -1) {
      if(global_distance_cm < 20) {
        printf("停车，前方有障碍物: %.2f cm\n", global_distance_cm);
        Motors_stop();
        os_wait2(K_TMO, 4);// 20ms
        last_pos = -10000; // 恢复默认值，避免障碍物移除后，继续停车
        continue;
      }
    }

    // 如果pos没有发生变化，不要重复执行
    if(pos == last_pos) {
      os_wait2(K_TMO, 4);// 20ms， 50Hz
      continue;
    }
    // 记录当前值，作为上一次状态
    last_pos = pos;

    printf("pos-> %d\n", pos);

    if(pos < 0) { // 左转
      Motors_around(30, 0);
      printf("Change-> %d 左转\n", pos);
    } else if(pos > 0) { // 右转
      Motors_around(30, 1);
      printf("Change-> %d 右转\n", pos);
    } else {
      Motors_forward(30);
      printf("Change-> %d 直行\n", pos);
    }
//    os_wait2(K_TMO, 10);// 50ms
    os_wait2(K_TMO, 4);// 20ms
  }

}