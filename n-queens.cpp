#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using std::pair;
using std::vector;

//type definitions
typedef vector<vector<int> > board;
typedef vector<int> State;


//function headers
State getRandomState(int boardSize);
//heuristic cost
long long  getCost(const State & state);
//next state generation functions
pair<State,int> getNextVanilla(const State & curr);
pair<State,int> getNextSteepest(const State & curr);
void printBoard(const State & s);
void printRow(const State & s);




//class definitions
enum class method{
	VanillaHill,
	SteepestHill
};

class HillClimbing{

public:
	HillClimbing(const State & start,const method & m,int randomRestarts):start(start),m(m),randomRestarts(randomRestarts){}

	struct info{
		info(const State & st,float time,int randomRestarts,long long ne):finalState(st),time(time),randomRestarts(randomRestarts),nodesExpanded(ne){}

		State finalState;
		float time = 0;
		int randomRestarts = 0;	
		long long nodesExpanded = 0;


	};

	info solve();



private:
	State start;
	method m;
	int randomRestarts;

};


int main(){


	using namespace std;
	cout.precision(4);
	srand(time(nullptr));



	int size = 8;
	string search;
	int randomRestarts = 0;
	method m;
	
	cout << "Enter Problem Size:";
	cin >> size;

	State start(getRandomState(size));

	cout << "Enter A for Vanilla Hill ,B for SteepestHill:";
		
	while(std::cin >> search){
		if(search == "A" || search == "B")
			break;
	}

	if(search == "A")
		m = method::VanillaHill;
	else
		m = method::SteepestHill;

	cout << "Enter number of Random Restarts allowed:";
	cin >> randomRestarts;
	


	HillClimbing h(start,m,randomRestarts);



	auto ans = h.solve();
	cout.precision(4);


	cout << "initial info" << endl;
	cout << "start state cost " << getCost(start) << endl;
	printBoard(start);


	cout << "solution info" <<endl;
	cout << "time taken: " <<  ans.time  << " sec"<< endl;
	cout << "randomRestarts taken: " <<  ans.randomRestarts << endl;
	cout << "nodes expanded: " << ans.nodesExpanded << endl;
	cout << "board config final cost: " << getCost(ans.finalState) << endl;
	printBoard(ans.finalState);


	


		




	return 0;
}




//function implementations
State getRandomState(int boardSize){

	State s(boardSize,0);

	for (int i = 0; i < boardSize; ++i)
	{
		s[i] = rand()%boardSize;
	}

	return s;
}

long long  getCost(const State & state){
	long long cost = 0;
	for(int i = 0;i<state.size();++i){
		for(int j = i+1;j<state.size();++j){
			if(state[i] == state[j] || abs(j-i) == abs(state[j]- state[i]))
				++cost;
		}
		
	}


	return cost;



}
pair<State,int> getNextVanilla(const State & curr){



	int cntr = 0;
	State ans(curr); 
	long long currCost = getCost(curr);
	for(int i = 0;i<curr.size();++i){
		for(int j = 0;j<curr.size();++j){
			if(j == curr[i])
				continue;
			else{
					ans[i] = j;
					long long  otherCost = getCost(ans);

					
					++cntr;

					if(otherCost <  currCost){

						return {ans,cntr};
					}

				}
				
		}
		ans[i] = curr[i];

	}	
	//return the original state if no next state less	
	return {curr,cntr};	
}
pair<State,int> getNextSteepest(const State & curr){


	int cntr = 0;
	State ans(curr); 
	State minState(curr);

	long long minCost = getCost(curr);

	for(int i = 0;i<curr.size();++i){
		for(int j = 0;j<curr.size();++j){
			if(j == curr[i])
				continue;
			else{

				++cntr;
				ans[i] = j;
				long long  otherCost = getCost(ans);
				if(otherCost <  minCost){
					minState = ans;
					minCost = otherCost;
					}	
				}
		}
		ans[i] = curr[i];
	}
	//return the min state(can be original as well)	
	return {minState,cntr};	
}




HillClimbing::info HillClimbing::solve(){


	const clock_t begin_time = clock();
	long long nodesExpanded = 0;
	State curr(this->start);
	State next(curr);
	int cntr = 0;
	//TODO timing here
	float time = 0;
	while(getCost(curr)!=0){

		if(this->m == method::VanillaHill){
			auto hold = getNextVanilla(curr);
			next = hold.first;
			nodesExpanded+=hold.second;
		}
		else if(this->m == method::SteepestHill){
			auto hold = getNextSteepest(curr);
			next = hold.first;
			nodesExpanded+=hold.second;

		}

		//stuck
		if(curr == next){
			if(cntr+1 > this->randomRestarts)
				break;
			else{
				curr = getRandomState(this->start.size());
				++cntr;
			}
		}
		else{
			curr = next;
		}
	
	}





	time =  float( clock () - begin_time ) /  CLOCKS_PER_SEC;
	return info(curr,time,cntr,nodesExpanded);

}



void printBoard(const State & s){

	using namespace std;

	for (int j = 0; j < s.size(); ++j)
	{
		for (int i = 0; i < s.size(); ++i)
		{
			int val = 0;
			if(j == s[i])
				val = 1;
			cout << val << " ";
		}
		cout << endl;
		
	}

}


void printRow(const State & s){

	using namespace std;

	for (int j = 0; j < s.size(); ++j)
		cout << s[j] << " ";
	cout << endl;

}
