/*
    Author: Melike Vurucu (melikechan)
    I hope I won't get confused.

    About this file:
    - Sample code for flood fill algorithm.
*/
#include <bits/stdc++.h>

#pragma GCC optimize("O3")
#pragma GCC target("sse4")

using namespace std;

using ll = long long;
using ld = long double;

constexpr ll mod = ll(1e9) + 7;
constexpr ll inf = LLONG_MAX - ll(1e6); // think that 1e6 like calculation window, for preventing overflow
constexpr int intinf = INT_MAX - 1e3;

template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v)
{
    for (auto &i : v)
    {
        os << i << " ";
    }
    return os;
}

vector<vector<ll>> grid;
vector<vector<bool>> vis;

// Above code is my utilies for competitive programming, don't be confused, solution code is below.
/*
    In order to get the votes,
    1) Hüseyin Burak has to get the maximal amount of votes
    2) Maximal number of votes shouldn't have any ties.
*/

// Island processing (Flood Fill)
void flood_fill(int x, int y, int n, int m, vector<vector<bool>> &vis, vector<vector<int>> &adj, vector<vector<int>> &total_votes)
{
    if (x < 0 || x >= n || y < 0 || y >= m || vis[x][y] || !adj[x][y])
    {
        return;
    }
    vis[x][y] = true;
    total_votes.back()[adj[x][y]]++;
    dfs(x + 1, y, n, m, vis, adj, total_votes);
    dfs(x - 1, y, n, m, vis, adj, total_votes);
    dfs(x, y + 1, n, m, vis, adj, total_votes);
    dfs(x, y - 1, n, m, vis, adj, total_votes);
}

void solve()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> total_votes;
    vector<vector<int>> grid(n, vector<int>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int x;
            cin >> x;
            grid[i][j] = x; // 0 is water, 1-9 are citizens.
        }
    }

    vector<vector<bool>> vis(n, vector<bool>(m, false));
    // Get the islands.
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (!grid[i][j] || vis[i][j])
            {
                continue;
            }
            vector<int> temp(10);
            total_votes.push_back(temp);
            dfs(i, j, n, m, vis, grid, total_votes);
        }
    }

    int total_citizens = 0;
    for (int i = 0; i < total_votes.size(); i++)
    {
        for (int j = 1; j < 10; j++)
        {
            total_citizens += total_votes[i][j];
        }
    }

    /*
        This problem can be reduced into knapsack.
        In classical knapsack problem, we try to maximize the value of the knapsack.
        However, our goal in this problem is to minimize the cost of the knapsack. (Minimum amount of coins needed to get the corresponding amount of votes)
        Be aware that this problem is not exactly a knapsack problem, but we use a SIMILAR APPROACH!
    */
    vector<int> dp(total_citizens / 2 + 2, intinf);
    dp[0] = 0; // Hüseyin Burak can have 0 votes without spending any coin.

    // To get the votes, Melike now has to spend coins.
    for (int i = 0; i < total_votes.size(); i++)
    {
        int husejin_votes = total_votes[i][1];
        int needed_max = 0;
        int island_size = husejin_votes; // Total amount of votes in the island.

        // In the above priority queue, we keep track of the votes that are greater than or equal to Hüseyin Burak's votes.
        priority_queue<int> pq;

        for (int j = 2; j < 10; j++)
        {
            island_size += total_votes[i][j];
            if (total_votes[i][j] >= husejin_votes)
            {
                pq.push(total_votes[i][j]);
            }
        }

        // Getting minimum amount of coins needed to get the votes.
        while (!pq.empty() && pq.top() >= husejin_votes)
        {
            int x = pq.top();
            pq.pop();
            x--;
            needed_max++;
            husejin_votes++;
            pq.push(x);
        }

        /*
            Getting minimal amount of coins needed to get the votes.
            Island size may be greater than total amount of votes needed, thus 0 is our bare minimum.
        */
        for (int j = dp.size() - 1; j >= 0; j--)
        {
            dp[j] = min(dp[j], dp[max(0, j - island_size)] + needed_max);
        }
    }

    // To win the election, you have to get more than half of the votes.
    cout << dp[total_citizens / 2 + 1] << "\n";
}

int main(void)
{
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(NULL);

    int t;
    cin >> t;
    while (t--)
        solve();
    return 0;
}