#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
using namespace std;

vector<vector<int> > graph;
vector<bool> vis;
int vertices, edges;

vector<int> order;

//method to perform dfs
void dfs(int node, map<int, vector<int> > &paths, vector<int> &path)
{
	if (!vis[node])
	{
		vis[node] = true;
		order.push_back(node);
		path.push_back(node);
		if (paths.find(node) != paths.end())
		{
			for (auto ele : path)
				paths[node].push_back(ele);
		}
		for (auto child : graph[node])
		{
			dfs(child, paths, path);
		}
		path.pop_back();
	}
}
//method to perform bfs
void bfs(int start, map<int, vector<int> > &paths, vector<int> path)
{
	queue<int> q;
	unordered_map<int, int> par;
	par[start] = -1;
	q.push(start);
	vis[start] = true;

	while (!q.empty())
	{
		int node = q.front();
		order.push_back(node);
		q.pop();
		for (auto child : graph[node])
		{
			if (!vis[child])
			{
				vis[child] = true;
				par[child] = node;
				q.push(child);
			}
		}
	}
	for (auto itr : paths)
	{
		int parent = itr.first;
		while (parent != -1)
		{
			paths[itr.first].push_back(parent);
			parent = par[parent];
		}
		reverse(paths[itr.first].begin(), paths[itr.first].end());
	}
}

//method to implement ibs
void ibs(int start, map<int, vector<int> > &paths, vector<int> &path)
{
	int limit;
	cout << "Enter the ibs limit --> ";
	cin >> limit;

	queue<int> q;
	unordered_map<int, int> par;
	par[start] = -1;
	q.push(start);
	vis[start] = true;

	while (!q.empty())
	{
		int node = q.front();
		order.push_back(node);
		q.pop();
		for (int it = 0; it <= min(limit - 1, (int)graph[node].size() - 1); ++it)
		{
			int child = graph[node][it];
			if (!vis[child])
			{
				vis[child] = true;
				par[child] = node;
				q.push(child);
			}
		}
	}
	for (auto itr : paths)
	{
		if (vis[itr.first])
		{
			int parent = itr.first;
			while (parent != -1)
			{
				paths[itr.first].push_back(parent);
				parent = par[parent];
			}
			reverse(paths[itr.first].begin(), paths[itr.first].end());
		}
	}
}
//method to perform depth limited search
void dls(int node, map<int, vector<int> > &paths, vector<int> &path, int curr_depth, int depth_limit)
{
	if (!vis[node] && curr_depth < depth_limit)
	{
		vis[node] = true;
		order.push_back(node);
		path.push_back(node);
		if (paths.find(node) != paths.end())
		{
			for (auto ele : path)
				paths[node].push_back(ele);
		}
		for (auto child : graph[node])
		{
			dls(child, paths, path, curr_depth + 1, depth_limit);
		}
		path.pop_back();
	}
}

//method to initialising the graph
void initGraph()
{
	cout << "Enter the number of vertices --> ";
	cin >> vertices;
	cout << "Enter the number of edges --> ";
	cin >> edges;
	graph.assign(vertices + 1, vector<int>());
	while (edges--)
	{
		int u, v;
		cin >> u >> v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
}
//to initialise the tools required to apply search techniques on the graph
void initialiseUtilityTools(int &start, map<int, vector<int> > &paths, vector<int> &path)
{
	int goal, no_of_goal;
	cout << "Enter the start --> ";
	cin >> start;
	cout << "Enter the number of goal nodes --> ";
	cin >> no_of_goal;
	cout << "Enter the Goal Nodes --> ";
	while (no_of_goal--)
	{
		cin >> goal;
		paths[goal] = path;
	}
}
//method to print the paths of all goal nodes
void printPaths(map<int, vector<int> > paths)
{
	for (auto itr : paths)
	{
		if (vis[itr.first])
		{
			cout << "Goal Encountered --> " << itr.first << "\n";
			for (auto ele : paths[itr.first])
			{
				cout << ele << " ";
			}
			cout << "\n";
		}
		else
		{
			cout << itr.first << " -> Goal could not be reached\n";
		}
	}
}
//method to perform ids;
void ids()
{

	map<int, vector<int> > paths;
	vector<int> path;
	int start, max_depth;

	initialiseUtilityTools(start, paths, path);
	cout << "Enter max depth --> ";
	cin >> max_depth;

	auto checkIfAllGoalsReached = [&](vector<bool> &temp) -> bool
	{
		for (auto itr : paths)
		{
			if (!temp[itr.first])
				return false;
		}
		return true;
	};
	//performing the search operation
	vector<bool> temp(vertices + 1, false);
	for (int depth = 0; depth <= max_depth; depth++)
	{
		if (!checkIfAllGoalsReached(temp))
		{
			vis.assign(vertices + 1, false);
			dls(start, paths, path, 0, depth);
			temp = vis;
		}
	}
	printPaths(paths);
}
//utility function to perform the search operation depending on the technique passed as argument
void utilityDFS_BFS_IBS(function<void(int, map<int, vector<int> > &, vector<int> &)> searchFunction)
{
	map<int, vector<int> > paths;
	vector<int> path;
	int start;

	initialiseUtilityTools(start, paths, path);

	vis.assign(vertices + 1, false);
	searchFunction(start, paths, path);
	printPaths(paths);
}

void printOrder()
{
	cout << "Order --> ";
	for (auto node : order)
		cout << node << " ";
	cout << endl;
}
//driver function to do the same
int main()
{
	int n, m;
	while (true)
	{
		int ch;
		cout << "1. Enter Graph\n2. dfs\n3. bfs\n4. dls\n5. IDS\n6. IBS\nEnter Choice --> ";
		cin >> ch;
		switch (ch)
		{
		case 1:
			initGraph();
			break;
		case 2:
		{
			order.clear();
			utilityDFS_BFS_IBS(&dfs);
			printOrder();
			break;
		}
		case 3:
		{
			order.clear();
			utilityDFS_BFS_IBS(&bfs);
			printOrder();
			break;
		}
		case 4:
		{
			int start;
			map<int, vector<int> > paths;
			vector<int> path;

			initialiseUtilityTools(start, paths, path);

			int depth_limit;
			cout << "Enter the depth limit --> ";
			cin >> depth_limit;
			vis.assign(vertices + 1, false);
			order.clear();
			dls(start, paths, path, 0, depth_limit);
			printPaths(paths);
			printOrder();
			break;
		}
		case 5:
		{
			ids();
			break;
		}
		case 6:
		{
			order.clear();
			utilityDFS_BFS_IBS(&ibs);
			printOrder();
			break;
		}
		default:
			exit(0);
			break;
		}
	}
	return 0;
}
