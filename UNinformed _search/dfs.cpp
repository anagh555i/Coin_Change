#include <bits/stdc++.h>
using namespace std;

#define TLE_LIMIT 99999999
#define printSpace 16

int NodeVisited;

// DFS using explicit stack (non-recursive)
int coinChangeDFS(vector<int>& coins, int amount) {
    stack<pair<int, int>> st;
    st.push(make_pair(amount, 0));

    int minCoins = INT_MAX;

    while (!st.empty()) {
        NodeVisited++;
        if(NodeVisited>TLE_LIMIT) return -2;
        pair<int, int> state = st.top();
        st.pop();

        int rem = state.first;
        int count = state.second;

        if (rem == 0) {
            minCoins = std::min(minCoins, count);
            continue;
        }

        if (rem < 0) continue;

        // Expand children
        for (int c : coins) {
            st.push(make_pair(rem - c, count + 1));
        }
    }

    return (minCoins == INT_MAX ? -1 : minCoins);
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
    ofstream fout("DFSoutput.txt");
    fout<<std::left;
    ifstream fin("../FinalTest.txt");
    if (!fin.is_open()) {
        cerr<<"Failed to open EdgeTestCases.txt"<<endl;
    }
    fout<<"****************************  DFS ALGORITHM  ******************************"<<endl;
    fout<<"**************************************************************************"<<endl;
    double correctCount=0,wrongCount=0;
    int n;
    fout<<"DFS-Solution    Solution        Time(nodes)     Status"<<endl;
    fout<<"--------------  -----------     -----------     --------------------------"<<endl;
    for(fin>>n;n!=-1;fin>>n){
        int amount;
        fin>>amount;
        vector<int> coins(n);
        for(int i=0;i<n;i++)fin>>coins[i];
        NodeVisited=0;
        int DFSSolution= coinChangeDFS(coins,amount);
        int dpSolution = coinChangeDP(coins,amount);
        fout<<setw(printSpace)<<DFSSolution<<setw(printSpace)<<dpSolution<<setw(printSpace)<<NodeVisited;
        if(DFSSolution==dpSolution){
            fout<<"Success"<<endl;
            correctCount++;
        }
        else if(DFSSolution==-2){
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