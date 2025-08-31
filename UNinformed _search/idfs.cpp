#include <bits/stdc++.h>
using namespace std;

// Depth-Limited Search using stack (iterative)
bool dlsIter(int amount, int depthLimit, const vector<int>& coins) {
    stack<pair<int,int>> st;
    st.push(make_pair(amount, depthLimit));

    while (!st.empty()) {
        pair<int,int> state = st.top();
        st.pop();

        int rem = state.first;
        int depth = state.second;

        if (rem == 0) return true;
        if (rem < 0 || depth == 0) continue;

        // push next states
        for (int c : coins) {
            st.push(make_pair(rem - c, depth - 1));
        }
    }
    return false;
}

// Iterative Deepening DFS for Coin Change
int coinChangeIDFS(vector<int>& coins, int amount) {
    if (amount == 0) return 0;
    int smallest = *min_element(coins.begin(), coins.end());
    int maxDepth = amount / smallest + 1; // safe bound

    for (int limit = 0; limit <= maxDepth; ++limit) {
        if (dlsIter(amount, limit, coins)) {
            return limit;
        }
    }
    return -1;
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
        int AstarSolution= coinChangeIDFS(coins,amount);
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

