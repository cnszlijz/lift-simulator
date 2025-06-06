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
inline double getrunningtime22(int f){
	if(f<0)f=-f;//abs
	f=2*f;//双层 
	if(f<1)return B/2;
	//if(f<=1)return TIME_ONEFLOOR;
	return (K*f+B);
}
inline double getrunningtime12(int f){
	if(f<0)f=-f;//abs
	f=2*f-1;//双层 
	if(f<1)return B/2;
	//if(f<=1)return TIME_ONEFLOOR;
	return (K*f+B);
}
inline double getrunningtimehigh(int f, int plus){
	if(f<0)f=-f;//abs
	f+=plus;//高层 
	if(f<1)return B/2;
	//if(f<=1)return TIME_ONEFLOOR;
	return (K*f+B);
}
int S=500;//总人数 
int F=12;//总层数 
inline double solve1(const unsigned seed, std::vector<double> &cost){//计算seed下S个人F层不分流的运行时间 
	cost.clear();
	int waitpeople[F+2]={0};
	int s=S;
	srand(seed);
	while(s--){
		int nowfloor=rand()%(F-1)+2;
		waitpeople[nowfloor]++;
	}
	//for(int i=1;i<=F;++i)cout<<waitpeople[i]<<',';
	//cout<<endl;
	//double
	const int mpeople=13;//MAX_PEOPLE
	double cost1=0,cost2=0;
	for(int f=F;f>1;--f){
		int times=waitpeople[f]/mpeople;
		waitpeople[f]-=times*mpeople;
		while(times--){
			cost.push_back(getrunningtime(f-1)*2+mpeople/2.0);
			(cost1>cost2?cost2:cost1)+=getrunningtime(f-1)*2+mpeople/2.0;
		}
		if(waitpeople[f]>=mpeople)abort();//assert 
		if(waitpeople[f]>0){
			double tmp=getrunningtime(f-1)+getrunningtime(f-2)+getrunningtime(1)+mpeople/2.0;
			cost.push_back(tmp);
			(cost1>cost2?cost2:cost1)+=tmp;
			waitpeople[f-1]-=mpeople-waitpeople[f];
			waitpeople[f]=0;
		}
	}
	//return (cost1<cost2?cost2:cost1);
	return (cost1+cost2)/2.0; 
}
inline double solve2(const unsigned seed){//计算seed下S个人F层分单双层的运行时间 
	int waitpeople[F+2]={0},waitpeople1[F+2]={0},waitpeople2[F+2]={0};
	int s=S;
	srand(seed);
	while(s--){
		int nowfloor=rand()%(F-1)+2;
		waitpeople[nowfloor]++;
	}
	//分流 
	int j=2,k=2;
	for(int i=2;i<=F;){
		waitpeople1[j++]=waitpeople[i++];
		if(i>F)break;
		waitpeople2[k++]=waitpeople[i++];
	}
	//danshuangceng
	const int mpeople=13;//MAX_PEOPLE
	double cost1=0, cost2=0;
	int f=F;
	while(waitpeople1[f]==0)f--;
	//cout<<'|'<<f<<'|'; 
	for(;f>1;--f){
		int times=waitpeople1[f]/mpeople;
		waitpeople1[f]-=times*mpeople;
		while(times--)cost1+=(getrunningtime12(f-1)*2)+mpeople/2.0;
		if(waitpeople1[f]>=mpeople)abort();//assert
		if(waitpeople1[f]>0){
			double tmp=getrunningtime12(f-1)+getrunningtime12(f-2)+getrunningtime(2)+mpeople/2.0;
			cost1+=tmp;
			waitpeople1[f-1]-=mpeople-waitpeople1[f];
			waitpeople1[f]=0;
		}
	}
	f=F;
	while(waitpeople2[f]==0)f--;
	//cout<<'|'<<f<<'|'; 
	for(;f>1;--f){
		int times=waitpeople2[f]/mpeople;
		waitpeople2[f]-=times*mpeople;
		while(times--)cost2+=(getrunningtime22(f-1)*2)+mpeople/2.0;
		if(waitpeople2[f]>=mpeople)abort();//assert
		if(waitpeople2[f]>0){
			double tmp=getrunningtime22(f-1)+getrunningtime22(f-2)+getrunningtime(2)+mpeople/2.0;
			cost2+=tmp;
			waitpeople2[f-1]-=mpeople-waitpeople2[f];
			waitpeople2[f]=0;
		}
	}
	//return (cost1>cost2?cost1:cost2);
	return (cost1+cost2)/2.0;
}
double solve3(unsigned seed, int floor){//2~floor&floor+1~F
	int waitpeople[F+2]={0},waitpeople1[F+2]={0},waitpeople2[F+2]={0};
	int s=S;
	srand(seed);
	while(s--){
		int nowfloor=rand()%(F-1)+2;
		waitpeople[nowfloor]++;
	}
	//分流 
	int j=2,k=2,i=2;
	if(floor>=F)abort();
	while(j<=floor)waitpeople1[j++]=waitpeople[i++];
	while(i<=F)waitpeople2[k++]=waitpeople[i++];
	//高低层 
	const int mpeople=13;//MAX_PEOPLE
	double cost1=0, cost2=0;
	int f=F;
	while(waitpeople1[f]==0)f--;
	//cout<<'|'<<f<<'|'; 
	for(;f>1;--f){
		int times=waitpeople1[f]/mpeople;
		waitpeople1[f]-=times*mpeople;
		while(times--)cost1+=(getrunningtime12(f-1)*2)+mpeople/2.0;
		if(waitpeople1[f]>=mpeople)abort();//assert
		if(waitpeople1[f]>0){
			double tmp=getrunningtime12(f-1)+getrunningtime12(f-2)+getrunningtime(2)+mpeople/2.0;
			cost1+=tmp;
			waitpeople1[f-1]-=mpeople-waitpeople1[f];
			waitpeople1[f]=0;
		}
	}
	f=F;
	while(waitpeople2[f]==0)f--;
	//cout<<'|'<<f<<'|'; 
	for(;f>1;--f){
		int times=waitpeople2[f]/mpeople;
		waitpeople2[f]-=times*mpeople;
		while(times--)cost2+=(getrunningtime22(f-1)*2)+mpeople/2.0;
		if(waitpeople2[f]>=mpeople)abort();//assert
		if(waitpeople2[f]>0){
			double tmp=getrunningtime22(f-1)+getrunningtime22(f-2)+getrunningtime(2)+mpeople/2.0;
			cost2+=tmp;
			waitpeople2[f-1]-=mpeople-waitpeople2[f];
			waitpeople2[f]=0;
		}
	}
	//return (cost1>cost2?cost1:cost2);
	return (cost1+cost2)/2.0; 
}
void test(unsigned seed){
	std::vector<double> costs;
	solve1(seed, costs);
	double cost1=0,cost2=0,cost;
	for(auto &i:costs){
		if(cost1<cost2)cost1+=i;
		else cost2+=i;
	}
	cost=(cost1>cost2?cost1:cost2);
	cout<<cost<<',';
	cout<<solve2(seed)<<endl;
}
double sum1=0,sum2=0,sum0=0;
void test2(unsigned seed){//没分流的加到sum0和sum1，分单双层的加到sum2 
	std::vector<double> costs;
	sum0+=solve1(seed, costs);
	double cost1=0,cost2=0,cost;
	for(auto &i:costs){
		if(cost1<cost2)cost1+=i;
		else cost2+=i;
	}
	cost=(cost1>cost2?cost1:cost2);
	sum1+=cost;
	sum2+=solve2(seed);
}
std::vector<double>sum3;
void test3(unsigned seed){//没分流的去sum1，分单双层的去sum2，分高度层的去sum3。sum3应该具有F+1个元素 
	std::vector<double> costs;
	sum1+=solve1(seed, costs);
	sum2+=solve2(seed);
	for(int floor=2;floor<F;++floor)sum3[floor]+=solve3(seed, floor);
}
int main() {
	sum3.resize(F+2);
	unsigned maxi=32768;
	for(unsigned i=0;i<maxi;++i)//test(i);
		test2(i);
	cout<<sum0/maxi<<endl<<sum1/maxi<<endl<<sum2/maxi<<endl;
	sum1=sum2=0;
	for(unsigned i=0;i<maxi;++i)test3(i);
	cout<<endl<<sum0/maxi<<endl<<sum1/maxi<<endl<<sum2/maxi<<endl;
	for(int floor=2;floor<F;++floor)cout<<floor<<':'<<sum3[floor]/maxi<<endl;
	return 0;
}

