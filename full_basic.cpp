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
		int waittime=0;//��ֱʱ�� 
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
	void random_insert_person(std::vector<std::deque<int>> &q, int clear=0){
		//��static������ÿN���һ���� 
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
			const int n=q.size()-1;//q[0]���� 
			//int gotofloor=rand()%n+1;
			int gotofloor=1, nowfloor=rand()%(n-1)+1;
			q[nowfloor].push_back(gotofloor);//2-n¥ȥ1¥ 
		}
	}
	protected:
		std::vector<lift> L;//lifts
		//int sec;//sec�ⲿ��ʱ���ڲ�ֻ��run()������true����idle�ˡ� 
		std::vector<std::deque<int>>waitpeople;//waitpeople[h]is the vector of people waiting at floor h, their gotofloor
		void updatepassengers(lift &l){
			if(l.tag){
				l.tag=0;//��״̬���޷�֮�� 
				return;
			}
			//�Ѿ�����waittime�ˣ������ 
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
			return 0.5*sum;//�����Ÿ�3s������1��0.5s��Ŀǰ���������������� 
		} 
};
/*
����ͳ����Ϣ��
���ݵ�����·��Ӧ���ڵ�����ͳ��
�˿͵ȴ�ʱ�䲻�����ˣ�������������㣨����ȴ�-sec���ϵ���+sec��������+sec����Ӧ��д��updateperson
����û��ʲôҪ����ģ����Բ�Ӧ��дupdatestats
*/
using std::cout; using std::endl; using std::cin;
int main() {
	return 0;
}

