#include<vector>
#include<deque>
#include<iostream>
#include<exception>
#include<cstdlib>
#include<cstring>
#include<cstdio>
using std::cout; using std::endl; using std::cin;
struct pair{
	double a,b;
	pair(double x,double y):a(x),b(y){}
	pair():a(0.0),b(0.0){}
	pair& operator+=(const pair &t){
		a+=t.a;
		b+=t.b;
		return *this;
	}
};
const double TIME_ONEFLOOR=10.26;
const double K=2.09964, B=8.276;//time=K*f+B;
inline double getrunningtime(int f){//from a floor goto a+f or a-f floor, maybe include time of door opening and closing
	if(f<0)f=-f;//abs
	if(f<1)return B/2;
	//if(f<=1)return TIME_ONEFLOOR;
	return (K*f+B);
}
int S=500;//总人数 
int F=12;//总层数 
inline pair solve_1(const unsigned seed){//计算seed下S个人F层不分流的运行时间 
	int waitpeople[F+2]={0},cnt=0;
	srand(seed);
	memset(waitpeople,0,sizeof(waitpeople));
	int s=S;
	double cost1=0.0, cost2=0.0;
	while(s--){
		waitpeople[rand()%(F-1)+2]=1;
		cnt++;
		if(cnt>=F){
			double currtime=F/2.0;//上下 
			int floor=1;
			for(int i=2; i<=F; ++i){
				if(waitpeople[i]){
					currtime+=getrunningtime(i-floor);
					floor=i;
				}
			}
			if(floor!=1)currtime+=getrunningtime(floor-1);
			(cost1>cost2?cost2:cost1)+=currtime;
			cnt=0;
			memset(waitpeople,0,sizeof(waitpeople));
		}
	}
	if(cnt>0){
		double currtime=cnt/2.0;//上下 
		int floor=1;
		for(int i=2; i<=F; ++i){
			if(waitpeople[i]){
				currtime+=getrunningtime(i-floor);
				floor=i;
			}
		}
		if(floor!=1)currtime+=getrunningtime(floor-1);
		(cost1>cost2?cost2:cost1)+=currtime;
	}
	return pair(cost1,cost2);
}
inline double solve1(const unsigned seed){
	pair tmp=solve_1(seed);
	return (tmp.a+tmp.b)/2;
}
inline pair solve_2(const unsigned seed){//计算seed下S个人F层分单双层的运行时间 
	int waitpeople1[F+2]={0},waitpeople2[F+2]={0},cnt1=0,cnt2=0;
	srand(seed);
	int s=S;
	double cost1=0.0, cost2=0.0;
	while(s--){
		int nowfloor=rand()%(F-1)+2;
		//waitpeople[rand()%(F-1)+2];
		//cnt++;
		if(nowfloor&1){
			waitpeople1[nowfloor]=1;
			cnt1++;
			if(cnt1>=F){
				double currtime=F/2.0;//上下 
				int floor=1;
				for(int i=2; i<=F; ++i){
					if(waitpeople1[i]){
						currtime+=getrunningtime(i-floor);
						floor=i;
					}
				}
				if(floor!=1)currtime+=getrunningtime(floor-1);
				(cost1)+=currtime;
				cnt1=0;
				memset(waitpeople1,0,sizeof(waitpeople1));
			}
		} else {
			waitpeople2[nowfloor]=1;
			cnt2++;
			if(cnt2>=F){
				double currtime=F/2.0;//上下 
				int floor=1;
				for(int i=2; i<=F; ++i){
					if(waitpeople2[i]){
						currtime+=getrunningtime(i-floor);
						floor=i;
					}
				}
				if(floor!=1)currtime+=getrunningtime(floor-1);
				(cost2)+=currtime;
				cnt2=0;
				memset(waitpeople2,0,sizeof(waitpeople2));
			}
		}
	}
	if(cnt1>0){
		double currtime=cnt1/2.0;//上下 
		int floor=1;
		for(int i=2; i<=F; ++i){
			if(waitpeople1[i]){
				currtime+=getrunningtime(i-floor);
				floor=i;
			}
		}
		if(floor!=1)currtime+=getrunningtime(floor-1);
		(cost1)+=currtime;
	}
	if(cnt2>0){
		double currtime=cnt2/2.0;//上下 
		int floor=1;
		for(int i=2; i<=F; ++i){
			if(waitpeople2[i]){
				currtime+=getrunningtime(i-floor);
				floor=i;
			}
		}
		if(floor!=1)currtime+=getrunningtime(floor-1);
		(cost2)+=currtime;
	}
	return pair(cost1,cost2);
}
inline double solve2(const unsigned seed){
	pair tmp=solve_2(seed);
	return (tmp.a+tmp.b)/2;
}
inline pair solve_3(unsigned seed, const int fl){//2~fl&fl+1~F
	int waitpeople1[F+2]={0},waitpeople2[F+2]={0},cnt1=0,cnt2=0;
	srand(seed);
	int s=S;
	double cost1=0.0, cost2=0.0;
	while(s--){
		int nowfloor=rand()%(F-1)+2;
		//waitpeople[rand()%(F-1)+2];
		//cnt++;
		if(nowfloor<=fl){
			waitpeople1[nowfloor]=1;
			cnt1++;
			if(cnt1>=F){
				double currtime=F/2.0;//上下 
				int floor=1;
				for(int i=2; i<=F; ++i){
					if(waitpeople1[i]){
						currtime+=getrunningtime(i-floor);
						floor=i;
					}
				}
				if(floor!=1)currtime+=getrunningtime(floor-1);
				(cost1)+=currtime;
				cnt1=0;
				memset(waitpeople1,0,sizeof(waitpeople1));
			}
		} else {
			waitpeople2[nowfloor]=1;
			cnt2++;
			if(cnt2>=F){
				double currtime=F/2.0;//上下 
				int floor=1;
				for(int i=2; i<=F; ++i){
					if(waitpeople2[i]){
						currtime+=getrunningtime(i-floor);
						floor=i;
					}
				}
				if(floor!=1)currtime+=getrunningtime(floor-1);
				(cost2)+=currtime;
				cnt2=0;
				memset(waitpeople2,0,sizeof(waitpeople2));
			}
		}
	}
	if(cnt1>0){
		double currtime=cnt1/2.0;//上下 
		int floor=1;
		for(int i=2; i<=F; ++i){
			if(waitpeople1[i]){
				currtime+=getrunningtime(i-floor);
				floor=i;
			}
		}
		if(floor!=1)currtime+=getrunningtime(floor-1);
		(cost1)+=currtime;
	}
	if(cnt2>0){
		double currtime=cnt2/2.0;//上下 
		int floor=1;
		for(int i=2; i<=F; ++i){
			if(waitpeople2[i]){
				currtime+=getrunningtime(i-floor);
				floor=i;
			}
		}
		if(floor!=1)currtime+=getrunningtime(floor-1);
		(cost2)+=currtime;
	}
	return pair(cost1,cost2);
}
inline double solve3(const unsigned seed, const int fl){
	pair tmp=solve_3(seed, fl);
	return (tmp.a+tmp.b)/2;
}
double sum1, sum2; 
std::vector<double>sum3;
void test3(const unsigned seed){//没分流的去sum1，分单双层的去sum2，分高度层的去sum3。sum3应该具有F+1个元素 
	sum1+=solve1(seed);
	sum2+=solve2(seed);
	for(int floor=2;floor<F;++floor)sum3[floor]+=solve3(seed, floor);
}
pair s1,s2;
std::vector<pair>s3;
void test(const unsigned seed){
	s1+=solve_1(seed);
	s2+=solve_2(seed);
	for(int floor=2;floor<F;++floor)s3[floor]+=solve_3(seed, floor);
} 
int main() {
	for(F=31;F<=60;++F){
		sum3.clear();
		s3.clear();
		sum3.resize(F+2);
		s3.resize(F+2);
		unsigned maxi=32768;
		sum1=sum2=0;
		s1=s2=pair();
		//for(unsigned i=0;i<maxi;++i)test3(i);
		//cout<<endl<<sum1/maxi<<endl<<sum2/maxi<<endl;
		//for(int floor=2;floor<F;++floor)cout<<floor<<':'<<sum3[floor]/maxi<<endl;
		for(unsigned i=0;i<maxi;++i)
			test(i);
		printf("%d,层\n%.2lf,%.2lf\n%.2lf,%.2lf\n",F,s1.a/maxi,s1.b/maxi,s2.a/maxi,s2.b/maxi);
		fprintf(stderr,"%d\n",F);
		for(int i=2;i<F;++i)
			printf("%i,%.2lf,%.2lf\n",i,s3[i].a/maxi,s3[i].b/maxi), fflush(stdout);
	}
	return 0;
}

