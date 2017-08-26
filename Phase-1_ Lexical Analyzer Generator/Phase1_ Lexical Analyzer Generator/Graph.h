#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <stack>

#include "State.h"

using namespace std;

class Graph
{
public:
    Graph();

    void set_start_state(State* start);
    State* get_start_state();

    void set_end_state(State* endd);
    State* get_end_state();

    void set_graph_size(int s);
    int get_graph_size();
    void clear_visited();

    virtual ~Graph();

private:
    State * start;
    State * endd;
    int graph_size =0 ;
};

#endif // GRAPH_H_INCLUDED
