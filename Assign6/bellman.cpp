#include <iostream>
#include <limits.h>
#include <string>
#include <bits/stdc++.h>
#define err()           cout<<"Erro in graph Creation\n";
#define isvalid(x)      if(x>graph->V||x<0){err();return 0;}
#define infinity        "\u221E"

using namespace std;

struct Edge{
    int src,dest,weight;
};

struct Graph{
    // V => No of Vertices;
    // E => NO of Edges
    int V,E;
    // Graph in the from of Array of edges 
    struct Edge *edge;
};


Graph * createGraph(int V,int E){
    //Create a graph with V-vertices and E-edges
    Graph * graph = new Graph;
    graph->V=V;
    graph->E=E;
    //Store the array of edge of graph
    graph->edge=new Edge[E];
    return graph;
}


void displayGraph(Graph *graph){
    int V=graph->V;
    int E=graph->E;
    for(int i=0;i<E;i++){
        cout<<"Edge ("<<++graph->edge[i].src<<" - "<<++graph->edge[i].dest<<") Weight "<<graph->edge[i].weight<<endl;
    }
}

bool bellmanFord(Graph *graph){
    int no_of_vertices=graph->V;
    int *dist=new int[no_of_vertices];
    int *pred=new int[no_of_vertices];
    //init
    for(int i=0;i<no_of_vertices;i++)dist[i]=INT_MAX,pred[i]=0;dist[0]=0;pred[0]=0;
    //
    for(int i=0;i<graph->V-1;i++){
        for(int j=0;j<graph->E;j++){
            int u=graph->edge[j].src;
            int v=graph->edge[j].dest;
            int w=graph->edge[j].weight;
            cout<<u+1<<" "<<v+1<<" "<<w<<endl;
            if(dist[u]!=INT_MAX&&dist[v]>dist[u]+w){
                dist[v]=dist[u]+w;
                pred[v]=u+1;
            }
            for(int k=0;k<no_of_vertices;k++)cout<<k+1<<"\t";cout<<endl;
            for(int k=0;k<no_of_vertices;k++)if(dist[k]==INT_MAX)cout<<infinity<<"\t";else cout<<dist[k]<<"\t";cout<<endl;
            for(int k=0;k<no_of_vertices;k++)cout<<pred[k]<<"\t";cout<<endl;
        }
    }
    // for(int i=0;i<no_of_vertices;i++)cout<<i+1<<"\t";cout<<endl;
    for(int j=0;j<graph->E;j++){
            int u=graph->edge[j].src;
            int v=graph->edge[j].dest;
            int w=graph->edge[j].weight;
            if(dist[u]!=INT_MAX&&dist[v]>dist[u]+w)return false;
    }
    return true;
}

int main(){

    #ifndef XX
        freopen("input.txt", "r", stdin); 
        freopen("output.txt", "w", stdout); 
    #endif 

    int E,V;
    // cout<<"Enter the number of Edge in graph :: ";
    cin>>E;
    // cout<<"Enter the number of Vertices in graph ::";
    cin>>V;
    Graph *graph=createGraph(V,E);

    for(int i=0;i<E;i++){
        //Source,Destination and Weight of Edge 
        int src,dest,weight;
        // cout<<"Enter the Source - Destination And Weight of Edge "<<i<<"::"<<endl;
        // cout<<"Source       :: ";
        cin>>src;isvalid(src);graph->edge[i].src=--src;
        // cout<<"Destination  :: ";
        cin>>dest;isvalid(dest);graph->edge[i].dest=--dest;
        // cout<<"Weight       :: ";
        cin>>graph->edge[i].weight;
    }
    if(!bellmanFord(graph)){
        cout<<"Graph contain neg cycle"<<endl;
    }
    displayGraph(graph);
    return 0;
}