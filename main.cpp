#include<vector>
#include "lift.hpp"
class lift{
	public:
		int waitsec = 0;
		lift(const int h):H(h),floor(1),people(0){
		}
	private:
		const int H;
		const int MAX_PEOPLE=13;
		int floor;//now floor
		int people;//now people
};
class simulator{
	public:
		simulator(int n,const int h):N(n),H(h){
			while(n--){
				L.push_back(lift(h));
			}
		}
	private:
		const int N,//number of lifts
		          H;//floors, start at 1 and end at H
		std::vector<lift> L;//lifts
};
int main() {
	return 0;
}

