#include <bits/stdc++.h>
#include <iomanip>
#include <fstream>
using namespace std;

#define TLE_LIMIT 99999
#define printSpace 16

int NodeVisited;

struct State
{
    int amount;
    int coinsUsed;

    State(int a, int c) : amount(a), coinsUsed(c) {}
};

int coinChangeHillClimbing(vector<int> &coins, int targetAmount)
{
    if (targetAmount == 0)
        return 0;

    State currentState(targetAmount, 0);
    NodeVisited = 0;

    while (currentState.amount > 0)
    {
        NodeVisited++;
        if (NodeVisited > TLE_LIMIT)
            return -2; // TLE

        // Generate all valid neighbors
        vector<State> neighbors;
        for (int coin : coins)
        {
            if (currentState.amount >= coin)
            {
                neighbors.push_back(State(currentState.amount - coin, currentState.coinsUsed + 1));
            }
        }

        // If no valid neighbors, we're stuck (local optimum)
        if (neighbors.empty())
        {
            return -1; // No solution found
        }

        // Find the neighbor with minimum remaining amount (best evaluation)
        State bestNeighbor = neighbors[0];
        for (const State &neighbor : neighbors)
        {
            if (neighbor.amount < bestNeighbor.amount)
            {
                bestNeighbor = neighbor;
            }
        }

        // If no neighbor is better than current state, we're at local optimum
        if (bestNeighbor.amount >= currentState.amount)
        {
            return -1; // Stuck at local optimum
        }

        // Move to the best neighbor
        currentState = bestNeighbor;
    }

    return currentState.coinsUsed; // Goal reached
}

int coinChangeDP(vector<int> &coins, int amount)
{
    vector<int> dp(amount + 1, INT_MAX);
    dp[0] = 0;
    for (int i = 1; i < amount + 1; i++)
    {
        for (auto c : coins)
        {
            if (i - c >= 0 && dp[i - c] != INT_MAX)
            {
                dp[i] = min(dp[i], dp[i - c] + 1);
            }
        }
    }
    return dp[amount] == INT_MAX ? -1 : dp[amount];
}

int main()
{
    ofstream fout("HillClimbingOutput.txt");
    fout << std::left;
    ifstream fin("../FinalTest.txt");
    if (!fin.is_open())
    {
        cerr << "Failed to open FinalTest.txt" << endl;
        return 1;
    }

    fout << "****************************  HILL CLIMBING ALGORITHM  ****************************" << endl;
    fout << "**********************************************************************************" << endl;
    double correctCount = 0, wrongCount = 0;
    int n;
    fout << "HC-Solution     DP-Solution     Time(nodes)     Status" << endl;
    fout << "--------------  -----------     -----------     --------------------------" << endl;

    for (fin >> n; n != -1; fin >> n)
    {
        int amount;
        fin >> amount;
        vector<int> coins(n);
        for (int i = 0; i < n; i++)
            fin >> coins[i];

        NodeVisited = 0;
        int HCSolution = coinChangeHillClimbing(coins, amount);
        int dpSolution = coinChangeDP(coins, amount);

        fout << setw(printSpace) << HCSolution << setw(printSpace) << dpSolution << setw(printSpace) << NodeVisited;

        if (HCSolution == dpSolution && HCSolution != -1)
        {
            fout << "Success" << endl;
            correctCount++;
        }
        else if (HCSolution == -2)
        {
            fout << "TLE" << endl;
            wrongCount++;
        }
        else
        {
            fout << "Fail/Local Optimum" << endl;
            wrongCount++;
        }
    }

    fout << "**********************************************************************************" << endl;
    fout << "**********************************************************************************" << endl;
    fout << "Success : " << correctCount << endl;
    fout << "Fail    : " << wrongCount << endl;
    fout << "Accuracy: " << ((correctCount) / (correctCount + wrongCount)) * 100 << "%" << endl;

    return 0;
}