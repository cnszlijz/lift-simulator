#include<vector>
#include<deque>
#include<iostream>
#include<exception>
#include<cstdlib>
using std::cout; using std::endl; using std::cin;
const double TIME_ONEFLOOR=10.26;
const double K=2.09964, B=8.276;//time=K*f+B;
inline double getrunningtime(int f){//from a floor goto a+f or a-f floor, maybe include time of door opening and closing
	if(f<0)f=-f;//abs
	if(f<1)return B/2;
	//if(f<=1)return TIME_ONEFLOOR;
	return (K*f+B);
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
			//random_insert_person(waitpeople);
			for(auto &l:L){
				if(l.waittime!=0)
					continue;
				updatepassengers(l);
				if(l.waittime==0){
					update_state(l);
				}
			}
			//最后，所有僵直时间-1，跳转到下一秒。
			bool isidle=true;
			for(auto &l:L){
				if(l.waittime>0){
					l.waittime -= 1;//不能合并，因为新的已经操作过的也要-=1
					isidle=false;
				}
			}
			return isidle;
		}
		void random_insert_person(int clear=0){
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
				//int gotofloor=rand()%n+1;
				int gotofloor=1, nowfloor=rand()%(h-1)+2;
				waitpeople[nowfloor].push_back(gotofloor);//2-n楼去1楼 
			}
		}
	//protected:
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
			if(ups||downs){
				l.waittime+=getupdowntime(ups,downs);
				l.tag=1;
			}
		}
		int getupdowntime(int ups, int downs){
			int sum=ups+downs;
			if(sum<1)sum=1;
			return 0.5*sum;//开关门各3s，上下1人0.5s，目前开关门算在外面了 
		}
		void update_state(lift &l){//已检查过waittime 
			if(l.floor<=1){//上行 
				int maxfloor=h;
				while(waitpeople[maxfloor].size()==0&&maxfloor>1)maxfloor--;
				l.floor=maxfloor;
				l.waittime+=getrunningtime(maxfloor-1);
			} else {
				int nextfloor=l.floor-1;
				if(l.people.size()==l.MAX_PEOPLE){
					//保送 
					nextfloor=1;
				} else{
					while(nextfloor>1&&waitpeople[nextfloor].size()==0)--nextfloor;
				}
				l.waittime+=getrunningtime(l.floor-nextfloor);
				l.floor=nextfloor;
			}
		}
};
/*
关于统计信息：
电梯的运行路程应该在电梯内统计
乘客等待时间不计算了，或者在外面计算（进入等待-sec，上电梯+sec，出电梯+sec），应该写进updateperson
其它没有什么要计算的，所以不应该写updatestats
*/
void solve1(const unsigned seed, std::vector<double> &cost){
	int waitpeople[13]={0};
	int s=100;
	srand(seed);
	while(s--){
		int nowfloor=rand()%(12-1)+2;
		waitpeople[nowfloor]++;
	}
	//for(int i=1;i<=12;++i)cout<<waitpeople[i]<<',';
	//cout<<endl;
	//double
	const int mpeople=13;//MAX_PEOPLE
	for(int f=12;f>1;--f){
		int times=waitpeople[f]%mpeople;
		while(times--)cost.push_back(getrunningtime(f-1)*2+mpeople/2.0);
		waitpeople[f]-=times*mpeople;
		if(waitpeople[f]>0){
			cost.push_back(getrunningtime(f-1)*2+mpeople/2.0);
			waitpeople[f-1]-=mpeople-waitpeople[f];
			waitpeople[f]=0;
		}
	}
}
double solve2(const unsigned seed){
	int waitpeople[13]={0},waitpeople1[13]={0},waitpeople2[13]={0};
	int s=100;
	srand(seed);
	while(s--){
		int nowfloor=rand()%(12-1)+2;
		waitpeople[nowfloor]++;
	}
	//for(int i=1;i<=12;++i)cout<<waitpeople[i]<<',';
	//cout<<endl;
	//分流 
	for(int i=2,j=2,k=2;i<=12;){
		waitpeople1[j++]=waitpeople[i++];
		if(i>12)break;
		waitpeople2[k++]=waitpeople[i++];
	}
	//for(int i=1;i<=12;++i)cout<<waitpeople1[i]<<',';
	//cout<<endl;
	//for(int i=1;i<=12;++i)cout<<waitpeople2[i]<<',';
	//cout<<endl;
	//danshuangceng
	const int mpeople=13;//MAX_PEOPLE
	double cost1=0, cost2=0;
	for(int f=7;f>1;--f){
		int times=waitpeople1[f]%mpeople;
		while(times--)cost1+=(getrunningtime(f-1)*2)+mpeople/2.0;
		waitpeople1[f]-=times*mpeople;
		if(waitpeople1[f]>0){
			cost1+=(getrunningtime(f-1)*2+mpeople/2.0);
			waitpeople1[f-1]-=mpeople-waitpeople1[f];
			waitpeople1[f]=0;
		}
	}
	for(int f=6;f>1;--f){
		int times=waitpeople2[f]%mpeople;
		while(times--)cost2+=(getrunningtime(f-1)*2)+mpeople/2.0;
		waitpeople2[f]-=times*mpeople;
		if(waitpeople2[f]>0){
			cost2+=(getrunningtime(f-1)*2+mpeople/2.0);
			waitpeople2[f-1]-=mpeople-waitpeople2[f];
			waitpeople2[f]=0;
		}
	}
	return (cost1>cost2?cost1:cost2);
}
int main() {
	const unsigned seed=1145;
	std::vector<double> costs;
	solve1(seed, costs);
	double cost1=0,cost2=0,cost;
	for(auto &i:costs){
		if(cost1<cost2)cost1+=i;
		else cost2+=i;
	}
	cost=(cost1>cost2?cost1:cost2);
	cout<<cost<<endl;
	cout<<solve2(seed);
	return 0;
}

