#include <string.h>
#include <stack>
#include <fstream>
#include "DFA_State.h"

using namespace std;

class Tokens
{
public:

    Tokens(vector <vector <DFA_State> > table, map <char, int> alphabet, vector <DFA_State> states,string infile);
    Tokens() {};

    string getToken();

    virtual ~Tokens();

private:

    char check_char();
    int get_alpha_idx(char test);
    ifstream inputFile;
    ofstream outputFile;
    ofstream errorFile;
    ofstream symbolFile;
    vector <vector <DFA_State> > transitionTable;
    map <char, int> alphabet;
    vector <DFA_State> states;
    vector <pair<DFA_State, char> > input;
    stack <pair<DFA_State, char> > readHistory;
    int currentStateNum;

};
