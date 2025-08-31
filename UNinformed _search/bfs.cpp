#include <bits/stdc++.h>
#include <iomanip>
#include <fstream>
using namespace std;

int coinChangeBFS(vector<int>& coins, int amount) {
    if (amount == 0) return 0;

    queue<pair<int,int>> q; // (remaining amount, steps)
    vector<bool> visited(amount + 1, false);

    q.push(make_pair(amount, 0));
    visited[amount] = true;

    while (!q.empty()) {
        pair<int,int> front = q.front();
        q.pop();

        int rem = front.first;
        int steps = front.second;

        if (rem == 0) return steps; // reached goal

        for (int c : coins) {
            int nxt = rem - c;
            if (nxt >= 0 && !visited[nxt]) {
                visited[nxt] = true;
                q.push(make_pair(nxt, steps + 1));
            }
        }
    }
    return -1; // no solution
}

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
    ofstream fout("output.txt");
    fout<<std::left;
    //ifstream fin("RandomTestCases.txt");
    ifstream fin("EdgeTestCases.txt");
    fout<<"**********  A* algorithm  **********"<<endl;
    double correctCount=0,wrongCount=0;
    int n;
    fout<<"Astar-Solution  DP-Solution     Status"<<endl;
    fout<<"--------------  -----------  ----------------------------------------------"<<endl;
    for(fin>>n;n!=-1;fin>>n){
        int amount;
        fin>>amount;
        vector<int> coins(n);
        for(int i=0;i<n;i++)fin>>coins[i];
        int AstarSolution= coinChangeBFS(coins,amount);
        int dpSolution = coinChangeDP(coins,amount);
        fout<<setw(16)<<AstarSolution<<setw(16)<<dpSolution;
        if(AstarSolution==dpSolution){
            fout<<"Success"<<endl;
            correctCount++;
        }
        else {
            fout<<"Fail"<<endl;
            wrongCount++;
        }
    }
    fout<<"*****************"<<endl;
    fout<<"*****************"<<endl;
    fout<<"Success : "<<correctCount<<endl;
    fout<<"Fail    : "<<wrongCount<<endl;
    fout<<"Accuracy: "<<((correctCount)/(correctCount+wrongCount))*100<<endl;
}
