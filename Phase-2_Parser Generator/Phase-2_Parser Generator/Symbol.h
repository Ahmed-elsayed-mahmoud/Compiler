#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED

#include <bits/stdc++.h>
#include "Rule.h"

using namespace std;

class Rule;

class Symbol {

public:

    Symbol(); // Synch
    Symbol(string symbol_string); // Terminal
    Symbol(Rule* grammar_rule); // Nonterminal

    enum Type {
        Terminal,
        NonTerminal,
        Synch
    };

    Rule* get_symbol_rule();
    Type get_symbol_type();
    string get_symbol_string();

private:
    Rule* grammar_rule;
    string symbol_string;
    Type type;
};

#endif // SYMBOL_H_INCLUDED
