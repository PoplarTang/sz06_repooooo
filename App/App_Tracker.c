#include "App.h"
#include "Tracker.h"
#include "Motors.h"

u8 is_tracking = 0; //0�ر�Ѳ�� 1����Ѳ��

void task_tracker() _task_ TASK_TRACK {
  u8 states[5] = {0};
//  u8 i;
  int pos = 0, last_pos = -10000;
  Tracker_init();
  while(1) {
    pos = Tracker_get_position();

    if(isLowPower()) { // �����ͣ�ͣ��
      Motors_stop();
      os_wait2(K_TMO, 4);// 20ms
      continue;
    }

    // ��������˳�����࣬���ж��ϰ�����룬���С��20cm����ͣ��
    if(is_ultrasonic_enable && global_distance_cm != -1) {
      if(global_distance_cm < 20) {
        printf("ͣ����ǰ�����ϰ���: %.2f cm\n", global_distance_cm);
        Motors_stop();
        os_wait2(K_TMO, 4);// 20ms
        last_pos = -10000; // �ָ�Ĭ��ֵ�������ϰ����Ƴ��󣬼���ͣ��
        continue;
      }
    }

    // ���posû�з����仯����Ҫ�ظ�ִ��
    if(pos == last_pos) {
      os_wait2(K_TMO, 4);// 20ms�� 50Hz
      continue;
    }
    // ��¼��ǰֵ����Ϊ��һ��״̬
    last_pos = pos;

    printf("pos-> %d\n", pos);

    if(pos < 0) { // ��ת
      Motors_around(30, 0);
      printf("Change-> %d ��ת\n", pos);
    } else if(pos > 0) { // ��ת
      Motors_around(30, 1);
      printf("Change-> %d ��ת\n", pos);
    } else {
      Motors_forward(30);
      printf("Change-> %d ֱ��\n", pos);
    }
//    os_wait2(K_TMO, 10);// 50ms
    os_wait2(K_TMO, 4);// 20ms
  }

}