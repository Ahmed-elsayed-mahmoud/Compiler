#ifndef NFA_TO_DFA_H_INCLUDED
#define NFA_TO_DFA_H_INCLUDED

#include "Graph.h"
#include "State.h"
#include "Edge.h"
#include "DFA_State.h"
#include <bits/stdc++.h>

using namespace std;

class NFA_To_DFA
{
public:
//Map to hold all inputs and its mapping to column number in transition table
    NFA_To_DFA();
    NFA_To_DFA(Graph * NFA, vector<string> input, vector<string> tokens);
    vector < vector < DFA_State > > get_DFA_table ();
    vector < DFA_State> get_DFA_states ();
    vector<string> get_accepted_tokens();

    virtual ~NFA_To_DFA();

private:
    void DFA_BFS(State *s, unordered_map<string,int> input_map);
    void do_subset_construction (int numberOfInputs);
    int get_dfa_state_index (DFA_State state );
    void print_DFA_trans_table();
    vector < vector < int > > nfa_trans_table;
    vector < vector <int > > ep_clos_table;
    vector<bool> accepted_states;
    vector<string> accepted_tokens;
    vector < vector < DFA_State > > DFA_table ;
    vector <DFA_State> DFA_states ;
    vector <string> new_accepted_tokens ;
    unordered_map<string, int> acc_tokens_map;
};


#endif // NFA_TO_DFA_H_INCLUDED
