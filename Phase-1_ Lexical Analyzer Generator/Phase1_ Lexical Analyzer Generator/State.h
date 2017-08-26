#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <bits/stdc++.h>
#include "Edge.h"

using namespace std;

class State
{
public:
    State();

    void set_state_num(int num);
    int get_state_num();

    void set_accepting(bool acceptance, string accepted_token);
    bool is_accepting_state();
    string get_accepted_token();

    void add_child_state(State* to, string weight);

    bool get_visit();
    void set_visit(bool v);

    vector<Edge> * get_children();

    virtual ~State();

private:
    bool visited = false;
    bool is_accepting = false;
    string accepted_token = "";
    int state_num = -1;
    vector<Edge> children;
};


#endif // STATE_H_INCLUDED
