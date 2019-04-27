// C++ program for Kruskal's algorithm to find Minimum
// Spanning Tree of a given connected, undirected and
// weighted graph
#include<bits/stdc++.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;


ofstream f("OUTPUT.txt");

long long int RandomField[10000][10000];

// function that generates a random matrix with entries 0 and 1, where 1 represents the existence of a mole hill in that space and 0 the absence of one.
void generatefield(int row, int col, long long int matrix[10000][10000])
{
    int random;
    for(int i=0; i<row; i++)
        for(int j=0; j<col; j++)
        {
            random=rand()%2;
            matrix[i][j]=random;
        }
}

//prints matrix into file
void print(int row, int col, long long int matrix[10000][10000])
{
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
            f<<matrix[i][j]<<" ";
        f<<endl;
    }
}


// Creating shortcut for an integer pair
typedef  pair<int, int> iPair;

// Structure to represent a graph
struct Graph
{
    int V, E;
    vector< pair<int, iPair> > edges;

    // Constructor
    Graph(int V, int E)
    {
        this->V = V;
        this->E = E;
    }

    // Utility function to add an edge
    void addEdge(int u, int v, int w)
    {
        edges.push_back({w, {u, v}});
    }

    // Function to find MST using Kruskal's
    // MST algorithm
    int kruskalMST();
};

// To represent Disjoint Sets
struct DisjointSets
{
    int *parent, *rnk;
    int n;

    // Constructor.
    DisjointSets(int n)
    {
        // Allocate memory
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];

        // Initially, all vertices are in
        // different sets and have rank 0.
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;

            //every element is parent of itself
            parent[i] = i;
        }
    }

    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

 /* Functions returns weight of the MST*/

int Graph::kruskalMST()
{
    int mst_wt = 0; // Initialize result

    // Sort edges in increasing order on basis of cost
    sort(edges.begin(), edges.end());

    // Create disjoint sets
    DisjointSets ds(V);

    // Iterate through all sorted edges
    vector< pair<int, iPair> >::iterator it;
    for (it=edges.begin(); it!=edges.end(); it++)
    {
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        // Check if the selected edge is creating
        // a cycle or not (Cycle is created if u
        // and v belong to same set)
        if (set_u != set_v)
        {
            // Current edge will be in the MST
            // so print it
            cout << u << " - " << v << endl;

            // Update MST weight
            mst_wt += it->first;

            // Merge two sets
            ds.merge(set_u, set_v);
        }
    }

    return mst_wt;
}


struct Edge_no{

int row_no;
int col_no;

};

// Driver program to test above functions
int main()
{


    int V=0, E, rows, cols;
    cin>>rows>>cols;

    int random;

    //introduce the number of rows and columns
    cin>>rows>>cols;
    print(rows,cols,RandomField);
    generatefield(rows, cols, RandomField);
    print(rows,cols,RandomField);


    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
            if(RandomField[i][j]==1)
                {
                    V++;
                }
    }
    f<<V<< endl;

    struct Edge_no edge_no[V]={};

    int c=0;

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
            if(RandomField[i][j]==1)
                {
                    edge_no[c].row_no=i;
                    edge_no[c].col_no=j;
                    f<<edge_no[c].row_no<<" "<<edge_no[c].col_no<<endl;
                    c++;
                }
    }


    E=V*(V-1)/2;
    f<<V<<" "<<E<<endl;
    f<<endl;

    Graph g(V, E);

    int p=0; float calc_row,calc_col,calc;

    for(int i=0; i<V-1; i++)
    {
        for(int j=i+1; j<V; j++)
            {
                if(edge_no[i].row_no>=edge_no[j].row_no)
                    calc_row=(edge_no[i].row_no-edge_no[j].row_no)^2;
                else
                    calc_row=(edge_no[j].row_no-edge_no[i].row_no)^2;
                if(edge_no[i].col_no>=edge_no[j].col_no)
                    calc_col=(edge_no[i].col_no-edge_no[j].col_no)^2;
                else
                    calc_col=(edge_no[j].col_no-edge_no[i].col_no)^2;
                calc=sqrt(calc_col+calc_row);
                g.addEdge(i, j, calc);
            }
    }


    f << "Edges of MST are \n";
    int mst_wt = g.kruskalMST();

    f << "\nWeight of MST is " << mst_wt;

    return 0;
}
