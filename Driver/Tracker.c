#include "Tracker.h"
#include "GPIO.h"

void Tracker_init(){
	TRACKER_GPIO_INIT();
}


// ɨ�赱ǰ���еƵ�״̬���浽����
// ��ĵƣ�1�ߵ�ƽ�����ߣ���������û�յ��⣩
// ���ĵƣ�0�͵�ƽ�����棨���������յ�����Ĺ⣩
void Tracker_scan(u8 states[]){
	states[0] = TRACK_0;
	states[1] = TRACK_1;
	states[2] = TRACK_2;
	states[3] = TRACK_3;
	states[4] = TRACK_4;
}

// ��ȡ��λ��
// [-64, -32, 0, 32, 64]
// ����ֵ��0ֱ�ߣ�>0��ת��<0��ת
int Tracker_get_position(){
	static int last_pos = 0;
	// ����Ƶ�λ�õ���ֵ��ֵ
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
	
	// û�е�����û��ѹ������
	if(cnt == 0){
		// ״̬��ʧ������һ��״̬λ��
		return last_pos;
//		return 0;
	}
	
	// ���ֵ avg = sum / cnt;
	pos = sum / cnt;
	
	// ��¼��ǰֵ����Ϊ��һ��״̬
  // ������һ��û��ѹ��ʱ��״̬	
	last_pos = pos;
	return pos;
}