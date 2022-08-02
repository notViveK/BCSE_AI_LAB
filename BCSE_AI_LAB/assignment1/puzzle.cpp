/*
    the following code will test the different unirformed search techniques 
    against the classic knights tour problem (Given a source cell check whether
    the knight can reach a particular target cell or not) along with some obstacles
    0 -> obstacle, the start cell cannot be obstacle
*/

#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <map>
#include <algorithm>
using namespace std;

int n; //to get the size of the n * n chessboard
//defining the knights move
vector<vector<int> > chessboard; //chessboard

int st_x, st_y, tg_x, tg_y; //to get the source and the target cell

vector<pair<int, int> > order; //to get the order of search techniques
//check whether the cell address is inside the board or not
bool canGo(int x, int y)
{
    if (x >= 0 && y >= 0 && x < n && y < n)
        return chessboard[x][y];
    return false;
}

vector<pair<int, int> > dir = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, -1}, {-2, 1}, {2, -1}};

void getPathUsingDFS(int start_x, int start_y, int target_x, int target_y, vector<pair<int, int> > &path, vector<pair<int, int> > &temp_path, vector<vector<bool> > &vis)
{
    if (!vis[start_x][start_y])
    {
        vis[start_x][start_y] = true;
        order.push_back({start_x, start_y});
        temp_path.push_back({start_x, start_y});
        if (start_x == target_x && start_y == target_y)
        {
            //target reached
            path = temp_path;
            return;
        }

        for (pair<int, int> move : dir)
        {
            int next_x = start_x + move.first;
            int next_y = start_y + move.second;
            if (canGo(next_x, next_y))
            {
                if (!vis[next_x][next_y])
                    getPathUsingDFS(next_x, next_y, target_x, target_y, path, temp_path, vis);
            }
        }
        temp_path.pop_back();
    }
}

//performing dls to check whether the goal can be reached within the given limits or not
void getPathUsingDLS(int start_x, int start_y, int target_x, int target_y, vector<pair<int, int> > &path, vector<pair<int, int> > &temp_path, vector<vector<bool> > &vis, int limit, int max_limit)
{
    if (!vis[start_x][start_y] && limit < max_limit)
    {
        temp_path.push_back({start_x, start_y});
        order.push_back({start_x, start_y});
        vis[start_x][start_y] = true;
        if (start_x == target_x && start_y == target_y)
        {
            path = temp_path;
            return;
        }
        for (pair<int, int> move : dir)
        {
            int next_x = start_x + move.first;
            int next_y = start_y + move.second;

            if (canGo(next_x, next_y))
            {
                if (!vis[next_x][next_y])
                {
                    getPathUsingDLS(next_x, next_y, target_x, target_y, path, temp_path, vis, limit + 1, max_limit);
                }
            }
        }
        temp_path.pop_back();
    }
}

