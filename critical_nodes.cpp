#include<bits/stdc++.h>
using namespace std;
ifstream infile1 ("graph.txt");
ifstream infile2 ("sets.txt");
ofstream outfile ("heuristic.txt");

void depthFirst(int v,vector<vector<int>> graph,vector<bool>& visited, int& ans)
{
    
    visited[v] = true;
    // cout << v << " ";
 
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
            // cout << "Count = " << ans << "\n";
        }
    }
    return sigma;
}

int getMinObjective(int n,int v,vector<vector<int>> neighbors,vector<vector<int>> adjMatNew,set<int> in_MIS)
{

	//cout<<"The value of v is "<<v<<endl;
	if(v!=-1)
	{
		set<int> isNeighbour;
		for(int i=0;i<neighbors[v].size();i++)
		{
			isNeighbour.insert(neighbors[v][i]);
		}

		set<int> intersect;
		set_intersection(in_MIS.begin(),in_MIS.end(),isNeighbour.begin(),isNeighbour.end(), inserter(intersect, intersect.begin()));


		//Updating the adj matrix


		in_MIS.insert(v);

		for(auto ele:intersect)
		{
			adjMatNew[v][ele]=1;
			adjMatNew[ele][v]=1;
		}
	}
	
	//From the adj matrix constructing the adj list
	vector<vector<int> > adjListNew;
	for(int i=0; i<adjMatNew.size(); i++)
	{
	  vector<int> row;
	  for(int j=0; j<adjMatNew[i].size(); j++)
	  if(adjMatNew[i][j]==1)
	  	row.push_back(j);
	  adjListNew.push_back(row);
	}


	vector<int> sigmaH=getSizeofConnectedComponents(n,adjListNew,in_MIS);
	// cout<<endl;

	//Sum up all the value;
	int sum=0;
	for(int i=0;i<sigmaH.size();i++)
	{
		
		sum+=(sigmaH[i]*(sigmaH[i]-1))/2;
	}

	//cout<<"For the value of "<<v<<" the sum came is "<<sum<<endl;
	return sum;
}

int main()
{
	//Read the maximal independent set;
	int MIS_size,vertex,edge,n,k;
	int i,j;
	set<int> in_MIS;
	infile2>>MIS_size;

	for(i=0;i<MIS_size;i++)
	{
		infile2>>vertex;
		in_MIS.insert(vertex);
		cout<<vertex<<",";
	}
	cout<<endl;

	//Read the graph and create the Adjacency list from Adjacency matrix;
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
	  if(graph[i][j]==1) neighbor.push_back(j);
	  neighbors.push_back(neighbor);
	}
	
	//printing the adj list
	for(i=0;i<neighbors.size();i++)
	{
		cout<<i<<" => ";
		for(j=0;j<neighbors[i].size();j++)
		{
			cout<<neighbors[i][j]<<" , ";
		}
		cout<<endl;
	}
	cout<<"Graph has n = "<<n<<" vertices."<<endl;

	cout<<"Size of MIS set is "<<in_MIS.size()<<endl;


	//create a graph from the MIS;

	cout<<"Enter the value of k \n";
	cin>>k;

	vector<vector<int>> adjNew(n,vector<int>(n,0));

	// for(i=0;i<neighbors.size();i++)
	// {
	// 	int a=i;
	// 	for(j=0;j<neighbors[i].size();j++)
	// 	{
	// 		int b=neighbors[i][j];
	// 		adjNew[a][ b]=1;
	// 	}
	// }

	int countOfLoop=0;
	while((in_MIS.size()<n-k) && (countOfLoop<1000))
	{
		//get the min i and add it;
		int add_ver;
		int min_val=INT_MAX;
		for(j=0;j<n;j++)
		{
			//checking if not in MIS;
			if(in_MIS.find(j)==in_MIS.end())
			{
				int temp_min_val=getMinObjective(n,j,neighbors,adjNew,in_MIS);
				if(min_val>temp_min_val)
				{
					min_val=temp_min_val;
					add_ver=j;
				}
			}
		}


		////Update the new graph////

		//getting all the neighbour
		set<int> isNeighbour;

		for(int i=0;i<neighbors[add_ver].size();i++)
		{
			isNeighbour.insert(neighbors[add_ver][i]);
		}

		set<int> intersect;
		set_intersection(in_MIS.begin(),in_MIS.end(),isNeighbour.begin(),isNeighbour.end(), inserter(intersect, intersect.begin()));

		//Updating the adj matrix
		for(auto ele:intersect)
		{
			adjNew[add_ver][ele]=1;
			adjNew[ele][add_ver]=1;
		}

		//add to the in_MIS
		in_MIS.insert(add_ver);
		countOfLoop++;
		cout<<"The min value came "<<min_val<<endl;
	}
	
	cout<<"The last answer is "<<getMinObjective(n,-1,neighbors,adjNew,in_MIS)<<endl;
	outfile<<in_MIS.size()<<endl;
	for(auto ele:in_MIS)
	{
		cout<<ele<<" ";
		outfile<<ele<<" ";
	}
	cout<<endl;


	return 0;
}