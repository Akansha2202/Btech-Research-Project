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
vector<pair<int,int>> edgeList;
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
vector<pair<int,int>> deleteFromEdgeList(vector<pair<int,int>> edgeLis,int node_u)
{
    int N=edgeLis.size();
    int cnt=0;

    //O(N)
    for(int i=0;i<N-cnt;)
    {
        if(edgeLis[i].first==node_u || edgeLis[i].second==node_u)
        {
            swap(edgeLis[N-1-cnt],edgeLis[i]);
            cnt++;
            continue;
        }
        i++;
    }
    while(cnt--)
    {
        edgeLis.pop_back();
    }
    return edgeLis;
}

vector<vector<int>> convertEdgeListToAdjMatrix(vector<pair<int,int>> &edgeListTemp)
{
    //O(M)
    vector<vector<int>> newAdjMat(n);
    int N=edgeListTemp.size();
    for(int i=0;i<N;i++)
    {
        int u=edgeListTemp[i].first;
        int v=edgeListTemp[i].second;

        newAdjMat[u].push_back(v);
    }

    return newAdjMat;
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
int getSizeofComponentsUsingEdgeList(int node_u,vector<pair<int,int>> edgeListTemp)
{
    //O(N)
    edgeListTemp=deleteFromEdgeList(edgeListTemp,node_u);
    //O(M)
    vector<vector<int>> adjMatrixTemp=convertEdgeListToAdjMatrix(edgeListTemp);

    //get size of connected components all;
    int size=0;

    //getting the size of graph;
    vector<bool> visitedSize(n,false);
    //O((N+M)*log(N))
    for(int i=0;i<n;i++)
    {
        if(!visitedSize[i] && (edges.find(i)==edges.end()))
        {
            int t=sizeHelper(visitedSize,i,adjMatrixTemp);
            size+=(t*(t-1))/2;
        }
    }
    return size;
}

int getMinObj()
{
    pair<int,float> ans={-1,INT_MAX};

   //printAdjList(adjList);

    //O(M)
    adjList=convertEdgeListToAdjMatrix(edgeList);

    //O(N*log(N)*((N+M)+((N+M)*log(N))))
    //O(N*log(N)*(N+M)*log(N))
    for(int i=0;i<n;i++)
    {
        int u=btw_values[i].first;
        int val=btw_values[i].second;

        if(nodes.find(u)!=nodes.end())
        {
            int nodesReachable=adjList[u].size();
            int child=get_children(u);
            child--;

            //int cutSize=getSizeofSubgraph(u,adjList);


            int cutSize=getSizeofComponentsUsingEdgeList(u,edgeList);
            float tempAns=nodesReachable-child+cutSize-val;
            //cout<<u<<" for "<<nodesReachable<<" - "<<child<<" + "<<cutSize<<" - "<<val<<" = "<<tempAns<<endl;
            
            if(tempAns<ans.second)
            {
                ans={u,tempAns};
            }
        }

        //O(N)
        fill(visited.begin(),visited.end(),false);
    }
   // cout<<endl;
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
vector<pair<int,int>> updateGraphEdgeList(set<int> answer)
{
    int cnt=0;
    int N=edgeList.size();

    //O(2NlogN)+O(N)
    for(int i=0;i<N-cnt;)
    {
        int u=edgeList[i].first;
        int v=edgeList[i].second;
        if(answer.find(u)!=answer.end() || answer.find(v)!=answer.end())
        {
            swap(edgeList[N-1-cnt],edgeList[i]);
            cnt++;
            continue;
        }
        i++;
    }

    while(cnt--)
    {
        edgeList.pop_back();
    }

    return edgeList;
}

void printEdgeList(vector<pair<int,int>> &edgeList)
{
    int N=edgeList.size();
    for(int i=0;i<N;i++)
    {
        cout<<edgeList[i].first<<" "<<edgeList[i].second<<endl;
    }
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

   //Input the adj matrix O(N^2)
   for(int i=0;i<n;i++)
   {
       for(int j=0;j<n;j++)
       {
           cin>>adjMatrix[i][j];
           if(adjMatrix[i][j])
           {
               edgeList.push_back({i,j});
           }
       }
   }

    //convert it to a adj list O(N^2)
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

    //O(NlogN)
    sort(btw_values.begin(),btw_values.end(),mysort);

    //insert all nodes in the NODES set;
    for(int i=0;i<n;i++)
    {
        nodes.insert(i);
    }

    //set of nodes;
    // set<int> nodes;

    //getSizeOfAdjMat(adjList)>0

    //loop by O(K)
    while(edges.size()<K && edgeList.size()>0)
    {
        //update the graph
        //O(2NlogN)+O(N)
        edgeList=updateGraphEdgeList(edges);

        //O(N)
        //printEdgeList(edgeList);
        //getting the minimum objective function

        //O(N*log(N)*((N+M)*log(N)+N+N))
        int vStar=getMinObj();

        cout<<vStar<<endl;
        //deleted the node from the nodes and add to the edges graph

        //O(log N)
        deleteFromNode(vStar);
        //O(log N)
        addToEdges(vStar);
    }

    for(auto it = edges.begin(); it != edges.end(); ++it)
    {
        cout<<*it<<endl;
    }

    return 0;
}
