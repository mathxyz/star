#include "inner.h"
#include <vector>
#include <map>
using namespace std;

class AdjacentNode {
public:
    int v, w;
    int edgeCapacity;
    int edgeFlow;

    friend class ResidualGraph;

    friend class MaxFlow;

public:
    int sourceVertex();

    int destinationVertex();

    int complementaryVertexOf(int vertex);

    int checkResidualCapacityOf(int vertex);

    void updateFlowOf(int vertex, int data);

    AdjacentNode(int v, int w, int capacity, int flow);
};


// Making data type of list of adjacent vertices and adjacency list as it is used often in program
typedef vector<AdjacentNode> AdjacentVertices;
//Map where key is the vertex and value are list of adjacent vertices
typedef map<int, AdjacentVertices> AdjacenyList;

// Gives residual graph in the form of adjacency list of map of vectors

class ResidualGraph {
public:
    int V;
    AdjacenyList residualGraph;

    ResidualGraph(int V);

    AdjacentVertices adjacent(int v);

    void addEdge(AdjacentNode e);
};

// Class used to incorporate implementation of Ford Fulkerson algorithm

class MaxFlowMinCut {
public:
    bool *marked;
    AdjacentNode *path;
    int count;
    ResidualGraph *fg;
    vector<int> augmentPath;

    int min(int x, int y);

    int max(int x, int y);

    MaxFlowMinCut(ResidualGraph *rg);

    // Module to get the two cut-sets
    vector<int> getCutSet(ResidualGraph G, int source);

    //Module to compute min cut
    void getMinCut(ResidualGraph *rg, bool path);

    // Module to compute the Augmented path for Ford Fulkerson Algorithm.
    bool hasAugmentPath(ResidualGraph rg);

    // Module to check whether there is augmented path for Capacity Scaling
    bool hasAugmentPathForWidest(ResidualGraph rg);

    // Module to implement variant of Ford Fulkerson using Capacity Scaling Algorithm
    ResidualGraph getMaxFlowWidestPath();

    //Module to print output of the final residual graph
    void writeOutput(ResidualGraph *rg);

    void getEdgeNonZero_no_s_t(const ResidualGraph *rg, Map &f);

    //Module to construct Residual Graph after every iteration.
    ResidualGraph constructResidualGraph(ResidualGraph rg, int currentFlow);

    // Calculation of max flow using ford fulkerson Algorithm
    ResidualGraph getMaxFlow();
};

// Module to read Input from the file

ResidualGraph readInput(FILE *input);

void setValue(int value_new);

void setFILE(FILE *fp_new);

int mfmc_method(const char* path1, const char* path2, Map &f_mfmc);
