#ifndef DFA_STATE_H_INCLUDED
#define DFA_STATE_H_INCLUDED

#include <bits/stdc++.h>
using namespace std;

class DFA_State
{
public:

    DFA_State(int dfa_state_size );

    void add_nfa_state (int nfa_state_num, bool is_acceptance);
    void set_accepting(bool acceptance, string accepted_token);
    void set_state_num(int num );
    bool is_accepting_state ();
    int get_state_num();
    vector <bool> get_nfa_states_accept();
    vector <int> get_nfa_states_nums();
    string get_accepted_token();
    bool is_empty_state ();
    virtual ~DFA_State();
private:
    bool is_acceptance, is_empty_s;
    vector <bool> nfa_states;
    string accepted_token;
    int dfa_state_num;
    vector <int> nfa_states_nums;
};

#endif // DFA_STATE_H_INCLUDED
