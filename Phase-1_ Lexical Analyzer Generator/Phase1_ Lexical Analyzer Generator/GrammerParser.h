#ifndef GRAMMERPARSER_H
#define GRAMMERPARSER_H

#include "Graph.h"

class GrammerParser
{
public:
    GrammerParser(string grammerFile);
    Graph* getFullNFA();
    Graph* clone(Graph* g);
    vector<string> get_weights();
    vector<string> get_expressions();

    virtual ~GrammerParser();

protected:

private:
    string grammerFile;
    vector<string> grammer;
    int statenum = 0;
    unordered_map<string,Graph*> definitions;
    unordered_map<string,Graph*> expressions;
    vector<string> expNames;
    vector<string> weights;

    void getNFA(string line);
    void readGrammerFile(string path);
    void addOperation(char c, stack<string>* in, stack<string>* post);
    bool isOperation(char c);
    bool isOperation(string c);
    string removeSpaces(string str);
    vector<string> getTokens(string str2);
    stack<string> getPostfix(vector<string> tokens);
    bool isBinaryOp(string c);
    Graph* draw(stack<string> postfix, string acceptance);
    Graph* drawOR(Graph* g1, Graph* g2, string acceptance);
    Graph* drawCO(Graph* g1, Graph* g2, string acceptance);
    Graph* drawKclosure(Graph* g1, string acceptance);
    Graph* drawPclosure(Graph* g1, string acceptance);
    void printGraph(Graph* g);
    void setWeights(Graph* g);
    bool check_for_weight(string weight);
};

#endif // GRAMMERPARSER_H
