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
const int S=100;//总人数 
inline double solve1(const unsigned seed, std::vector<double> &cost){
	int waitpeople[13]={0};
	int s=S;
	srand(seed);
	while(s--){
		int nowfloor=rand()%(12-1)+2;
		waitpeople[nowfloor]++;
	}
	//for(int i=1;i<=12;++i)cout<<waitpeople[i]<<',';
	//cout<<endl;
	//double
	const int mpeople=13;//MAX_PEOPLE
	double cost1=0,cost2=0;
	for(int f=12;f>1;--f){
		int times=waitpeople[f]/mpeople;
		waitpeople[f]-=times*mpeople;
		while(times--){
			cost.push_back(getrunningtime(f-1)*2+mpeople/2.0);
			(cost1>cost2?cost2:cost1)+=getrunningtime(f-1)*2+mpeople/2.0;
		}
		if(waitpeople[f]>=mpeople)abort();//assert 
		if(waitpeople[f]>0){
			cost.push_back(getrunningtime(f-1)*2+mpeople/2.0);
			(cost1>cost2?cost2:cost1)+=getrunningtime(f-1)*2+mpeople/2.0;
			waitpeople[f-1]-=mpeople-waitpeople[f];
			waitpeople[f]=0;
		}
	}
	return (cost1<cost2?cost2:cost1);
}
inline double solve2(const unsigned seed){
	int waitpeople[13]={0},waitpeople1[13]={0},waitpeople2[13]={0};
	int s=S;
	srand(seed);
	while(s--){
		int nowfloor=rand()%(12-1)+2;
		waitpeople[nowfloor]++;
	}
	//for(int i=1;i<=12;++i)cout<<waitpeople[i]<<',';
	//cout<<endl;
	//分流 
	int j=2,k=2; 
	for(int i=2;i<=12;){
		waitpeople1[j++]=waitpeople[i++];
		if(i>12)break;
		waitpeople2[k++]=waitpeople[i++];
	}
	/*for(int i=1;i<=12;++i)cout<<waitpeople1[i]<<',';
	cout<<endl;
	for(int i=1;i<=12;++i)cout<<waitpeople2[i]<<',';
	cout<<endl;*/
	//danshuangceng
	const int mpeople=13;//MAX_PEOPLE
	double cost1=0, cost2=0;
	for(int f=7;f>1;--f){
		int times=waitpeople1[f]/mpeople;
		waitpeople1[f]-=times*mpeople;
		while(times--)cost1+=(getrunningtime12(f-1)*2)+mpeople/2.0;
		if(waitpeople1[f]>=mpeople)abort();//assert 
		if(waitpeople1[f]>0){
			cost1+=(getrunningtime(f-1)*2+mpeople/2.0);
			waitpeople1[f-1]-=mpeople-waitpeople1[f];
			waitpeople1[f]=0;
		}
	}
	for(int f=6;f>1;--f){
		int times=waitpeople2[f]/mpeople;
		waitpeople2[f]-=times*mpeople;
		while(times--)cost2+=(getrunningtime22(f-1)*2)+mpeople/2.0;
		if(waitpeople2[f]>=mpeople)abort();//assert 
		if(waitpeople2[f]>0){
			cost2+=(getrunningtime(f-1)*2+mpeople/2.0);
			waitpeople2[f-1]-=mpeople-waitpeople2[f];
			waitpeople2[f]=0;
		}
	}
	return (cost1<cost2?cost1:cost2);
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
void test2(unsigned seed){
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
double su1=0,su2=0,su3=0;
void test3(unsigned seed){
	;
}
int main() {
	unsigned maxi=32768;
	for(unsigned i=0;i<maxi;++i)//test(i);
		test2(i);
	cout<<sum0/maxi<<endl<<sum1/maxi<<endl<<sum2/maxi<<endl;
	//cout<<solve2(1134);
	return 0;
}

