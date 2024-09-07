#include "Tracker.h"
#include "GPIO.h"

void Tracker_init(){
	TRACKER_GPIO_INIT();
}


// 扫描当前所有灯的状态，存到数组
// 灭的灯：1高电平，黑线（光敏电阻没收到光）
// 亮的灯：0低电平，地面（光敏电阻收到反射的光）
void Tracker_scan(u8 states[]){
	states[0] = TRACK_0;
	states[1] = TRACK_1;
	states[2] = TRACK_2;
	states[3] = TRACK_3;
	states[4] = TRACK_4;
}

// 读取线位置
// [-64, -32, 0, 32, 64]
// 返回值：0直走，>0右转，<0左转
int Tracker_get_position(){
	static int last_pos = 0;
	// 求灭灯的位置的数值均值
	int sum = 0, cnt = 0, pos = 0;
	if(TRACK_0){
		sum += -64;
		cnt++;
	}
	if(TRACK_1){
		sum += -32;
		cnt++;
	}
	if(TRACK_2){
		sum += 0;
		cnt++;
	}
	if(TRACK_3){
		sum += 32;
		cnt++;
	}
	if(TRACK_4){
		sum += 64;
		cnt++;
	}
	
	// 没有灯亮，没有压到黑线
	if(cnt == 0){
		// 状态丢失，以上一次状态位置
		return last_pos;
//		return 0;
	}
	
	// 求均值 avg = sum / cnt;
	pos = sum / cnt;
	
	// 记录当前值，作为上一次状态
  // 用于下一次没有压线时的状态	
	last_pos = pos;
	return pos;
}