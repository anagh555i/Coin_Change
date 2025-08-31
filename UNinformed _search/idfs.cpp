// #include <bits/stdc++.h>
// using namespace std;

// #define TLE_LIMIT 99999999
// #define printSpace 16

// int NodeVisited;

// // Depth-Limited Search using stack (iterative)
// bool dlsIter(int amount, int depthLimit, const vector<int>& coins) {
//     if (coins.empty()) return false; // no coins, no solution

//     stack<pair<int,int>> st;
//     st.push(make_pair(amount, depthLimit));

//     while (!st.empty()) {
//         NodeVisited++;
//         if (NodeVisited > TLE_LIMIT) return true; // timeout

//         pair<int,int> state = st.top();
//         st.pop();

//         int rem = state.first;
//         int depth = state.second;

//         if (rem == 0) return true;   // solution found
//         if (rem < 0 || depth == 0) continue;

//         // push next states
//         for (int c : coins) {
//             st.push(make_pair(rem - c, depth - 1));
//         }
//     }
//     return false;
// }

// // Iterative Deepening DFS for Coin Change
// int coinChangeIDFS(vector<int>& coins, int amount) {
//     if (amount == 0) return 0;      // no coins needed
//     if (coins.empty()) return -1;   // impossible with no coins

//     int smallest = *min_element(coins.begin(), coins.end());
//     int maxDepth = amount / smallest + 1; // safe bound

//     for (int limit = 0; limit <= maxDepth; ++limit) {
//         if (dlsIter(amount, limit, coins)) {
//             if (NodeVisited > TLE_LIMIT) return -2; // timeout
//             return limit;
//         }
//     }
//     return -1; // no solution found
// }


// int coinChangeDP(vector<int>& coins, int amount) {
//     vector<int> dp(amount+1,INT_MAX);
//     dp[0]=0;
//     for (int i=1;i<amount+1;i++){
//         for (auto c: coins){
//             if (i-c>=0 && dp[i-c]!=INT_MAX) dp[i] = min(dp[i],dp[i-c]+1);
//         }
//     }
//     return dp[amount]==INT_MAX?-1:dp[amount];
// }

// int main(){
//     ofstream fout("IDFSoutput.txt");
//     fout<<std::left;
//     ifstream fin("../FinalTest.txt");
//     if (!fin.is_open()) {
//         cerr<<"Failed to open EdgeTestCases.txt"<<endl;
//     }
//     fout<<"****************************  IDFS ALGORITHM  ******************************"<<endl;
//     fout<<"**************************************************************************"<<endl;
//     double correctCount=0,wrongCount=0;
//     int n;
//     fout<<"IDFS-Solution   Solution        Time(nodes)     Status"<<endl;
//     fout<<"--------------  -----------     -----------     --------------------------"<<endl;
//     for(fin>>n;n!=-1;fin>>n){
//         int amount;
//         fin>>amount;
//         vector<int> coins(n);
//         for(int i=0;i<n;i++)fin>>coins[i];
//         NodeVisited=0;
//         int IDFSSolution= coinChangeIDFS(coins,amount);
//         int dpSolution = coinChangeDP(coins,amount);
//         fout<<setw(printSpace)<<IDFSSolution<<setw(printSpace)<<dpSolution<<setw(printSpace)<<NodeVisited;
//         if(IDFSSolution==dpSolution){
//             fout<<"Success"<<endl;
//             correctCount++;
//         }
//         else if(IDFSSolution==-2){
//             fout<<"TLE"<<endl;
//             wrongCount++;
//         }
//         else {
//             fout<<"Fail"<<endl;
//             wrongCount++;
//         }
//     }
//     fout<<"**************************************************************************"<<endl;
//     fout<<"**************************************************************************"<<endl;
//     fout<<"Success : "<<correctCount<<endl;
//     fout<<"Fail    : "<<wrongCount<<endl;
//     fout<<"Accuracy: "<<((correctCount)/(correctCount+wrongCount))*100<<"%"<<endl;
// }

#include <bits/stdc++.h>
using namespace std;

#define TLE_LIMIT 99999999
#define printSpace 16

int NodeVisited;

enum DLSResult { NO_SOLUTION = 0, SOLUTION_FOUND = 1, TIMEOUT = -1 };

