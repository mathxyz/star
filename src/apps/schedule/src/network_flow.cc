#include <cstdlib>
#include <iostream>
#include <fstream>
#include <deque>
#include <limits>
#include <time.h>

#include "network_flow.h"

using namespace std;
int k;
int source, destination;
int nV;
int nE;
int delta;
int value = 0;
FILE *fp = 0;

//This class contains the Object of adjacent node of the particular vertex
// args are source, destination, edge capacity, and edge flow

AdjacentNode::AdjacentNode(int v, int w, int capacity, int flow) {
    this->v = v;
    this->w = w;
    this->edgeCapacity = capacity;
    this->edgeFlow = flow;
}

int AdjacentNode::sourceVertex() {
    return v;
}

int AdjacentNode::destinationVertex() {
    return w;
}

int AdjacentNode::complementaryVertexOf(int vertex) {
    if (vertex == this->v) return w;
    else if (vertex == this->w) return v;
    assert(0);
    return 0;
}

int AdjacentNode::checkResidualCapacityOf(int vertex) {
    // cout<<"Inside Residual capacity\n";
    if (vertex == this->v) {
        return this->edgeFlow;
    } else if (vertex == this->w) {
        //   cout<<capacity<<" "<<flow;
        int res = this->edgeCapacity - edgeFlow;
        // cout<<"Residual capacity:"<<res<<"\n";
        return res;
    }
    assert(0);
    return 0;
}
// Module to update flow of either the reverse edge or forward edge of corresponding vertex

void AdjacentNode::updateFlowOf(int vertex, int data) {
    //cout<<"Inside Residual flow\n";
    if (vertex == this->v)
        edgeFlow -= data;
    else if (vertex == this->w)
        edgeFlow += data;
}

ResidualGraph::ResidualGraph(int V) {
    this->V = V;
    //cout<<"Inside flow network\n";
}

AdjacentVertices ResidualGraph::adjacent(int v) {
    map<int, AdjacentVertices>::iterator it = residualGraph.find(v);
    if (it != residualGraph.end()) {
        return it->second;
    }
    assert(0);
    return it->second;
}

void ResidualGraph::addEdge(AdjacentNode e) {
    //e.flow = 0;
    int v = e.sourceVertex();
    //cout<<"Inside add edge\n";
    int w = e.destinationVertex();
    if (residualGraph.empty()) {
        AdjacentVertices adjacentVertexList;
        adjacentVertexList.push_back(e);
        residualGraph.insert(AdjacenyList::value_type(v, adjacentVertexList));
        residualGraph.insert(AdjacenyList::value_type(w, adjacentVertexList));
        // cout<<"Inside empty";
    } else {
        if (residualGraph.find(v) == residualGraph.end()) {
            AdjacentVertices adjacentVertexList;
            adjacentVertexList.push_back(e);
            residualGraph.insert(AdjacenyList::value_type(v, adjacentVertexList));
            //   cout<<"Inside v first time";

        } else {
            map<int, AdjacentVertices>::iterator it = residualGraph.find(v);
            if (it != residualGraph.end()) {
                //                    flows f = it->second;
                //                    f.push_back(e);
                //                mapGraph.insert(map_test::value_type(v, f));
                residualGraph[v].push_back(e);
            }
        }

        if (residualGraph.find(w) == residualGraph.end()) {
            AdjacentVertices adjacentVertexList;
            adjacentVertexList.push_back(e);
            residualGraph.insert(AdjacenyList::value_type(w, adjacentVertexList));
        } else {
            map<int, AdjacentVertices>::iterator it = residualGraph.find(w);
            if (it != residualGraph.end()) {
                //                    flows f = it->second;
                //                    f.push_back(e);
                //                mapGraph.insert(map_test::value_type(w, f));
                residualGraph[w].push_back(e);
            }
        }
    }

}


int MaxFlowMinCut::min(int x, int y) {
    return x < y ? x : y;
}

int MaxFlowMinCut::max(int x, int y) {
    return x > y ? x : y;
}

MaxFlowMinCut::MaxFlowMinCut(ResidualGraph *rg) {
    this->fg = rg;
}
// Module to get the two cut-sets

