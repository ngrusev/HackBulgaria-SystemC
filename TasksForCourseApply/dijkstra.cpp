#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
#include <cstdlib>

using namespace std;

typedef unsigned int uint;

struct NodeInfo
{
	uint vertex;
	uint distance;

	NodeInfo(uint vertex, uint distance) : vertex(vertex), distance(distance)
	{
	}

	bool operator < (NodeInfo other) const
	{
		return distance > other.distance;
	}
};

uint n, m, a, b;
vector<vector<NodeInfo>> graph;

void input()
{
	scanf("%u %u", &n, &m);

	for(int i = 0; i < n; i++)
	{
		graph.push_back(vector<NodeInfo>());
	}

	for (int i = 0; i < m; i++)
	{
		uint v1, v2, w;
		scanf("%u %u %u", &v1, &v2, &w);

		graph[v1].push_back(NodeInfo(v2, w));
		graph[v2].push_back(NodeInfo(v1, w));
	}

	scanf("%u %u", &a, &b);
}

void *alloc_and_init(uint size, int value)
{
	void *result = malloc(size);
	memset(result, value, size);
	return result;
}

uint solve()
{
	priority_queue<NodeInfo> queue;

	bool *visited = (bool *)alloc_and_init(sizeof(bool) * n, 0);
	uint *distance = (uint *)alloc_and_init(sizeof(uint) * n, -1);

	distance[a] = 0;
	queue.push(NodeInfo(a, 0));

	while(queue.size())
	{
		NodeInfo current = queue.top();
		queue.pop();
		visited[current.vertex] = true;

		for (int i = 0; i < graph[current.vertex].size(); i++)
		{
			NodeInfo neighbour = graph[current.vertex][i];
			if(!visited[neighbour.vertex] && distance[neighbour.vertex] > current.distance + neighbour.distance)
			{
				distance[neighbour.vertex] = current.distance + neighbour.distance;
				queue.push(NodeInfo(neighbour.vertex, distance[neighbour.vertex]));
			}
		}

		while(queue.size() && visited[queue.top().vertex])
		{
			queue.pop();
		}
	}

	uint result = distance[b];

	free(visited);
	free(distance);

	return result;
}

int main()
{
	input();
	int solution = (int)solve();

	printf(solution == -1 ? "%d" : "%u", solution);
	printf("\n");
	return 0;
}