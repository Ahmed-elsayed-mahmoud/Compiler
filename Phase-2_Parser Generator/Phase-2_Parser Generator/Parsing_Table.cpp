#include "Parsing_Table.h"
#include <vector>

using namespace std;

Parsing_Table::Parsing_Table(vector<Rule*> rules,vector<Symbol*> terminals,vector<Symbol*> non_terminals)
{
    this->rules = rules;
    this->terminals = terminals;
    this->non_terminals = non_terminals;
    for(int i = 0; i < non_terminals.size(); i++){
        finished[non_terminals[i]->get_symbol_string()] = false;
        visited[non_terminals[i]->get_symbol_string()] = false;
    }
}

void Parsing_Table::make_parse_table(){
    for(int i=0; i < non_terminals.size(); i++){
        get_first(non_terminals[i]);
    }
    print_first();
    clear_visited();
    follow[non_terminals[0]->get_symbol_string()].insert("$");
    start = non_terminals[0]->get_symbol_string();
    for(int i=0; i < non_terminals.size(); i++){
        get_follow(non_terminals[i]);
        finished[non_terminals[i]->get_symbol_string()] = true;
    }
    print_follow();
    create_parse_table();
}

void Parsing_Table::get_first(Symbol* s){
    if(finished[s->get_symbol_string()]){
        return;
    }
    for(int i = 0; i < rules.size(); i++){
        if(!s->get_symbol_string().compare(rules[i]->get_rule_name())){
            vector<vector<Symbol*>> production = rules[i]->get_productions();
            for(int j = 0; j < production.size(); j++){
                int l = 0;
                Symbol* symbol = production[j][0];
                if(symbol->get_symbol_type() == 0){
                    first[s->get_symbol_string()].insert(symbol->get_symbol_string());
                    continue;
                }
                else{
                    visited[s->get_symbol_string()] = true;
                    while(true){
                        set<string> next_first;
                        if(visited[symbol->get_symbol_string()]){
                            next_first = first[symbol->get_symbol_string()];
                        }
                        else{
                            get_first(symbol);
                            next_first = first[symbol->get_symbol_string()];
                        }
                        bool epsillon = false;
                        for(auto str: next_first) {
                           first[s->get_symbol_string()].insert(str);
                            if(!str.compare("\\L")){
                                epsillon = true;
                            }
                        }
                        if(!epsillon || l == production[j].size() - 1){
                            break;
                        }
                        symbol = production[j][++l];
                        if(symbol->get_symbol_type() == 0){
                            first[s->get_symbol_string()].insert(symbol->get_symbol_string());
                            break;
                        }
                    }
                    visited[s->get_symbol_string()] = false;
                }
            }
        }
    }
    finished[s->get_symbol_string()] = true;
    return;
}

void Parsing_Table::get_follow(Symbol* s){
    if(finished[s->get_symbol_string()]){
        return;
    }
    for(int i = 0; i < rules.size(); i++){
        string rule_name = rules[i]->get_rule_name();
        bool nont_follow = false;
        vector<vector<Symbol*>> productions = rules[i]->get_productions();
        for(int j = 0; j < productions.size(); j++){
            for(int k = 0; k < productions[j].size(); k++){
                int l = 1;
                Symbol* symbol = productions[j][k];
                if(!s->get_symbol_string().compare(symbol->get_symbol_string())){
                    if(k == productions[j].size()-1){
                        nont_follow = true;
                    }
                    else{
                        bool epsillon = false;
                        while(k+l < productions[j].size()){
                            epsillon = false;
                            if(productions[j][k+l]->get_symbol_type() == 0){
                                follow[s->get_symbol_string()].insert(productions[j][k+l]->get_symbol_string());
                                break;
                            }
                            set<string> firsts = first[productions[j][k+l]->get_symbol_string()];
                            for(auto str: firsts) {
                                if(!str.compare("\\L")){
                                    epsillon = true;
                                }
                                else{
                                    follow[s->get_symbol_string()].insert(str);
                                }
                            }
                            if(!epsillon){
                                break;
                            }
                            l++;
                        }
                        if(k+l == productions[j].size()){
                            nont_follow = true;
                        }
                    }
                }
            }
        }
        if(nont_follow){
            visited[s->get_symbol_string()] = true;
            Symbol *s2 = new Symbol(rules[i]);
            if(visited[s2->get_symbol_string()]){
                set<string> follwers2 = follow[s2->get_symbol_string()];
                for(auto str : follwers2){
                    follow[s->get_symbol_string()].insert(str);
                }
                visited[s->get_symbol_string()] = false;
                continue;
            }
            get_follow(s2);
            visited[s->get_symbol_string()] = false;
            set<string> followers = follow[s2->get_symbol_string()];
            for(auto str: followers) {
                follow[s->get_symbol_string()].insert(str);
            }
        }
    }
}

