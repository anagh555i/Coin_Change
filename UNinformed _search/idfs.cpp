#include <bits/stdc++.h>
using namespace std;

#define TLE_LIMIT 99999999
#define printSpace 16

int NodeVisited;

// Depth-Limited Search using stack (iterative)
bool dlsIter(int amount, int depthLimit, const vector<int>& coins) {
    stack<pair<int,int>> st;
    st.push(make_pair(amount, depthLimit));

    while (!st.empty()) {
        NodeVisited++;
        if(NodeVisited>TLE_LIMIT) return true;
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
            if(NodeVisited>TLE_LIMIT) return -2;
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
    ofstream fout("IDFSoutput.txt");
    fout<<std::left;
    ifstream fin("../FinalTest.txt");
    if (!fin.is_open()) {
        cerr<<"Failed to open EdgeTestCases.txt"<<endl;
    }
    fout<<"****************************  IDFS ALGORITHM  ******************************"<<endl;
    fout<<"**************************************************************************"<<endl;
    double correctCount=0,wrongCount=0;
    int n;
    fout<<"IDFS-Solution   Solution        Time(nodes)     Status"<<endl;
    fout<<"--------------  -----------     -----------     --------------------------"<<endl;
    for(fin>>n;n!=-1;fin>>n){
        int amount;
        fin>>amount;
        vector<int> coins(n);
        for(int i=0;i<n;i++)fin>>coins[i];
        NodeVisited=0;
        int IDFSSolution= coinChangeIDFS(coins,amount);
        int dpSolution = coinChangeDP(coins,amount);
        fout<<setw(printSpace)<<IDFSSolution<<setw(printSpace)<<dpSolution<<setw(printSpace)<<NodeVisited;
        if(IDFSSolution==dpSolution){
            fout<<"Success"<<endl;
            correctCount++;
        }
        else if(IDFSSolution==-2){
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
