#include <bits/stdc++.h>
#include "State.h"
#include "Edge.h"
#include "Graph.h"

/********************************** State implementation *****************************************/
State::State()
{
    //Constructor
}
/*
 * set the number of this state
 */
void State::set_state_num(int num)
{
    state_num = num;
}
/*
 * get the number of this state
 */
int State::get_state_num()
{
    return this->state_num;
}

/*
 * set this state as an acceptance state
 * set the token that is accepted at this node
 */
void State::set_accepting(bool acceptance, string accepted_token)
{
    this->is_accepting = acceptance;
    this->accepted_token = accepted_token;
}
/*
 * get the token that is accepted at this node
 */
string State::get_accepted_token()
{
    return accepted_token;
}
/*
 * get if this state is acceptance or not
 */
bool State::is_accepting_state()
{
    return this->is_accepting;
}
/*
 * get all the children of this state
 */
vector<Edge>* State::get_children()
{
    return &children;
}
/*
 * add a new state v as a child to this state u
 */
void State::add_child_state(State* to, string weight)
{
    Edge edge(this, to, weight);
    children.push_back(edge);
}

bool State::get_visit()
{
    return visited;
}
void State::set_visit(bool v)
{
    visited = v;
}
/* Destructor */
State::~State()
{
    //don't forget Destructor in c++ => no garbage collector
}


/********************************** Edge implementation *****************************************/

Edge::Edge(State* from, State* to, string weight)
{
    //Constructor
    this->from = from;
    this->to = to;
    this->weight = weight;

}

/*
 * get the value(move) of this edge
 */
string Edge::get_weight()
{
    return weight;
}
/*
 * get the 2 states of this edge as a pair
 */
pair<State*, State*> Edge::get_start_end_states()
{
    return make_pair(from, to);
}

State* Edge::getTo()
{
    return to;
}
/* Destructor */
Edge::~Edge()
{
    //don't forget Destructor in c++ => no garbage collector
}

/********************************** Graph implementation *****************************************/
Graph::Graph()
{
    //Constructor
    start = new State();
    endd = new State();
}

/*
 * set start state of this graph
 */
void Graph::set_start_state(State* start)
{
    this->start = start;
}

/*
 * get start state of this graph
 */
State* Graph::get_start_state()
{
    return this->start;
}

/*
 * set end state of this graph
 */
void Graph::set_end_state(State* endd)
{
    this->endd = endd;
}

/*
 * get end state of this graph
 */
State* Graph::get_end_state()
{
    return endd;
}

/*
 * set the size (# of states) of this graph
 */
void Graph::set_graph_size(int s)
{
    this->graph_size = s;
}

/*
 * get size (# of states) of this graph
 */
int Graph::get_graph_size()
{
    return graph_size;
}

void Graph::clear_visited()
{
    State* oldst = get_start_state();
    queue<State*> current;
    current.push(oldst);
    while(!current.empty())
    {
        State* tmp = current.front();
        tmp->set_visit(false);
        current.pop();
        vector<Edge>* children = tmp->get_children();
        for(int i=0; i < children->size(); i++)
        {
            Edge e = (*children)[i];
            State* to = e.getTo();
            if(to->get_visit())
            {
                current.push(to);
            }
        }
    }
}
/* Destructor */
Graph::~Graph()
{
    //don't forget Destructor in c++ => no garbage collector
}