void Parsing_Table::create_parse_table(){
    for(int i=0; i< rules.size(); i++){
        string rule_name = rules[i]->get_rule_name();
        Symbol *s2 = new Symbol(rules[i]);
        unordered_map<string,vector<Production_Rule*>> row;
        vector<vector<Symbol*>> productions = rules[i]->get_productions();
        for(int j=0; j<productions.size(); j++){
            bool epsillon = false;
            Production_Rule *production = new Production_Rule();
            production->set_production_rule(productions[j]);
            set<string> firsts =  get_vector_first(productions[j]);
            for(auto str : firsts){
                if(!str.compare("\\L")){
                    epsillon = true;
                    continue;
                }
                unordered_map<string,vector<Production_Rule*>>::const_iterator found = row.find(str);
                if(found == row.end())
                {
                    vector<Production_Rule*>v;
                    v.push_back(production);
                    row[str] = v;
                }
                else{
                    vector<Production_Rule*> temp = row[str];
                    temp.push_back(production);
                    row[str] = temp;
                    ambig = true;
                }
            }
            if(epsillon){
                set<string> followers = follow[rule_name];
                for(auto str : followers){
                    unordered_map<string,vector<Production_Rule*>>::const_iterator found = row.find(str);
                    if(found == row.end())
                    {
                        vector<Production_Rule*>v;
                        v.push_back(production);
                        row[str] = v;
                    }
                    else{
                        vector<Production_Rule*> temp = row[str];
                        temp.push_back(production);
                        row[str] = temp;
                        ambig = true;
                    }
                }
            }
        }
        if(!contain_epsillon(first[rule_name])){
            set<string> followers = follow[rule_name];
            for(auto str : followers){
                unordered_map<string,vector<Production_Rule*>>::const_iterator found = row.find(str);
                if(found == row.end())
                {
                    Symbol *sync = new Symbol();
                    Production_Rule *pr = new Production_Rule();
                    vector<Symbol*> prod;
                    prod.push_back(sync);
                    pr->set_production_rule(prod);
                    vector<Production_Rule*>v;
                    v.push_back(pr);
                    row[str] = v;
                }
            }
        }
        parse_table[rule_name] = row;
    }
    print_table();
}

set<string> Parsing_Table::get_vector_first(vector<Symbol*> production){
    set<string> firsts;
    for(int i = 0; i < production.size(); i++){
  //      cout << "get first of " << production[i]->get_symbol_string() << endl;
        if(production[i]->get_symbol_type() == 0){
            firsts.insert(production[i]->get_symbol_string());
            return firsts;
        }
        else{
            set<string> fir = first[production[i]->get_symbol_string()];
            for(auto str : fir){
  //              cout << "add to first : " << str <<endl;
                firsts.insert(str);
            }
            if(!contain_epsillon(fir)){
                return firsts;
            }
        }
    }
}

unordered_map<string,int> Parsing_Table::get_terminals(){
    unordered_map<string,int> o;
    for(int i = 0; i <terminals.size(); i++){
        o[terminals[i]->get_symbol_string()] = i + 1;
    }
    return o;
}

unordered_map<string,int> Parsing_Table::get_non_terminals(){
    unordered_map<string,int> o;
    for(int i = 0; i <non_terminals.size(); i++){
        o[non_terminals[i]->get_symbol_string()] = i + 1;
    }
    return o;
}

bool Parsing_Table::contain_epsillon(set<string> symbols){
    for(auto str: symbols){
        if(!str.compare("\\L")){
            return true;
        }
    }
    return false;
}

bool Parsing_Table::is_ambig(){
    return ambig;
}

