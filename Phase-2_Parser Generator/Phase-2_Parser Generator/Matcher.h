#include <vector>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <sstream>
#include <iterator>

using namespace std;

class Matcher
{

public:
    Matcher(vector<vector<string> > table, unordered_map<string , int> *terminals, unordered_map<string , int> *non_terminals, string start_symbol);
    virtual vector<string> start_matcher(vector <string>  tokens);
    virtual ~Matcher();

private :

    vector < vector <string> > table ;
    unordered_map <string , int> *terminals ;
    unordered_map <string , int> *non_terminals ;
    string start_symbol ;

};
