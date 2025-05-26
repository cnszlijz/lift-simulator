#include<vector>
#include<deque>
#include<iostream>
#include<exception>
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
class lift{//基类，后面添加update_state在上面继承 
	public:
		lift(int hh):h(hh),state(0) { /*本来想一次性allocate MAX_PEOPLE的，后来发现没什么用，算了。*//*现在加回来*/people.reserve(MAX_PEOPLE); }
		int waittime=0;//僵直时间 
		int get_floor(){ return floor; }
		int down_person(){//尝试执行下电梯，返回下电梯的人数以供统计时间，如果没开门就返回0
			if(waittime>0)return 0;
			if(state!=1){
				if(state==0){
					state=1;
					waittime += DOOR_TIME;
				}
				return 0;
			}
			int cnt=0;
			for(auto i=people.begin();i<people.end();){
				if(*i==floor){
					i=people.erase(i);
					++cnt;
				} else {
					++i;
				}
			}
			return cnt;
		}
		bool try_up_person(int f){
			if(waittime>0)return false;
			if(people.size()>=MAX_PEOPLE)return false;
			if(state!=1){
				if(state==0){
					state=1;
					waittime += DOOR_TIME;
				}
				return false;
			}
			people.push_back(f);
			return true;
		}
		virtual void update_state(){}//do nothing in base class
	private:
		int h;//height,floor is from 1 to h
		const unsigned MAX_PEOPLE=13;
		int floor=1;//now floor
		std::vector<int>people;
		//states
		int state;
		/*
		0:关门 
		1:开门 
		2:上 
		4:下 
		8:正在关门 
		9:正在开门 
		3/5:开门运行，不应该存在 
		*/
		//运行参数 
		const int TIME_WITH_START=2,
				  TIME_WITHOUT_START=3,
				  DOOR_TIME=2;
};
void random_insert_person(std::vector<std::deque<int>> q, int sec){
}
template<typename lft>
class simulator{
	public:
		const int n,//number of lifts
		          h;//floors, start at 1 and end at h
		simulator(int nn, int hh):n(nn),h(hh){
			while(nn--){
				L.push_back(lft(hh));
			}
			waitpeople.push_back(std::deque<int>());
			while(hh--){
				waitpeople.push_back(std::deque<int>());
			}
		}
		//run 1s, update all states, return true if we do nothing
		bool run(){
		//按照约定，第n次执行完run()后的结果应该是电梯已经运行到第n秒但是还没上下乘客和统计信息。
		//如，第三次run()后，电梯运行到第3秒，但是刚刚开门还未上人/下人。
		//第零次run()后，即第一次run()前，电梯运行到第0秒，第0秒的人也还没上电梯。 
			random_insert_person(waitpeople, sec);
			for(auto l:L){
				if(l.waittime!=0){
					//检查僵直时间是否为0
					continue;
				}
				//上下乘客 
				updatepassengers(l);
				//更新统计信息 
				//updatestats(l);
				//将电梯情况更新到下一秒。 
				if(l.waittime==0)//可能关门了。这里先检查一遍，在update_state里面也应该检查一遍 
					l.update_state();
			}
			//最后，所有僵直时间-1，跳转到下一秒。 
			bool isidle=true;
			for(auto l:L){
				if(l.waittime>0){
					l.waittime -= 1;//不能合并，因为新的已经操作过的也要-=1
					isidle=false;
				}
			}
			return isidle;
		}
		bool insertoneperson(int nowfloor, int gotofloor){//insert one person
			waitpeople[nowfloor].push_back(gotofloor);
			return true;
			//return false;//true is ok, false is failed
		}
		int querywaitpeople(int floor){
			return waitpeople[floor].size();
		}
	private:
		std::vector<lft> L;//lifts
		//放到lift类里面去//void (*update_state)(std::vector<lift> &L);//update lifts' state,only the lifts, such as gotofloor, not persons
		int sec;//simulator has run secs
		std::vector<std::deque<int>>waitpeople;//waitpeople[h]is the vector of people waiting at floor h, their gotofloor
		void updatepassengers(lft &l){//对l和waitpeople进行上下乘客 
			//调用l的down_person和try_up_person
			int floor=l.get_floor();
			int downs=l.down_person();
			std::deque<int> &q=waitpeople[floor];
			int ups=0;
			while(q.size()>0){
				int i=q.front();
				if(l.try_up_person(i)){
					q.pop_front();
					++ups;
				}
				else break;
			}
			if(downs==0&&ups==0)return;
			else if(l.waittime>0){
				throw std::exception();
				return;
			}
			l.waittime+=getupdowntime(ups,downs);
		}
		//void updatestats(lft &l){}//更新与l有关的统计，l应该在L里并刚刚经历过改变//弃用 
		int getupdowntime(int ups, int downs){
			if(ups>1)ups=1+(ups-1+1)/2;//+1是上取整 
			if(downs>1)downs=1+(downs-1+1)/2;//第一个人算一个，后面每人算半个 
			int persons=ups+downs;
			if(persons<=1)return 0;
			return (persons-1)*1;//每人1s，开门可以出1个人 
		} 
};
/*
关于统计信息：
电梯的运行路程应该在电梯内统计
乘客等待时间不计算了，或者在外面计算（进入等待-sec，上电梯+sec，出电梯+sec），应该写进updateperson
其它没有什么要计算的，所以不应该写updatestats
*/
using std::cout; using std::endl;
int main() {
	simulator<lift> s(2,13);
	cout<<'1'<<endl;
	for(int i=2;i<=13;++i)s.insertoneperson(i,i-1),cout<<'a';
	for(int i=1;i<=13;++i)std::cout<<s.querywaitpeople(i)<<' ';
	return 0;
}

