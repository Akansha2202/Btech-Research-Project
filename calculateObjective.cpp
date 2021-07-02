#include<bits/stdc++.h>
using namespace std;
ifstream infile2 ("graph.txt");
ifstream infile1("finalAns.txt");
void depthFirst(int v,vector<vector<int>> graph,vector<bool>& visited, int& ans)
{
    
    visited[v] = true;
 
    // Incrementing the count of
    // connected chain length
    ans++;
 
    for (auto i : graph[v]) {
        if (visited[i] == false) {
            // Recursive call to the DFS algorithm
            depthFirst(i, graph, visited, ans);
        }
    }
}
vector<int> getSizeofConnectedComponents(int n,vector<vector<int>> graph,set<int> in_MIS)
{
	vector<bool> visited(n + 1, false);
	vector<int> sigma;

	for (int i = 0; i < n; i++) {

		if(in_MIS.find(i)==in_MIS.end())
		{
			continue;
		}
        if (visited[i] == false) {
            int ans = 0;
            // DFS algorithm
            depthFirst(i, graph, visited, ans);
            sigma.push_back(ans);
            //cout << endl<<"Count = " << ans << "\n";
        }
    }
    return sigma;
}


int getObjectiveValue(set<int> in_MIS,vector<vector<int>> neighbors,int n)
{
	//create the new temp graph
	vector<vector<int>> adjMatNew(n,vector<int>(n,0));

	for(auto u:in_MIS)
	{
		//get the vertex which is ele;
		for(int i=0;i<neighbors[u].size();i++)
		{
			int v=neighbors[u][i];
			if(in_MIS.find(v)!=in_MIS.end())
			{
				//add a 1 in the adjMatNew
				adjMatNew[u][v]=1;
				adjMatNew[v][u]=1;
			}
		}
	}


	//convert the adjMatrix in the list;
	vector<vector<int> > adjListNew;
	for(int i=0; i<adjMatNew.size(); i++)
	{
	  vector<int> row;
	  for(int j=0; j<adjMatNew[i].size(); j++)
	  if(adjMatNew[i][j]==1)
	  	row.push_back(j);
	  adjListNew.push_back(row);
	}

	//get the obj value;
	vector<int> sigmaH=getSizeofConnectedComponents(n,adjListNew,in_MIS);
	// cout<<endl;

	//Sum up all the value;
	int sum=0;
	for(int i=0;i<sigmaH.size();i++)
	{
		sum+=(sigmaH[i]*(sigmaH[i]-1))/2;
	}
	//return the value;
	return sum;
}

int main()
{
	//create the graph;
	int m,n,i,j,edge;
	//n is size of graph (no od nodes)
	
	infile2>>n;
	vector< vector<int> > graph;
	for(i=0; i<n; i++)
	{
	  vector<int> row;
	  for(j=0; j<n; j++)
	  {
	   infile2>>edge;
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
	  if(graph[i][j]==1) neighbor.push_back(j);
	  neighbors.push_back(neighbor);
	}

	set<int> ans;
	infile1>>m;

	for(i=0;i<m;i++)
	{
		int vertex;
		infile1>>vertex;
		ans.insert(vertex);
	}
	int min_ans=getObjectiveValue(ans,neighbors,n);
	cout<<min_ans<<endl;
	return 0;
}