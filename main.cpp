#include<vector>
#include<deque> 
#include "lift.hpp"
class lift{
	/*
	需要保存floor,people,waitsec,waitevent
	需要暴露函数gotofloor,getfloor。
	可以支持break(cancel)和isbreakable，先不支持 	
	
	由于保存函数太难了，所以这样操作：每次改变状态时先改变，然后设置waitsec的僵直时间，即现在变量里的状态是waitsec秒后的，等他到0了就可以进行下一次行动。
	上下人另写函数。然后update_state由初始化指定 
	*/
	public:
		int waitsec = 0;
		//const int K=3,B=-4,doortime=5;//time=doortime+(K*floor+B)+doortime
		const int doortime=4,floortime=3,acceltime=3,onefloortime=8;//开关门doortime，上下一楼floortime，首尾层额外加acceltime，只有一层用onefloortime
		lift(const int h):H(h),floor(1),people(0){
		}
		int getfloor(){
			return floor;
		}
		void gotofloor(int f){//请求去 f floor, but not 真的去，需要再 simulator::run 里面去 
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
		//放到lift类里面去//void (*update_state)(std::vector<lift> &L);//update lifts' state,only the lifts, such as gotofloor, not persons
		int sec;//simulator has run secs
		std::vector<std::deque<int>>waitpeople;//waitpeople[h]is the vector of people waiting at floor h, their gotofloor
};
int main() {
	return 0;
}

