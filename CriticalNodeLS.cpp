#include<bits/stdc++.h>
using namespace std;
ifstream infile1 ("graph.txt");
int main()
{
	//Read the graph
	int n,edge,i,j,k;
	infile1>>n;

	vector< vector<int> > graph;

	for(i=0; i<n; i++)
	{
		vector<int> row;
		for(j=0; j<n; j++)
		{
			infile1>>edge;
			row.push_back(edge);
		}
		graph.push_back(row);
	}

	//Find Neighbors
	vector<vector<int> > neighbors;

	for(i=0; i<graph.size(); i++)
	{
		vector<int> neighbor;

		for(j=0; j<graph[i].size(); j++)
		{
			if(graph[i][j]==1)
			{
				neighbor.push_back(j);
			}
		}
		neighbors.push_back(neighbor);
	}
	cout<<"Graph has n = "<<n<<" vertices."<<endl;

}