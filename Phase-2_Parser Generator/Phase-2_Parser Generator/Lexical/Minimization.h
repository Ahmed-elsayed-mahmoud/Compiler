#ifndef MINIMIZATION_H_INCLUDED
#define MINIMIZATION_H_INCLUDED

#include "DFA_State.h"
#include <bits/stdc++.h>
using namespace std;

class Minimization
{

public:

    Minimization(unordered_map<string, int> tokensPriorities);
    vector < vector <DFA_State> > minimize_DFA (vector < vector <DFA_State> > DFA,vector <DFA_State> states );
    vector < DFA_State > get_minimum_states();
    virtual ~Minimization();

private:

    vector < vector <DFA_State> > min_DFA;
    vector < DFA_State > min_states;
    unordered_map<string, int> tokensPriorities;
    template<typename T> void printTable (T t, int width);
    void printTransitionTable (vector< vector <DFA_State> > states);

    ofstream tables;

};


#endif // MINIMIZATION_H_INCLUDED
