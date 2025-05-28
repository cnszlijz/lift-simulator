#include<vector>
#include<deque>
#include<iostream>
#include<exception>
#include<cstdlib>
const double TIME_ONEFLOOR=10.26;
const double K=2.09964, B=8.276;//time=K*f+B;
inline double getrunningtime(int f){//from a floor goto a+f or a-f floor, maybe include time of door opening and closing
	if(f<1)return B/2; 
	//if(f<=1)return TIME_ONEFLOOR;
	return K*f+B;
}
struct lift{ 
	public:
		lift(int hh):h(hh) { people.reserve(MAX_PEOPLE); }
		int waittime=0;//僵直时间 
		const int h;//height,floor is from 1 to h
		const unsigned MAX_PEOPLE=13;
		int floor=1;//now floor
		std::vector<int>people;
		int tag=0;
};
class simulator{
	public:
		const int n,//number of lifts
		          h;//floors, start at 1 and end at h
		simulator(int nn, int hh):n(nn),h(hh){
			while(nn--){
				L.push_back(lift(hh));
			}
			waitpeople.push_back(std::deque<int>());
			while(hh--){
				waitpeople.push_back(std::deque<int>());
			}
		}
		//run 1s, update all states, return true if we do nothing
		bool run(){
			random_insert_person(waitpeople);
			for(auto l:L){
				if(l.waittime!=0)
					continue;
				updatepassengers(l);
				if(l.waittime==0){
					//update_state
				}
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
	void random_insert_person(std::vector<std::deque<int>> &q, int clear=0){
		//用static计数，每N秒进一个人 
		static const double freq=1.0;
		static const unsigned seed=1145;
		static double s=0.0;
		if(clear){
			s=0.0;
			srand(seed);
			return;
		}
		s += 1.0;
		while(s>=freq){
			s-=freq;
			//do_random_insert_person once
			const int n=q.size()-1;//q[0]禁用 
			//int gotofloor=rand()%n+1;
			int gotofloor=1, nowfloor=rand()%(n-1)+1;
			q[nowfloor].push_back(gotofloor);//2-n楼去1楼 
		}
	}
	protected:
		std::vector<lift> L;//lifts
		//int sec;//sec外部计时，内部只管run()，返回true就是idle了。 
		std::vector<std::deque<int>>waitpeople;//waitpeople[h]is the vector of people waiting at floor h, their gotofloor
		void updatepassengers(lift &l){
			if(l.tag){
				l.tag=0;//无状态的无法之举 
				return;
			}
			//已经检查过waittime了，不检查 
			int floor=l.floor;
			int ups=0, downs=0;
			//down person
			for(auto i=l.people.begin();i<l.people.end();){
				if(*i==floor){
					i=l.people.erase(i);
					++downs;
				} else
					++i;
			}
			//up person
			while(waitpeople[floor].size()>0){
				if(l.people.size()>=l.MAX_PEOPLE)break;
				l.people.push_back(waitpeople[floor].front());
				waitpeople[floor].pop_front();
				++ups;
			}
			l.waittime+=getupdowntime(ups,downs);
			l.tag=1;
		}
		int getupdowntime(int ups, int downs){
			int sum=ups+downs;
			if(sum<1)sum=1;
			return 0.5*sum;//开关门各3s，上下1人0.5s，目前开关门算在外面了 
		} 
};
/*
关于统计信息：
电梯的运行路程应该在电梯内统计
乘客等待时间不计算了，或者在外面计算（进入等待-sec，上电梯+sec，出电梯+sec），应该写进updateperson
其它没有什么要计算的，所以不应该写updatestats
*/
using std::cout; using std::endl; using std::cin;
int main() {
	return 0;
}

