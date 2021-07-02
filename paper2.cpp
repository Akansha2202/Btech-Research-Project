#include <bits/stdc++.h>
#include <unistd.h> 
using namespace std;

//global variable
int n;
int K;
vector<vector<int>> adjList;
vector<int> child;
vector<bool> visited;
vector<pair<int,float>> btw_values;

//set of nodes;
set<int> nodes;

//set of edges in the K set;
set<int> edges;

bool mysort(pair<int,float> &a,pair<int,float> &b)
{
    return a.second>b.second;
}
int get_children(int u)
{
    if(visited[u])
        return 0;

    int ans=1;
    visited[u]=true;

    for(int i=0;i<adjList[u].size();i++)
    {
        int v=adjList[u][i];
        ans+=get_children(v);
    }
    return child[u]=ans;
}
void deleteFromList(map<int, unordered_set<int> >& adj, int u)
{ 
    if (adj.find(u) == adj.end()) 
    {
        cout << "Node not present\n";
    }
    else
    {    
        adj.erase(u);
        for (auto it = adj.begin(); it != adj.end(); it++) {
            if (it->second.find(u) != it->second.end()) {
                it->second.erase(u);
            }
        }
        //cout << "node "<< u <<" deleted from the graph\n";
    }
}

map<int, unordered_set<int>> convertToMapList(vector<vector<int>> list)
{
    map<int, unordered_set<int> > adj;
    for(int i=0;i<list.size();i++)
    {
        unordered_set<int> st;
        adj[i]=st;
        for(int j=0;j<list[i].size();j++)
        {
            adj[i].insert(list[i][j]);
        }
    }
    return adj;
}
vector<vector<int>> convertToList( map<int, unordered_set<int>> adj)
{
    vector<vector<int>> list(n);
    for(auto ele=adj.begin(); ele!= adj.end(); ele++)
    {
        int u=ele->first;
        auto s=ele->second;
        for(auto stEle=s.begin(); stEle!= s.end(); stEle++)
        {
            list[u].push_back(*stEle);
        }
    }
    return list;
}
int sizeHelper(vector<bool> &visitedSize,int node,vector<vector<int>> list)
{
    if(visitedSize[node] || (edges.find(node)!=edges.end()))
    return 0;

    visitedSize[node]=true;
    int ans=1;

    for(int i=0;i<list[node].size();i++)
    {
        int t=sizeHelper(visitedSize,list[node][i],list);
        ans+=t;
    }
    return ans;
}
int getSizeofSubgraph(int node,vector<vector<int>> list)
{
    //adjacency list
    map<int, unordered_set<int>> adj;
    adj=convertToMapList(list);
    deleteFromList(adj,node);
    list=convertToList(adj);

    //get size of connected components all;
    int size=0;

    //getting the size of graph;
    vector<bool> visitedSize(n,false);
    for(int i=0;i<n;i++)
    {
        if(!visitedSize[i] && (edges.find(i)==edges.end()))
        {
            int t=sizeHelper(visitedSize,i,list);
            size+=(t*(t-1))/2;
        }
    }
    return size;
}
void printAdjList(vector<vector<int>> mat)
{
    for(int i=0;i<mat.size();i++)
    {
        cout<<i<<"->";
        for(int j=0;j<mat[i].size();j++)
        {
            cout<<mat[i][j]<<",";
        }
        cout<<endl;
    }
}

int getMinObj()
{
    pair<int,float> ans={-1,INT_MAX};

   //printAdjList(adjList);

    for(int i=0;i<n;i++)
    {
        int u=btw_values[i].first;
        int val=btw_values[i].second;

        if(nodes.find(u)!=nodes.end())
        {
            int nodesReachable=adjList[u].size();
            int child=get_children(u);
            child--;
            int cutSize=getSizeofSubgraph(u,adjList);
            float tempAns=nodesReachable-child+cutSize-val;
            //cout<<u<<" for "<<nodesReachable<<" - "<<child<<" + "<<cutSize<<" - "<<val<<" = "<<tempAns<<endl;
            
            if(tempAns<ans.second)
            {
                ans={u,tempAns};
            }
        }
        fill(visited.begin(),visited.end(),false);
    }
    //cout<<endl;
    return ans.first;
}
void deleteFromNode(int vStar)
{
    nodes.erase(vStar);
}
void addToEdges(int vStar)
{
    edges.insert(vStar);
}
int getSizeOfAdjMat(vector<vector<int>> adj)
{
    int size=0;
    for(int i=0;i<adj.size();i++)
    {
        if(adj[i].size()>0)
            size++;
    }
    return size;
}
vector<vector<int>> updateGraph(set<int> nodes)
{
    vector<vector<int>> adj(n);
    for(int i=0;i<n;i++)
    {
        if(nodes.find(i)!=nodes.end())
        {
            for(int j=0;j<adjList[i].size();j++)
            {
                if((edges.find(adjList[i][j]))==edges.end())
                    adj[i].push_back(adjList[i][j]);
            }
        }
    }
    return adj;
}
int main()
{
   //number of nodes;
   cin>>n>>K;
   adjList.resize(n,vector<int>(n));
   vector<vector<int>> adjMatrix(n,vector<int>(n));

   for(int i=0;i<n;i++)
   {
       adjList[i].clear();
   }

   //Input the adj matrix;
   for(int i=0;i<n;i++)
   {
       for(int j=0;j<n;j++)
       {
           cin>>adjMatrix[i][j];
       }
   }

   //convert it to a adj list;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(adjMatrix[i][j])
            {
                adjList[i].push_back(j);
            }
        }
    }

    //resize child vector
    child.resize(n);
    fill(child.begin(),child.end(),INT_MIN);
    visited.resize(n);
    fill(visited.begin(),visited.end(),false);

    //input the betweeness centrality
    btw_values.resize(n);

    for(int i=0;i<n;i++)
    {
        btw_values[i].first=i;
        cin>>btw_values[i].second;
    }

    sort(btw_values.begin(),btw_values.end(),mysort);

    //insert all nodes in the NODES set;
    for(int i=0;i<n;i++)
    {
        nodes.insert(i);
    }

    //set of nodes;
    // set<int> nodes;

    //getSizeOfAdjMat(adjList)>0
    cout<<"#####\n";
    while(edges.size()<K)
    {
        //update the graph
        adjList=updateGraph(nodes);

        //getting the minimum objective function
        int vStar=getMinObj();

        //deleted the node from the nodes and add to the edges graph
        deleteFromNode(vStar);
        addToEdges(vStar);
    }

    for(auto it = edges.begin(); it != edges.end(); ++it)
    {
        cout<<*it<<endl;
    }
    return 0;
}
