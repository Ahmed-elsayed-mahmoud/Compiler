#ifndef LEXICAL_ANALYZER_H_INCLUDED
#define LEXICAL_ANALYZER_H_INCLUDED

#include <bits/stdc++.h>
#include "GrammerParser.h"
#include "NFA_To_DFA.h"
#include "Minimization.h"
#include "DFA_State.h"
#include "Tokens.h"
#include "State.h"
#include "Edge.h"
#include "Graph.h"

using namespace std;

class Lexical_Analyzer
{

public:
    Lexical_Analyzer(string file_path);
    void test_dfa();
    void test_grammer_parser();
    void test_minimization();
    vector<string> get_tokens();
    virtual ~Lexical_Analyzer();
protected:
private:
};

#endif // LEXICAL_ANALYZER_H_INCLUDED
