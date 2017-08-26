#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include "Rule.h"
#include "Production_Rule.h"
#include <set>

class Parsing_Table
{
    public:
        Parsing_Table(vector<Rule*> rules,vector<Symbol*> terminals,vector<Symbol*> non_terminals);
        void make_parse_table();
        string get_start();
        bool is_ambig();
        unordered_map<string,int> get_terminals();
        unordered_map<string,int> get_non_terminals();
        vector<vector<string>> get_reconstructed_table();
        virtual ~Parsing_Table();
    private:
        vector<Rule*> rules;
        vector<Symbol*> non_terminals;
        vector<Symbol*> terminals;
        string start;
        bool ambig = false;
        unordered_map<string,set<string>> first;
        unordered_map<string,set<string>> follow;
        unordered_map<string,bool> visited;
        unordered_map<string,bool> finished;
        unordered_map<string,unordered_map<string,vector<Production_Rule*>>> parse_table;
        int max_non = 0;
        vector<int> max_col_len;
        void get_first(Symbol* s);
        void get_follow(Symbol* s);
        void create_parse_table();
        void clear_visited();
        set<string> get_vector_first(vector<Symbol*>production);
        bool contain_epsillon(set<string> symbols);
        void print_first();
        void print_follow();
        void print_table();
        void print_table_vector(vector<vector<string>> table);
};

#endif // PARSING_TABLE_H