vector<int> MaxFlowMinCut::getCutSet(ResidualGraph G, int source) {
    marked = (bool *) malloc(sizeof(bool) * (nV + 1));
    for (int i = 1; i <= nV; i++) {
        marked[i] = false;
    }
    vector<int> cutSet;
    //FlowEdge tempEdge[100];
    path = (AdjacentNode *) malloc(sizeof(AdjacentNode) * (nV + 1));
    std::deque<int> bfsQueue;
    bfsQueue.push_front(source);
    marked[source] = true;
    while (!bfsQueue.empty()) {
        int v = bfsQueue.back();
        bfsQueue.pop_back();
        AdjacentVertices f = G.adjacent(v);
        for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
            AdjacentNode e = *it;
            int w = e.complementaryVertexOf(v);
            if (e.checkResidualCapacityOf(w) > 0 && !marked[w]) {
                if (w == source || w == destination) {
                    //do nothing
                } else {
                    cutSet.push_back(w);
                }
                marked[w] = true;
                bfsQueue.push_front(w);
            }
        }
    }
    return cutSet;
}

ResidualGraph MaxFlowMinCut::getMaxFlowWidestPath() {
    delta = 1;
    count = 0;
    ResidualGraph rg = *fg;
    int maxCapacity = 0, cap = 0;
    AdjacentVertices f = rg.adjacent(source);
    for (int i = 0; i < f.size(); i++) {
        AdjacentNode fl = f[i];
        int v = fl.complementaryVertexOf(source);
        int w = fl.edgeCapacity;
        //cout << w << " " << " max" << maxCapacity << " " << v << endl;
        cap = max(cap, w);
        maxCapacity = maxCapacity + w;
    }

    // cout << "max capacity is " << maxCapacity << endl;
    // cout << "C is " << cap << endl;
    if (cap > 1) {
        while (delta < cap) {
            delta <<= 1;
        }
        if (delta > cap) {
            delta >>= 1;
        }
        // cout << "delta:" << delta << endl;
    } else if (cap == 1) {
        delta = 1;
    } else {
        exit(1);
    }
    while (delta >= 1) {

        while (hasAugmentPathForWidest(rg)) {
            //    cout << "Max Flow Inside while\n";
            int flowCurrent = std::numeric_limits<int>::max();
            for (int v = destination; v != source; v = path[v].complementaryVertexOf(v)) {
                flowCurrent = min(flowCurrent, path[v].checkResidualCapacityOf(v));
                //      cout << "vertex:" << v << " bottleneck:" << flowCurrent << endl;
            }
            rg = constructResidualGraph(rg, flowCurrent);
            value += flowCurrent;
            count++;
        }
        delta = delta / 2;
    }
    //cout << "Total Number of Augmented Paths Needed:" << count << endl;
    fprintf(fp, "Total Number of Augmented Paths Needed:\n");
    fprintf(fp, "%d\n", count);

    //cout << "Flow value:" << value << endl;
    fprintf(fp, "Flow Value:\n");
    fprintf(fp, "%d\n", value);
    //  time = clock() - time;
    return rg;
}
//Module to compute min cut

void MaxFlowMinCut::getMinCut(ResidualGraph *rg, bool path) {
    ResidualGraph G = *rg;
    //cout << "The min cut capacity is:" << value << endl;
    fprintf(fp, "Min-Cut capacity:\n");
    fprintf(fp, "%d\n", value);
    vector<int> S, T;
    bool flag = false;
    if (path == false) {
        S = getCutSet(G, source);
        S.push_back(source);
        for (int i = 0; i < S.size(); i++) {
            //cout << S[i] << "\t";
            fprintf(fp, "%d\t", S[i]);
        }
        //cout << "\n";
        fprintf(fp, "\n");

        T = getCutSet(G, destination);
        T.push_back(destination);
        for (int i = 0; i < T.size(); i++) {
            flag = false;
            for (int j = 0; j < S.size(); j++) {
                if (T[i] == S[j]) {
                    flag = true;
                }
            }
            if (flag == false) {
                //cout << T[i] << "\t";
                fprintf(fp, "%d\t", T[i]);
            }

        }
        fprintf(fp, "\n");

        //cout << endl << endl;
    }

}
// Module to compute the Augmented path for Ford Fulkerson Algorithm.

