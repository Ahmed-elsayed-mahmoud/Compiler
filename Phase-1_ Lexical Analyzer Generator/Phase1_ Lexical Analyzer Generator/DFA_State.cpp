#include "DFA_State.h"


//Constructor
DFA_State::DFA_State(int dfa_state_size )
{
    for (int i=0; i<dfa_state_size; i++)
        this->nfa_states.push_back(false);
    this->is_acceptance = false;
    is_empty_s = true;
}

void DFA_State::add_nfa_state (int nfa_state_num, bool is_acceptance)
{
    is_empty_s = false;
    if (!this->is_acceptance)
        this->is_acceptance = is_acceptance;

    if (nfa_states[nfa_state_num])
        return ;

    nfa_states.at(nfa_state_num) = true;
    nfa_states_nums.push_back (nfa_state_num);
}
void DFA_State:: set_accepting(bool acceptance, string accepted_token)
{
    is_acceptance = acceptance;
    this->accepted_token = accepted_token;
}
string DFA_State::get_accepted_token()
{
    return accepted_token;
}
vector<bool> DFA_State::get_nfa_states_accept ()
{
    return nfa_states;
}

bool DFA_State::is_accepting_state ()
{
    return is_acceptance;
}
void  DFA_State::set_state_num (int num )
{
    this->dfa_state_num = num ;
}
int DFA_State::get_state_num ()
{
    return dfa_state_num;
}

vector <int> DFA_State::get_nfa_states_nums ()
{
    return nfa_states_nums;
}
bool DFA_State::is_empty_state ()
{
    return is_empty_s;
}


DFA_State::~DFA_State()
{
    //Destructor
}
