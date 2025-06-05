#include<vector>
#include<deque>
#include<iostream>
#include<cmath>
using std::cout; using std::endl; using std::cin;
const double TIME_ONEFLOOR=10.26;
const double K=2.09964, B=8.276;//time=K*f+B;
inline double getrunningtime(int f){//from a floor goto a+f or a-f floor, maybe include time of door opening and closing
	if(f<1)return B/2; 
	//if(f<=1)return TIME_ONEFLOOR;
	return K*f+B;
}
int ans[4];
double besttime;
double do_solve(int n,int h,int *lift){
	long double sumtime=0.0;
	for(int i=1;i<=h;++i){
		int minfloor=h+1;
		for(int j=0;j<n;++j){
			if(abs(i-lift[j])<minfloor)minfloor=abs(i-lift[j]);
		}
		sumtime+=getrunningtime(minfloor);
	}
	return (double)(sumtime/(h-1+1));
}
void solve(
	int n,//number of lifts
	int h//floors, start at 1 and end at h
		){
	ans[0]=ans[1]=ans[2]=0;
	int lift[3];
	besttime=1.0e20;
	for(lift[0]=1;lift[0]<h;++lift[0])
		for(lift[1]=lift[0]+1;lift[1]<=h;++lift[1]){
			if(n<=2){
				double t = do_solve(n,h,lift);
				if(t<besttime){
					besttime=t;
					for(int i=0;i<n;++i)
						ans[i]=lift[i];
				}
			} else if(n==3){
				for(lift[2]=lift[1]+1;lift[2]<=h;++lift[2]){
					double t = do_solve(n,h,lift);
					if(t<besttime){
						besttime=t;
						for(int i=0;i<n;++i)
							ans[i]=lift[i];
					}
				} 
			} else throw std::exception();
		}
}
void solve1(
	int n,//number of lifts
	int h//floors, start at 1 and end at h
		){
	ans[0]=ans[1]=ans[2]=0;
	int lift[3];
	besttime=1.0e20;
	lift[0]=1;
		for(lift[1]=lift[0]+1;lift[1]<=h;++lift[1]){
			if(n<=2){
				double t = do_solve(n,h,lift);
				if(t<besttime){
					besttime=t;
					for(int i=0;i<n;++i)
						ans[i]=lift[i];
				}
			} else if(n==3){
				for(lift[2]=lift[1]+1;lift[2]<=h;++lift[2]){
					double t = do_solve(n,h,lift);
					if(t<besttime){
						besttime=t;
						for(int i=0;i<n;++i)
							ans[i]=lift[i];
					}
				} 
			} else throw std::exception();
		}
}
int main() {
	int n;
	cin>>n;
	/*cout<<"floor,ans,"<<endl;
	for(int i=3;i<200;++i){
		//solve(n,i);
		solve1(n,i);
		cout<<i<<",";
		for(int j=0;j<n;++j)cout<<ans[j]<<",";
		//接下来可以计算损失
		//cout<<besttime;
		int lift[3]={1,1,1};
		double curtime=do_solve(n,i,lift);
		cout<<(curtime-besttime)/curtime;
		cout<<endl;
	}*/
	const int h=12;
	int lift[3];
	double ans[h+2][h+2]={0};
	for(lift[0]=1;lift[0]<h;++lift[0])
		for(lift[1]=lift[0];lift[1]<=h;++lift[1]){
			if(n<=2){
				ans[lift[0]][lift[1]]=do_solve(n,h,lift);
			} else throw std::exception();
		}
	for(lift[0]=1;lift[0]<=h;++lift[0]){
		for(lift[1]=1;lift[1]<=h;++lift[1]){
			cout<<ans[lift[1]][lift[0]]<<',';
		}
		cout<<endl;
	}
	return 0;
}

