#include <bits/stdc++.h>
using namespace std;
ifstream infile ("graph.txt");
ofstream outfile ("sets.txt");
//global variable
int n;
vector<vector<int>> adjList;

void printMIS(set<int> st)
{
    for(auto &s:st)
    {
        outfile<<s<<" ";
    }
    outfile<<endl;
}

bool isSafe(vector<int> MIS,int node)
{
    //check whether there is an edge between any value in MIS and the node 
    unordered_map<int,bool> isEdge;
    for(int i=0;i<MIS.size();i++)
    {
        isEdge[MIS[i]]=true;
    }

    for(int i=0;i<adjList[node].size();i++)
    {
        int v=adjList[node][i];
        if(isEdge.find(v)!=isEdge.end())
        {
            return false;
        }
    }
    return true;
}

void getMIS(int index,vector<int> MIS,set<int> &ans)
{
    //base case
    if(index>=n)
    {
        return;
    }

    if(MIS.size()>ans.size())
    {
        ans.clear();
        for(int i=0;i<MIS.size();i++)
        {
            ans.insert(MIS[i]);
        }
    }

    //printMIS(ans);

    for(int i=index;i<n;i++)
    {
        if(isSafe(MIS,i))
        {
            MIS.push_back(i);
            getMIS(i+1,MIS,ans);
            MIS.pop_back();
        }
    }
}
int main() {
    //number of nodes;
    
   infile>>n;
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
           infile>>adjMatrix[i][j];
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
    //UNCOMMENT FOR ANY SOME SPECIFIC STARTNODE ;

    // int startIndex;
    // cin>>startIndex;
    // MIS.push_back(startIndex);

    //get the MIS using the startIndex;
    vector<int> MIS;
    set<int> Ans;
   

    //MIS Call function
    getMIS(0,MIS,Ans);
    
    //Print MIS
    printMIS(Ans);

    return 0;
}