bool MaxFlowMinCut::hasAugmentPath(ResidualGraph rg) {
    //cout<<"Inside Augment path\n";
    marked = (bool *) malloc(sizeof(bool) * (nV + 1));
    for (int i = 1; i <= nV; i++) {
        marked[i] = false;
    }

    //FlowEdge tempEdge[100];
    path = (AdjacentNode *) malloc(sizeof(AdjacentNode) * (nV + 1));
    std::deque<int> adjVertices;
    adjVertices.push_front(source);
    marked[source] = true;
    while (!adjVertices.empty()) {
        int v = adjVertices.back();
        // cout<<"From vertex:"<<v<<"\n";
        adjVertices.pop_back();
        AdjacentVertices f = rg.adjacent(v);
        //cout<<"Size:"<<f.size()<<"\n";
        for (std::vector<AdjacentNode>::iterator edge = f.begin(); edge != f.end(); ++edge) {
            AdjacentNode e = *edge;
            int w = e.complementaryVertexOf(v);
            // cout<<"To vertex delta value:"<<w<<"\n";
            if (e.checkResidualCapacityOf(w) > 0 && !marked[w]) {
                path[w] = e;
                marked[w] = true;
                adjVertices.push_front(w);
            }
        }
    }
    if (count + 1 == k) {
        int i = 0;

        //cout << "The path at iteration " << k << " is\n";
        fprintf(fp, "The path at iteration %d is\n", k);

        for (int v = destination; v != source; v = path[v].complementaryVertexOf(v)) {
            augmentPath.push_back(v);
            i++;
            //       cout<<v<<"\t";
        }
        augmentPath.push_back(source);
        for (int j = augmentPath.size() - 1; j >= 0; j--) {
            //cout << augmentPath[j] << "\t";
            fprintf(fp, "%d\t", augmentPath[j]);
        }


        fprintf(fp, "\n");
        //cout << "\n";
    }
    return marked[destination];
}

// Module to check whether there is augmented path for Capacity Scaling

bool MaxFlowMinCut::hasAugmentPathForWidest(ResidualGraph rg) {
    //cout<<"Inside Augment path\n";
    marked = (bool *) malloc(sizeof(bool) * (nV + 1));
    for (int i = 1; i <= nV; i++) {
        marked[i] = false;
    }

    //FlowEdge tempEdge[100];
    path = (AdjacentNode *) malloc(sizeof(AdjacentNode) * (nV + 1));
    std::deque<int> adjVertices;
    adjVertices.push_front(source);
    marked[source] = true;
    while (!adjVertices.empty()) {
        int v = adjVertices.back();
        // cout<<"From vertex:"<<v<<"\n";
        adjVertices.pop_back();
        AdjacentVertices f = rg.adjacent(v);
        //cout<<"Size:"<<f.size()<<"\n";
        for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
            AdjacentNode e = *it;
            int w = e.complementaryVertexOf(v);
            // cout<<"To vertex delta value:"<<w<<"\n";
            if (e.checkResidualCapacityOf(w) >= delta && !marked[w]) {
                path[w] = e;
                marked[w] = true;
                adjVertices.push_front(w);
            }
        }
    }
    return marked[destination];
}

//Module to print output of the final residual graph

void MaxFlowMinCut::writeOutput(ResidualGraph *rg) {
    ResidualGraph G = *rg;
//	cout << "The max-flow is:" << endl;
    for (int v = 0; v <= nV; v++) {
        map<int, AdjacentVertices>::iterator iter = G.residualGraph.find(v);
        if (iter != G.residualGraph.end()) {
            AdjacentVertices f = G.residualGraph[v];
            for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
                if ((*it).v == v) {
//					cout << (*it).v << "\t" << (*it).w << "\t" << (*it).edgeFlow << endl;
                    fprintf(fp, "%d\t%d\t%d\n", (*it).v, (*it).w, (*it).edgeFlow);
                }
            }
        }
    }
    //cout << endl;
}

void MaxFlowMinCut::getEdgeNonZero_no_s_t(const ResidualGraph *rg, Map &f_mfmc) {
    ResidualGraph G = *rg;
    for (int v = 0; v <= nV; v++) {
        map<int, AdjacentVertices>::iterator iter = G.residualGraph.find(v);
        if (iter != G.residualGraph.end()) {
            AdjacentVertices f = G.residualGraph[v];
            for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
                if ((*it).v == v) {
                    if ((*it).v != nV - 1 && (*it).w != nV && (*it).edgeFlow == 1) {
                        f_mfmc.add((NumN) (*it).v, (NumN) (*it).w);
                    }
                }
            }
        }
    }
}

//Module to construct Residual Graph after every iteration.