//performing IDLS to get the node within the specified level limits;
void getPathUsingIDLS(int start_x, int start_y, int target_x, int target_y, int max_limit)
{
    bool flag = false;
    for (int limit = 1; limit <= max_limit; limit++)
    {
        vector<pair<int, int> > temp_path, path;
        vector<vector<bool> > vis(n + 1, vector<bool>(n + 1, false));
        getPathUsingDLS(start_x, start_y, target_x, target_y, path, temp_path, vis, 0, limit);
        if (vis[target_x][target_y])
        {
            cout << "---------path using IDLS----------------\n";
            for (pair<int, int> node : path)
            {
                cout << "{" << node.first << "," << node.second << "}"
                     << "\n";
            }
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        cout << "Target cannot be reached within the given limits\n";
    }
}

void getPathUsingBFS(int start_x, int start_y, int target_x, int target_y)
{

    if (canGo(target_x, target_y) == false)
    {
        cout << "Entered target is out of the board\n";
        return;
    }
    vector<vector<bool> > vis(n, vector<bool>(n, false));
    queue<pair<int, int> > q;
    q.push({start_x, start_y});
    vis[start_x][start_y] = true;

    map<pair<int, int>, pair<int, int> > parent;
    parent[{start_x, start_y}] = {-1, -1};

    while (!q.empty())
    {
        pair<int, int> curr_pos = q.front();
        q.pop();
        order.push_back(curr_pos);
        for (auto move : dir)
        {
            int next_pos_x = curr_pos.first + move.first;
            int next_pos_y = curr_pos.second + move.second;

            if (canGo(next_pos_x, next_pos_y))
            {
                if (!vis[next_pos_x][next_pos_y])
                {
                    parent[{next_pos_x, next_pos_y}] = {curr_pos.first, curr_pos.second};
                    vis[next_pos_x][next_pos_y] = true;
                    q.push({next_pos_x, next_pos_y});
                }
            }
        }
    }

    vector<pair<int, int> > path;
    pair<int, int> par = {target_x, target_y};

    if (!vis[target_x][target_y])
    {
        cout << "Goal Cannot be reached !";
    }
    else
    {
        while (par.first != -1 && par.second != -1)
        {
            path.push_back(par);
            par = parent[par];
        }
        reverse(path.begin(), path.end());
        cout << "------------path using BFS-------------\n";
        for (pair<int, int> node : path)
        {
            cout << "{" << node.first << "," << node.second << "}"
                 << "\n";
        }
    }
}

void getPathUsingIBS(int start_x, int start_y, int target_x, int target_y)
{

    if (canGo(target_x, target_y) == false)
    {
        cout << "Entered target is out of the board\n";
        return;
    }
    int limit;
    cout << "Enter IBS limit --> ";
    cin >> limit;
    vector<vector<bool> > vis(n, vector<bool>(n, false));
    queue<pair<int, int> > q;
    q.push({start_x, start_y});
    vis[start_x][start_y] = true;

    map<pair<int, int>, pair<int, int> > parent;
    parent[{start_x, start_y}] = {-1, -1};

    while (!q.empty())
    {
        pair<int, int> curr_pos = q.front();
        q.pop();
        order.push_back(curr_pos);
        for (int itr = 0; itr < min(limit, (int)dir.size()); ++itr)
        {
            pair<int, int> move = dir[itr];
            int next_pos_x = curr_pos.first + move.first;
            int next_pos_y = curr_pos.second + move.second;

            if (canGo(next_pos_x, next_pos_y))
            {
                if (!vis[next_pos_x][next_pos_y])
                {
                    parent[{next_pos_x, next_pos_y}] = {curr_pos.first, curr_pos.second};
                    vis[next_pos_x][next_pos_y] = true;
                    q.push({next_pos_x, next_pos_y});
                }
            }
        }
    }

    vector<pair<int, int> > path;
    pair<int, int> par = {target_x, target_y};

    if (!vis[target_x][target_y])
    {
        cout << "Goal Cannot be reached !\n";
    }
    else
    {
        while (par.first != -1 && par.second != -1)
        {
            path.push_back(par);
            par = parent[par];
        }
        reverse(path.begin(), path.end());
        cout << "------------path using IBS-------------\n";
        for (pair<int, int> node : path)
        {
            cout << "{" << node.first << "," << node.second << "}"
                 << "\n";
        }
    }
}

void getSourceTarget()
{
    cout << "Enter the start cell index (space separated i, j) --> ";
    cin >> st_x >> st_y;
    cout << "Enter the target cell index (space separated i, j) --> ";
    cin >> tg_x >> tg_y;
}
//function to print the order of search techniques;
void printOrder()
{
    cout << "---------Order-----------\n";
    for (pair<int, int> node : order)
    {
        cout << "{" << node.first << "," << node.second << "}"
             << "\n";
    }
}
//driver code to test the functionality
int main()
{
    while (1)
    {
        int ch;
        cout << "1. Enter Chess Board size\n2. BFS\n3. DFS\n4. DLS\n5. IDLS\n6. IBS\nEnter Choice :- ";
        cin >> ch;
        switch (ch)
        {
        case 1:
        {
            cout << "Enter the chess board size :- ";
            cin >> n;
            cout << "Enter the chess board with obstacles :- (0 -> obstacle)\n";
            chessboard.assign(n, vector<int>(n, 0));
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    cin >> chessboard[i][j];
                }
            }
            break;
        }
        case 2:
        {
            getSourceTarget();
            order.clear();
            getPathUsingBFS(st_x, st_y, tg_x, tg_y);
            printOrder();
            break;
        }
        case 3:
        {
            vector<vector<bool> > vis(n, vector<bool>(n, false));
            vector<pair<int, int> > temp_path, path;
            getSourceTarget();
            order.clear();
            getPathUsingDFS(st_x, st_y, tg_x, tg_y, path, temp_path, vis);
            //print the path obtained using DFS
            cout << "---------Path using DFS-------\n";
            for (pair<int, int> node : path)
            {
                cout << "{" << node.first << "," << node.second << "}\n";
            }
            printOrder();
            break;
        }
        case 4:
        {
            vector<vector<bool> > vis(n, vector<bool>(n, false));
            vector<pair<int, int> > temp_path, path;
            int max_limit;
            cout << "Enter max limit --> ";
            cin >> max_limit;
            order.clear();
            getSourceTarget();
            getPathUsingDLS(st_x, st_y, tg_x, tg_y, path, temp_path, vis, 0, max_limit);

            //print the path obtained using DFS
            if (vis[tg_x][tg_y])
            {
                cout << "---------Path using DLS-------\n";
                for (pair<int, int> node : path)
                {
                    cout << "{" << node.first << "," << node.second << "}\n";
                }
            }
            else
            {
                cout << "Target could not be reached within the given depth limits\n";
            }
            printOrder();
            break;
        }
        case 5:
        {
            getSourceTarget();
            int max_limit;
            cout << "Enter the max limit --> ";
            cin >> max_limit;
            getPathUsingIDLS(st_x, st_y, tg_x, tg_y, max_limit);

            break;
        }
        case 6:
        {
            getSourceTarget();
            order.clear();
            getPathUsingIBS(st_x, st_y, tg_x, tg_y);
            printOrder();
            break;
        }
        default:
        {
            exit(0);
        }
        }
    }
    return 0;
}