vector<vector<string>> Parsing_Table::get_reconstructed_table(){
    vector<vector<string>> cur_table;
    for(int i = 0; i < non_terminals.size(); i++){
        vector<string> row;
        unordered_map<string,vector<Production_Rule*>> old_row = parse_table[non_terminals[i]->get_symbol_string()];
        for(int j = 0; j < terminals.size(); j++){
            unordered_map<string,vector<Production_Rule*>>::const_iterator found = old_row.find(terminals[j]->get_symbol_string());
            if(found != old_row.end())
            {
                vector<Production_Rule*>p = old_row[terminals[j]->get_symbol_string()];
                int len = p[0]->get_production_rule().size();
                string s = "";
                for(int m = 0; m < len; m++){
                    s += p[0]->get_production_rule()[m]->get_symbol_string();
                    if(m < len - 1){
                        s += " ";
                    }
                }
                row.push_back(s);
            }
            else{
                string s = "";
                row.push_back(s);
            }
        }
        cur_table.push_back(row);
    }
    print_table_vector(cur_table);
    return cur_table;
}

string Parsing_Table::get_start(){
    return start;
}

void Parsing_Table::print_first(){
    ofstream output;
    output.open("Firsts.txt");
    output << "Firsts :" << endl;
    output << "--------" << endl;
    for(int i = 0; i < non_terminals.size(); i++){
        output << non_terminals[i]->get_symbol_string() << " : ";
        set<string> fir = first[non_terminals[i]->get_symbol_string()];
        for(auto str : fir){
            output << str << " ";
        }
        output << endl;
    }
    output.close();
}

void Parsing_Table::print_follow(){
    ofstream output;
    output.open("Follow.txt");
    output << "Follow :" << endl;
    output << "--------" << endl;
    for(int i = 0; i < non_terminals.size(); i++){
        output << non_terminals[i]->get_symbol_string() << " : ";
        set<string> fir = follow[non_terminals[i]->get_symbol_string()];
        for(auto str : fir){
            output << str << " ";
        }
        output << endl;
    }
    output.close();
}

void Parsing_Table::print_table(){
    ofstream output;
    output.open("Parse_Table.txt");
    output << "Printing Table :" << endl;
    for(int i = 0; i < non_terminals.size(); i++){
        output << "...................................................." << endl;
        output << non_terminals[i]->get_symbol_string() << endl;
        if(non_terminals[i]->get_symbol_string().length() > max_non){
            max_non = non_terminals[i]->get_symbol_string().length();
        }
        unordered_map<string,vector<Production_Rule*>> row = parse_table[non_terminals[i]->get_symbol_string()];
        for(int j = 0; j < terminals.size(); j++){
            output << terminals[j]->get_symbol_string() << " :  ";
            if(i == 0){
                max_col_len.push_back(terminals[j]->get_symbol_string().length());
            }
            unordered_map<string,vector<Production_Rule*>>::const_iterator found = row.find(terminals[j]->get_symbol_string());
            if(found != row.end())
            {
                int max_len = 0;
                vector<Production_Rule*>p = row[terminals[j]->get_symbol_string()];
                for(int k = 0; k < p.size(); k++){
                    int pro_len = 0;string str = "";
                    for(int m = 0; m < p[k]->get_production_rule().size(); m++){
                        str += p[k]->get_production_rule()[m]->get_symbol_string();
                        str += " ";
                    }
                    pro_len = str.length();
                    output << str;
                    if(pro_len > max_len){
                        max_len = pro_len;
                    }
                    output << "\t";
                }
                output << endl;
                if(max_len > max_col_len[j]){
                    max_col_len[j] = max_len;
                }

            }
            else{
                output << "Empty" << endl;
            }
        }
    }
    output.close();
}

void Parsing_Table::clear_visited(){
    for(int i=0; i< non_terminals.size(); i++){
        finished[non_terminals[i]->get_symbol_string()] = false;
        visited[non_terminals[i]->get_symbol_string()] = false;
    }
}

void Parsing_Table::print_table_vector(vector<vector<string>> table){
    ofstream output;
    output.open("Reconstructed_Parse_Table.txt");
    output << "Printing reconstructed table :" << endl;
    output << "------------------------------" << endl;
    output << left << setw(max_non+2) << " ";
    for(int i = 0; i <terminals.size(); i++){
        output << left << setw(max_col_len[i]+2) <<terminals[i]->get_symbol_string();
    }
    output << endl;
    for(int i=0; i < table.size(); i++){
        output << left << setw(max_non+2) << non_terminals[i]->get_symbol_string();
        for(int j = 0; j< table[i].size(); j++){
            output << left << setw(max_col_len[j]+2) << table[i][j] ;
        }
        output << endl;
    }

    output.close();
}

Parsing_Table::~Parsing_Table(){}
