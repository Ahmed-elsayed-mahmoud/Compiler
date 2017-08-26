#ifndef RULE_H_INCLUDED
#define RULE_H_INCLUDED

#include <bits/stdc++.h>
#include "Symbol.h"
#include "StringHelp.h"
#include "CFG_Parser.h"

using namespace std;

class Symbol;
class CFG_Parser;

class Rule {

public:
    Rule();
    Rule(string rule_name, CFG_Parser* cfg);

    struct RuleStringStruct { //X --> a b
        string lhs; // X
        string rhs; // a b
    };

    string get_rule_name();
    vector<vector<Symbol*> > get_productions();
    void set_rule_name(string name);
    void set_productions(vector<vector<Symbol*> > prod);
    void parse_productions_from_string(string rhs);
    RuleStringStruct split_rule_string(string rule);

private:

    CFG_Parser* cfg;
    // rule name is left hand side before = sign
    string rule_name;
    // Outer vector is OR predication_rules x = a | b. Inner following predication_rules a = '(' 'id' ')'.
    vector<vector<Symbol*> > productions;

    vector<string> parse_or_tokens(string s);
    vector<Symbol*> parse_one_token(string &s);
};


#endif // RULE_H_INCLUDED