// Depth-Limited Search using stack (iterative) with timeout and zero-coin skip
DLSResult dlsIter(int amount, int depthLimit, const vector<int>& coins) {
    if (coins.empty()) return NO_SOLUTION;

    stack<pair<int,int>> st;
    st.push(make_pair(amount, depthLimit));

    while (!st.empty()) {
        NodeVisited++;
        if (NodeVisited > TLE_LIMIT) return TIMEOUT;

        auto state = st.top();
        st.pop();

        int rem = state.first;
        int depth = state.second;

        if (rem == 0) return SOLUTION_FOUND;
        if (rem < 0 || depth == 0) continue;

        for (int c : coins) {
            if (c == 0) continue;  // Skip zero coin to avoid infinite loop
            st.push(make_pair(rem - c, depth - 1));
        }
    }
    return NO_SOLUTION;
}

// Iterative Deepening DFS for Coin Change with zero-coin check
int coinChangeIDFS(const vector<int>& coins, int amount) {
    if (amount == 0) return 0;
    // Filter out zeros before min_element to avoid errors
    vector<int> filteredCoins;
    for (int c : coins) if (c != 0) filteredCoins.push_back(c);
    if (filteredCoins.empty()) return -1;

    int smallest = *min_element(filteredCoins.begin(), filteredCoins.end());
    int maxDepth = amount / smallest + 1;

    for (int limit = 0; limit <= maxDepth; ++limit) {
        DLSResult res = dlsIter(amount, limit, filteredCoins);
        if (res == SOLUTION_FOUND) return limit;
        if (res == TIMEOUT) return -2; // timeout
    }
    return -1;
}

// DP solution for Coin Change with zero-coin skip
int coinChangeDP(const vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, INT_MAX);
    dp[0] = 0;
    for (int i = 1; i <= amount; i++) {
        for (int c : coins) {
            if (c == 0) continue;  // Skip zero coin in DP too
            if (i - c >= 0 && dp[i - c] != INT_MAX)
                dp[i] = min(dp[i], dp[i - c] + 1);
        }
    }
    return dp[amount] == INT_MAX ? -1 : dp[amount];
}

int main() {
    ofstream fout("IDFSoutput.txt");
    fout << std::left;

    ifstream fin("../FinalTest.txt");
    if (!fin.is_open()) {
        cerr << "Failed to open ../FinalTest.txt" << endl;
        fout << "Failed to open ../FinalTest.txt" << endl;
        return 1;
    }

    fout << "****************************  IDFS ALGORITHM  ******************************" << endl;
    fout << "**************************************************************************" << endl;
    double correctCount = 0, wrongCount = 0;
    fout << "IDFS-Solution   Solution        Time(nodes)     Status" << endl;
    fout << "--------------  -----------     -----------     --------------------------" << endl;

    int n;
    bool anyTest = false;
    int testCaseNumber = 0;

    while (fin >> n && n != -1) {
        testCaseNumber++;
        anyTest = true;
        int amount;
        fin >> amount;
        vector<int> coins(n);
        for (int i = 0; i < n; i++) fin >> coins[i];

        NodeVisited = 0;
        //cout << "Processing test case " << testCaseNumber << ": n=" << n << ", amount=" << amount << endl;

        int IDFSSolution = coinChangeIDFS(coins, amount);
        int dpSolution = coinChangeDP(coins, amount);

        fout << setw(printSpace) << IDFSSolution
             << setw(printSpace) << dpSolution
             << setw(printSpace) << NodeVisited;

        if (IDFSSolution == dpSolution) {
            fout << "Success" << endl;
            correctCount++;
        } else if (IDFSSolution == -2) {
            fout << "TLE" << endl;
            wrongCount++;
        } else {
            fout << "Fail" << endl;
            wrongCount++;
        }
    }

    fout << "**************************************************************************" << endl;
    fout << "**************************************************************************" << endl;
    fout << "Success : " << correctCount << endl;
    fout << "Fail    : " << wrongCount << endl;

    if (correctCount + wrongCount > 0) {
        fout << "Accuracy: " << (correctCount / (correctCount + wrongCount)) * 100 << "%" << endl;
    } else {
        fout << "Accuracy: N/A (no test cases run)" << endl;
    }
    return 0;
}

