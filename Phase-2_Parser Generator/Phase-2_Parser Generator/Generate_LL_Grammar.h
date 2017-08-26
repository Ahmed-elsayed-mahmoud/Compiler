#ifndef GENERATE_LL_GRAMMAR_H_INCLUDED
#define GENERATE_LL_GRAMMAR_H_INCLUDED

#include <bits/stdc++.h>
#include "Symbol.h"
#include "Rule.h"

using namespace std;

class Symbol;
class Rule;

class Generate_LL_Grammar {

public:
    Generate_LL_Grammar(vector <Rule*> cfg_rules, vector<Symbol*> non_terminals);
    vector <Rule*> get_rules();
    vector<Symbol*> get_non_terminals();
private:

    vector <Rule*> rules;
    vector<Symbol*> non_terminals;
    void eliminate_left_recursion();
    void eliminate_left_factoring();
    int find_rule_index(string s);
    void substitute_left_rec(Rule* rule);
    void substitute_left_fac(Rule* rule);
    void add_non_terminal(Symbol* s);
};


#endif // GENERATE_LL_GRAMMAR_H_INCLUDED
