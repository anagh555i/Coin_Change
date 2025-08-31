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
        int curr=0;
        while(curr!=targetAmount){
            result.push_back(traceBack[curr]-curr);
            curr=traceBack[curr];
        }
        return result;
    }

    int findExpectedCost(int depth,int amount){
        // f(n) = g(n) + h(n) 
        int cost = depth; // g(n)
        cost += (amount + Cmax - 1)/Cmax; // h(n) = ceil(amount/Cmax);
        return cost;
    }

    int Astar(int amount){
        priority_queue<node,vector<node>,bool(*)(const node&, const node&)> pq(node::compare);
        pq.push(node(amount,0,0));

        while(!pq.empty()){
            NodesVisited++;
            node best = pq.top();
            pq.pop();
            if(best.amount==0) return best.depth; // goal state
            if(depth[best.amount]<best.depth) continue; // cuz there is some other state that does it in better depth

            for(auto c:coins){
                if(c>best.amount) continue;
                int s=best.amount-c; // new state
                if(depth[s]<=best.depth+1) continue;
                depth[s]=best.depth+1;
                traceBack[s]=best.amount;
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
    vector<int> coins={7,11,13};
    cout<<c.solution(coins,990)<<endl;
    vector<int> sol=c.traceBackSolution();
    for(auto it:sol) cout<<it<<" ";
    cout<<endl;
}