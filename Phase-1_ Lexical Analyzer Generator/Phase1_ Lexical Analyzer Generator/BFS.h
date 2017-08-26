#ifndef BFS_H_INCLUDED
#define BFS_H_INCLUDED

#include <bits/stdc++.h>
#include "Graph.h"

using namespace std;

class BFS
{
public:
    BFS(Graph* g, string word);
    vector <State> get_visited_states();
    vector <Edge> get_visited_edges();
    vector <string> get_visited_edges_weights();
    virtual ~BFS();

private:
    queue <Edge> q;
    vector <State> visited_states;
    vector <Edge> visited_edges;
    vector <string> visited_edges_weights;
};

#endif // BFS_H_INCLUDED
