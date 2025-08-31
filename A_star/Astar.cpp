#include<iostream>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<queue>
#include<fstream>
#include<iomanip>

using namespace std;

#define INF INT_MAX
#define printSpace 16
#define TLE_LIMIT 9999999

class node{
public:
    int amount;
    int depth;
    int expectedCost; // f(n)

    node(int a,int d, int exp){
        amount=a;
        depth=d;
        expectedCost=exp;
    }
    static bool compare(const node &a,const node &b){
        if(a.expectedCost==b.expectedCost) return (a.depth+a.amount)>(b.depth+b.amount);
        return a.expectedCost>b.expectedCost;
    }
};

class CoinChangeAStar{
public:
    vector<int> coins; // sorted list of coins
    vector<int> depth;
    vector<int> traceBack;
    int Cmax;
    int NodesVisited;
    int targetAmount;
    CoinChangeAStar(){

    }

    vector<int> traceBackSolution(){
        vector<int> result;
        int curr=targetAmount;
        while(curr!=0){
            result.push_back(curr-traceBack[curr]);
            curr=traceBack[curr];
        }
        return result;
    }

    int findExpectedCost(int depth,int amount){
        // f(n) = g(n) + h(n) 
        int cost = depth; // g(n)
        if(Cmax<=0) return INT_MAX;
        else cost += (amount + Cmax - 1)/Cmax; // h(n) = ceil(amount/Cmax);
        return cost;
    }

    int Astar(int amount){
        priority_queue<node,vector<node>,bool(*)(const node&, const node&)> pq(node::compare);
        pq.push(node(amount,0,0));

        while(!pq.empty()){
            NodesVisited++;
            if(NodesVisited>=TLE_LIMIT) return -2;
            node best = pq.top();
            pq.pop();
            if(best.amount==0) return best.depth; // goal state
            if(depth[best.amount]<best.depth) continue; // cuz there is some other state that does it in better depth
            for(auto c:coins){
                if(c>best.amount) continue;
                int s=best.amount-c; // new state
                if(depth[s]<=best.depth+1) continue;
                depth[s]=best.depth+1;
                traceBack[best.amount]=s;
                pq.push(node(s,depth[s],findExpectedCost(depth[s],s)));
            }
        }

        return -1;
    }

    int solution(vector<int> &Cs, int amount){
        coins=Cs;
        depth=vector<int>(amount+1,INT_MAX);
        traceBack=vector<int>(amount+1);
        sort(coins.begin(),coins.end(),greater<int>());
        Cmax=coins[0];
        NodesVisited=0;
        targetAmount=amount;
        return Astar(amount);
    }
};

int coinChangeDP(vector<int>& coins, int amount) {
    vector<int> dp(amount+1,INT_MAX);
    dp[0]=0;
    for (int i=1;i<amount+1;i++){
        for (auto c: coins){
            if (i-c>=0 && dp[i-c]!=INT_MAX) dp[i] = min(dp[i],dp[i-c]+1);
        }
    }
    return dp[amount]==INT_MAX?-1:dp[amount];
}

int main(){
    CoinChangeAStar c;
    ofstream fout("output.txt");
    fout<<std::left;
    ifstream fin("../FinalTest.txt");
    if (!fin.is_open()) {
        cerr<<"Failed to open EdgeTestCases.txt"<<endl;
    }
    fout<<"****************************  A* ALGORITHM  ******************************"<<endl;
    fout<<"**************************************************************************"<<endl;
    double correctCount=0,wrongCount=0;
    int n;
    fout<<"Astar-Solution  Solution        Time(nodes)     Status"<<endl;
    fout<<"--------------  -----------     -----------     --------------------------"<<endl;
    for(fin>>n;n!=-1;fin>>n){
        int amount;
        fin>>amount;
        vector<int> coins(n);
        for(int i=0;i<n;i++)fin>>coins[i];
        int AstarSolution= c.solution(coins,amount);
        int dpSolution = coinChangeDP(coins,amount);
        fout<<setw(printSpace)<<AstarSolution<<setw(printSpace)<<dpSolution<<setw(printSpace)<<c.NodesVisited;
        if(AstarSolution==dpSolution){
            fout<<"Success"<<endl;
            correctCount++;
        }
        else if(AstarSolution==-2){
            fout<<"TLE"<<endl;
            wrongCount++;
        }
        else {
            fout<<"Fail"<<endl;
            wrongCount++;
        }
    }
    fout<<"**************************************************************************"<<endl;
    fout<<"**************************************************************************"<<endl;
    fout<<"Success : "<<correctCount<<endl;
    fout<<"Fail    : "<<wrongCount<<endl;
    fout<<"Accuracy: "<<((correctCount)/(correctCount+wrongCount))*100<<"%"<<endl;
}