#include<vector>
#include<deque>
#include<iostream>
#include<exception>
#include "lift.hpp"
//class lift{
	/*
	���ڱ��溯��̫���ˣ���������������ÿ�θı�״̬ʱ�ȸı䣬Ȼ������waitsec�Ľ�ֱʱ�䣬�����ڱ������״̬��waitsec���ģ�������0�˾Ϳ��Խ�����һ���ж���
	��������д��������Ҫռ��ʱ�䡣
	ÿ�ο��Բ���ʱ��update_state�����Ƿ���Ҫ�л�״̬�� 
	���������Ϊclosedoor0,opendoor1,uping2,downing3���֡����ּ�ת������Ҫʱ�䡣
	*/
	/*public:
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
};*/
class lift{//���࣬�������update_state������̳� 
	public:
		lift(int hh):h(hh),state(0) { /*������һ����allocate MAX_PEOPLE�ģ���������ûʲô�ã����ˡ�*//*���ڼӻ���*/people.reserve(MAX_PEOPLE); }
		int waittime=0;//��ֱʱ�� 
		int get_floor(){ return floor; }
		int down_person(){//����ִ���µ��ݣ������µ��ݵ������Թ�ͳ��ʱ�䣬���û���žͷ���0
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
		0:���� 
		1:���� 
		2:�� 
		4:�� 
		8:���ڹ��� 
		9:���ڿ��� 
		3/5:�������У���Ӧ�ô��� 
		*/
		//���в��� 
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
		//����Լ������n��ִ����run()��Ľ��Ӧ���ǵ����Ѿ����е���n�뵫�ǻ�û���³˿ͺ�ͳ����Ϣ��
		//�磬������run()�󣬵������е���3�룬���Ǹոտ��Ż�δ����/���ˡ�
		//�����run()�󣬼���һ��run()ǰ���������е���0�룬��0�����Ҳ��û�ϵ��ݡ� 
			random_insert_person(waitpeople, sec);
			for(auto l:L){
				if(l.waittime!=0){
					//��齩ֱʱ���Ƿ�Ϊ0
					continue;
				}
				//���³˿� 
				updatepassengers(l);
				//����ͳ����Ϣ 
				//updatestats(l);
				//������������µ���һ�롣 
				if(l.waittime==0)//���ܹ����ˡ������ȼ��һ�飬��update_state����ҲӦ�ü��һ�� 
					l.update_state();
			}
			//������н�ֱʱ��-1����ת����һ�롣 
			bool isidle=true;
			for(auto l:L){
				if(l.waittime>0){
					l.waittime -= 1;//���ܺϲ�����Ϊ�µ��Ѿ���������ҲҪ-=1
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
		//�ŵ�lift������ȥ//void (*update_state)(std::vector<lift> &L);//update lifts' state,only the lifts, such as gotofloor, not persons
		int sec;//simulator has run secs
		std::vector<std::deque<int>>waitpeople;//waitpeople[h]is the vector of people waiting at floor h, their gotofloor
		void updatepassengers(lft &l){//��l��waitpeople�������³˿� 
			//����l��down_person��try_up_person
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
		//void updatestats(lft &l){}//������l�йص�ͳ�ƣ�lӦ����L�ﲢ�ով������ı�//���� 
		int getupdowntime(int ups, int downs){
			if(ups>1)ups=1+(ups-1+1)/2;//+1����ȡ�� 
			if(downs>1)downs=1+(downs-1+1)/2;//��һ������һ��������ÿ������ 
			int persons=ups+downs;
			if(persons<=1)return 0;
			return (persons-1)*1;//ÿ��1s�����ſ��Գ�1���� 
		} 
};
/*
����ͳ����Ϣ��
���ݵ�����·��Ӧ���ڵ�����ͳ��
�˿͵ȴ�ʱ�䲻�����ˣ�������������㣨����ȴ�-sec���ϵ���+sec��������+sec����Ӧ��д��updateperson
����û��ʲôҪ����ģ����Բ�Ӧ��дupdatestats
*/
using std::cout; using std::endl;
int main() {
	simulator<lift> s(2,13);
	cout<<'1'<<endl;
	for(int i=2;i<=13;++i)s.insertoneperson(i,i-1),cout<<'a';
	for(int i=1;i<=13;++i)std::cout<<s.querywaitpeople(i)<<' ';
	return 0;
}

