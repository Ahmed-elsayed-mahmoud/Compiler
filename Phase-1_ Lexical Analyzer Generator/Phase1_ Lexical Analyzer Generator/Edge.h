#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

class State;

using namespace std;

class Edge
{
public:
    Edge(State* from, State* to, string weight);

    pair<State*, State*> get_start_end_states();
    State* getTo();
    string get_weight();

    virtual ~Edge();

private:
    string weight;
    State *from;
    State *to;
};


#endif // EDGE_H_INCLUDED
