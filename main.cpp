#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

ofstream g("Matrix.txt");

int RandomField[10000][10000], Vector_Weights[100000];

// function that generates a random matrix with entries 0 and 1, where 1 represents the existence of a mole hill in that space and 0 the absence of one.
void generatefield(int row, int col, int matrix[10000][10000])
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
void print(int row, int col, int matrix[10000][10000])
{
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
            g<<matrix[i][j]<<" ";
        g<<endl;
    }
}

// a structure that represent a weighted edge in graph
struct Edge
{
    int source, dest;
    float weight;
};

// a structure that represent a connected, undirected
// and weighted graph
// V-> Number of vertices, E-> Number of edges
struct Graph
{
    int V, E;
    struct Edge* edge;
    // graph is represented as an array of edges.
    // Since the graph is undirected, the edge from source to dest is also edge from dest to source.
    // Both are counted as 1 edge here.
};

// Creates a graph with V vertices and E edges
struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;

    graph->edge = new Edge[E];

    return graph;
}

// A structure that represent a subset for union-find
struct subset
{
    int parent;
    int rank;
};

// A utility function to find set of an element i
// (uses path compression technique)
int find(struct subset subsets[], int i)
{
    // find root and make root as parent of i
    // (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// A function that does union of two sets of x and y
// (uses union by rank)
void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of high
    // rank tree (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and
    // increment its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Compare two edges according to their weights.
// Used in qsort() for sorting an array of edges
int myComp(const void* a, const void* b)
{
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return a1->weight > b1->weight;
}



// The main function to construct MST using Kruskal's algorithm
void KruskalMST(struct Graph* graph)
{
    int V = graph->V;
    struct Edge result[V];  // This will store the resultant MST
    int e = 0;  // An index variable, used for result[]
    int i = 0;  // An index variable, used for sorted edges

    // Step 1:  Sort all the edges in non-decreasing
    // order of their weight. If we are not allowed to
    // change the given graph, we can create a copy of
    // array of edges
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp);

    // Allocate memory for creating V subsets
    struct subset *subsets =
        (struct subset*) malloc( V * sizeof(struct subset) );

    // Create V subsets with single elements
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Number of edges to be taken is equal to V-1
    while (e < V - 1)
    {
        // Step 2: Pick the smallest edge. And increment
        // the index for next iteration
        struct Edge next_edge = graph->edge[i++];

        int x = find(subsets, next_edge.source);
        int y = find(subsets, next_edge.dest);

        // If including this edge doesn't cause cycle,
        // include it in result and increment the index
        // of result for next edge
        if (x != y)
        {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
        // Else discard the next_edge
    }

    // print the contents of result[] to display the
    // built MST
    g<<"Following are the edges in the constructed MST\n";
    for (i = 0; i < e; ++i)
        g<<result[i].source<<" "<<result[i].dest<<" "<<result[i].weight<<"\n";
    return;
}




struct Vertex_no{

int row_no;
int col_no;

};

int main()
{
    int rows, cols;
    int random;

    //introduce the number of rows and columns
    cin>>rows>>cols;

    generatefield(rows, cols, RandomField);
    print(rows,cols,RandomField);
    g<<"\n";

    //count how many vertices in the graph
    int nr=0;
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
            if(RandomField[i][j]==1)
                {
                    nr++;
                }
    }
    g<< nr<<"\n";
    g<<"\n";

    //store the position in the matrix of all the vertices
    struct Vertex_no vrt_no[nr]={};

    int c=0;

    for(int k=0; k<rows; k++)
    {
        for(int l=0; l<cols; l++)
            if(RandomField[k][l]==1)
                {
                    vrt_no[c].row_no=k;
                    vrt_no[c].col_no=l;
                    g<<vrt_no[c].row_no<<" "<<vrt_no[c].col_no<<endl;
                    c++;
                }
    }


    //no of vertices and edges
    int edg=nr*(nr-1)/2;
    g<<nr<<" "<<edg<<endl;
    g<<endl;

    struct Graph* graph = createGraph(nr, edg);

    int p=0; int calc_row,calc_col,calc;

    //construct the graph with the weights and print the weight of each edge
    for(int i=0; i<nr-1; i++)
    {
        for(int j=i+1; j<nr; j++)
            {
                graph->edge[p].source=i;
                graph->edge[p].dest=j;
                if(vrt_no[i].row_no>=vrt_no[j].row_no)
                    calc_row=(vrt_no[i].row_no-vrt_no[j].row_no)^2;
                else
                    calc_row=(vrt_no[j].row_no-vrt_no[i].row_no)^2;
                if(vrt_no[i].col_no>=vrt_no[j].col_no)
                    calc_col=(vrt_no[i].col_no-vrt_no[j].col_no)^2;
                else
                    calc_col=(vrt_no[j].col_no-vrt_no[i].col_no)^2;
                calc=calc_col+calc_row;
                graph->edge[p].weight=sqrt(calc);
                g<<graph->edge[p].weight<<endl;
            }
    }

    //the final tree

    KruskalMST(graph);

    return 0;
}