ResidualGraph MaxFlowMinCut::constructResidualGraph(ResidualGraph rg, int currentFlow) {
    for (int v = destination; v != source; v = path[v].complementaryVertexOf(v)) {
        int w = path[v].complementaryVertexOf(v);
        map<int, AdjacentVertices>::iterator it = (rg).residualGraph.find(v);
        if (it != (rg).residualGraph.end()) {
            AdjacentVertices f = (rg).residualGraph[v];
            for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
                if ((*it).v == w) {
                    (*it).updateFlowOf(v, currentFlow);
                }
            }

            rg.residualGraph[v] = f;
        }
        it = rg.residualGraph.find(w);
        if (it != rg.residualGraph.end()) {
            AdjacentVertices f = rg.residualGraph[w];
            for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
                if ((*it).w == v) {
                    (*it).updateFlowOf(v, currentFlow);
                }
            }
            rg.residualGraph[w] = f;
        }
    }
    return rg;
}

// Calculation of max flow using ford fulkerson Algorithm

ResidualGraph MaxFlowMinCut::getMaxFlow() {
    //cout<<"Max Flow Inside\n";

    ResidualGraph rg = *fg;
    value = 0;
    count = 0;
    delta = 0;
    bool isPath;
    while ((isPath = hasAugmentPath(rg)) != false) {
        // cout<<"Max Flow Inside while\n";

        int flowCurrent = std::numeric_limits<int>::max();
        for (int v = destination; v != source; v = path[v].complementaryVertexOf(v)) {
            flowCurrent = min(flowCurrent, path[v].checkResidualCapacityOf(v));
            //     cout<<"vertex:"<<v<<" bottleneck:"<<bottle;
        }
        rg = constructResidualGraph(rg, flowCurrent);
        value += flowCurrent;
        count++;
    }

    //cout << "Total Number of Augmented Paths Needed:" << count << endl;
    fprintf(fp, "Total Number of Augmented Paths Needed:\n");
    fprintf(fp, "%d\n", count);

    fprintf(fp, "Flow Value:\n");
    fprintf(fp, "%d\n", value);
    //cout << "Flow value:" << value << "\n";
    //  time = clock() - time;
    return rg;
}

// Module to read Input from the file

ResidualGraph readInput(FILE *input) {

    int v, w, capacity;
    if (input == NULL) {
        //cout << "File not present";
    } else {
        fscanf(input, "%d %d", &nV, &nE);
        //cout << "The number of vertices:" << nV << "The number of edges" << nE << "\n";
        fscanf(input, "%d %d %d", &source, &destination, &k);
        ResidualGraph flow(nV);
        while (!feof(input)) {
            fscanf(input, "%d %d %d", &v, &w, &capacity);
            AdjacentNode e(v, w, capacity, 0);
            flow.addEdge(e);
        }
        return flow;
    }
    assert(0);
    ResidualGraph flow(nV);
    return flow;
}

void setValue(int value_new) {
    value = value_new;
}

void setFILE(FILE *fp_new) {
    fp = fp_new;
}

int mfmc_method1(FILE *input, FILE *output, Map &f_mfmc) {
    setFILE(output);

    ResidualGraph f = readInput(input);
    MaxFlowMinCut max(&f);
    f = max.getMaxFlow();
    max.writeOutput(&f);
    max.getEdgeNonZero_no_s_t(&f, f_mfmc);
    max.getMinCut(&f, false);

    return 0;
}

int mfmc_method2(FILE *input, FILE *output, Map &f_mfmc) {
    setFILE(output);

    ResidualGraph flowNetwork = readInput(input);
    MaxFlowMinCut max1(&flowNetwork);
    setValue(0);
    fprintf(output, "The variant of max flow ford fulkerson\n");
    flowNetwork = max1.getMaxFlowWidestPath();
    max1.writeOutput(&flowNetwork);
    max1.getEdgeNonZero_no_s_t(&flowNetwork, f_mfmc);
    max1.getMinCut(&flowNetwork, false);
    return 0;
}

int mfmc_method(const char *path1, const char *path2, Map &f_mfmc) {
    FILE *input = 0;
    input = fopen(path1, "r");
    FILE *output = fopen(path2, "w");
//	mfmc_method1(input, output, f_mfmc);
    mfmc_method2(input, output, f_mfmc);
    fclose(input);
    fclose(output);
    return 0;
}