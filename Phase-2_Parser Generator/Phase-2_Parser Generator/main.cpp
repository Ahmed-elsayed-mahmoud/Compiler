#include <bits/stdc++.h>
#include "CFG_Parser.h"
#include "Parsing_Table.h"
#include "Generate_LL_Grammar.h"
#include "Matcher.h"
#include "Lexical/Lexical_Analyzer.h"

using namespace std;

int main()
{
    freopen("logs.txt", "w", stdout);
    cout<<"************* Lexical Analyzer *****************"<<endl;
    Lexical_Analyzer la ("Lexical/inputFile.txt");
    cout<<"************* Predictive Parser ****************"<<endl;
    CFG_Parser cfg = CFG_Parser("ContextFreeGrammar.txt");
    cfg.is_LL_grammar();
    Generate_LL_Grammar ll = Generate_LL_Grammar(cfg.get_rules(), cfg.get_non_terminals());
    vector<Rule*> vv = ll.get_rules();
    vector<Symbol*> terminals = cfg.get_terminals();
    vector<Symbol*> non_terminals = ll.get_non_terminals();
    Parsing_Table *pt = new Parsing_Table(vv,terminals,non_terminals);
    pt->make_parse_table();
    if(!pt->is_ambig()){
        vector<vector<string> > table = pt->get_reconstructed_table();
        unordered_map<string , int> terminals_map = pt->get_terminals();
        unordered_map<string , int> non_terminals_map = pt->get_non_terminals();
        string start_symbol = pt->get_start();
        Matcher match (table, &terminals_map, &non_terminals_map, start_symbol);
        // read tokens from file
        vector<string> tokens;
        ifstream infile;
        infile.open ("Lexical/outputFile.txt");
        string str;
        while(!infile.eof())
        {
            getline(infile,str);
            if(str.size()>0)
            {
                tokens.push_back(str);
            }
            else{
                break;
            }
        }
        infile.close();
        tokens.push_back("$");
        // validating
        vector<string> v = match.start_matcher(tokens);
        ofstream output;
        output.open("parserOutput.txt");
        for (int i = 0; i < v.size(); i++) {
            output << v[i] << endl;
        }
        output.close();
    }
    else {
        cout << "Grammar is ambiguous." << endl;
        ofstream output;
        output.open("Reconstructed_Parse_Table.txt");
        output << "It's ambiguous look at \"Parse Table.txt\" file." << endl;
        output.close();
    }
    return 0;
}
