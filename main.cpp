#include<vector>
#include<deque> 
#include "lift.hpp"
class lift{
	/*
	��Ҫ����floor,people,waitsec,waitevent
	��Ҫ��¶����gotofloor,getfloor��
	����֧��break(cancel)��isbreakable���Ȳ�֧�� 	
	
	���ڱ��溯��̫���ˣ���������������ÿ�θı�״̬ʱ�ȸı䣬Ȼ������waitsec�Ľ�ֱʱ�䣬�����ڱ������״̬��waitsec���ģ�������0�˾Ϳ��Խ�����һ���ж���
	��������д������Ȼ��update_state�ɳ�ʼ��ָ�� 
	*/
	public:
		int waitsec = 0;
		//const int K=3,B=-4,doortime=5;//time=doortime+(K*floor+B)+doortime
		const int doortime=4,floortime=3,acceltime=3,onefloortime=8;//������doortime������һ¥floortime����β������acceltime��ֻ��һ����onefloortime
		lift(const int h):H(h),floor(1),people(0){
		}
		int getfloor(){
			return floor;
		}
		void gotofloor(int f){//����ȥ f floor, but not ���ȥ����Ҫ�� simulator::run ����ȥ 
			;
		}
	private:
		const int H;
		const int MAX_PEOPLE=13;
		int floor;//now floor
		int peoplecnt;//now people
		std::vector<int>people;
		int isrunning=0;//1 up, -1 down
		bool isdooropen=false;
};
class simulator{
	public:
		const int n,//number of lifts
		          h;//floors, start at 1 and end at h
		simulator(int nn, int hh):n(nn),h(hh){
			while(nn--){
				L.push_back(lift<update_state>(hh));
			}
		}
		bool run(){//run 1s, update all states, return true if we do nothing
			return true;
		}
		bool insertoneperson(int nowfloor, int gotofloor){//insert one person
			return false;//true is ok, false is failed
		}
	private:
		std::vector<lift<update_state>> L;//lifts
		//�ŵ�lift������ȥ//void (*update_state)(std::vector<lift> &L);//update lifts' state,only the lifts, such as gotofloor, not persons
		int sec;//simulator has run secs
		std::vector<std::deque<int>>waitpeople;//waitpeople[h]is the vector of people waiting at floor h, their gotofloor
};
int main() {
	return 0;
}

