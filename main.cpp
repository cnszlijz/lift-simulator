#include<vector>
#include<deque> 
#include "lift.hpp"
//class lift{
	/*
	由于保存函数太难了，所以这样操作：每次改变状态时先改变，然后设置waitsec的僵直时间，即现在变量里的状态是waitsec秒后的，等他到0了就可以进行下一次行动。
	上下人另写函数，需要占用时间。
	每次可以操作时用update_state决定是否需要切换状态。 
	电梯情况分为closedoor0,opendoor1,uping2,downing3四种。四种间转换都需要时间。
	*/
	/*public:
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
};*/
template<typename lft>
class simulator{
	public:
		const int n,//number of lifts
		          h;//floors, start at 1 and end at h
		simulator(int nn, int hh):n(nn),h(hh){
			while(nn--){
				L.push_back(lft(hh));
			}
		}
		bool run(){//run 1s, update all states, return true if we do nothing
		//按照约定，第n次执行完run()后的结果应该是电梯已经运行到第n秒但是还没上下乘客和统计信息。
		//如，第三次run()后，电梯运行到第3秒，但是刚刚开门还未上人/下人。
		//第零次run()后，即第一次run()前，电梯运行到第0秒，第0秒的人也还没上电梯。 
			//检查僵直时间是否为0 
			//上下乘客 
			//更新统计信息 
			//将L里面的电梯情况更新到下一秒。 
			//最后，所有僵直时间-1，跳转到下一秒。 
			return true;
		}
		bool insertoneperson(int nowfloor, int gotofloor){//insert one person
			waitpeople[nowfloor].push_back(gotofloor);
			return true;
			//return false;//true is ok, false is failed
		}
	private:
		std::vector<lft> L;//lifts
		//放到lift类里面去//void (*update_state)(std::vector<lift> &L);//update lifts' state,only the lifts, such as gotofloor, not persons
		int sec;//simulator has run secs
		std::vector<std::deque<int>>waitpeople;//waitpeople[h]is the vector of people waiting at floor h, their gotofloor
};
int main() {
	return 0;
}

