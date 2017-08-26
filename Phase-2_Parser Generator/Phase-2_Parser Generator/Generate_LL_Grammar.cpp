#include "Generate_LL_Grammar.h"

Generate_LL_Grammar::Generate_LL_Grammar(vector <Rule*> cfg_rules, vector<Symbol*> non_terminals)
{
    this->rules = cfg_rules;
    this->non_terminals = non_terminals;
    eliminate_left_recursion();
    eliminate_left_factoring();
}

int Generate_LL_Grammar::find_rule_index(string s)
{
    for(int i=0; i<rules.size(); i++)
    {
        if(rules[i]->get_rule_name() == s) return i;
    }
    return 0;
}

void Generate_LL_Grammar::substitute_left_rec(Rule* rule)
{
    vector<vector<Symbol*>> productions = rule->get_productions();
    vector<vector<Symbol*>> new_prod = productions;
    int curr_rule_ind = find_rule_index(rule->get_rule_name());
    for(int i=0; i<productions.size(); i++)
    {
        Symbol* s = productions[i][0];

        if(s->get_symbol_type() == Symbol::Type::NonTerminal)
        {
            int sub_rule_ind = find_rule_index(s->get_symbol_string());
            if(sub_rule_ind < curr_rule_ind){
                Rule* sub_rule = rules[sub_rule_ind];
                vector<vector<Symbol*>> sub_prod = sub_rule->get_productions();
                vector<vector<Symbol*>> new_or = sub_prod;
                for(int j=0; j<sub_prod.size(); j++)
                {
                    if(sub_prod[j][0]->get_symbol_string() == rule->get_rule_name())
                    {

                        for(int k=0; k<sub_prod.size() && productions[i].size()>1; k++)
                        {
                            sub_prod[k].insert(sub_prod[k].end(), productions[i].begin() + 1, productions[i].end());
                        }
                        new_prod.erase(new_prod.begin() + i);
                        new_prod.insert(new_prod.end(), sub_prod.begin(), sub_prod.end());
                        break;
                    }
                }
            }
        }
    }
    rule->set_productions(new_prod);
    //rules[curr_rule_ind] = rule;
}
void Generate_LL_Grammar::substitute_left_fac(Rule* rule)
{
    vector<vector<Symbol*>> productions = rule->get_productions();
    vector<vector<Symbol*>> new_prod = productions;
    int curr_rule_ind = find_rule_index(rule->get_rule_name());
    for(int i=0; i<productions.size(); i++)
    {
        Symbol* curr_sym = productions[i][0];

        if(curr_sym->get_symbol_type() == Symbol::Type::NonTerminal)
        {
            int sub_rule_ind = find_rule_index(curr_sym->get_symbol_string());
            if (sub_rule_ind < curr_rule_ind){
                Rule* sub_rule = rules[sub_rule_ind];
                vector<vector<Symbol*>> sub_prod = sub_rule->get_productions();
                vector<vector<Symbol*>> new_or = sub_prod;
                for(int j=0; j<sub_prod.size(); j++)
                {
                    for(int k=0; k<productions.size(); k++)
                    {
                        if(k == i) continue;
                        if(sub_prod[j][0]->get_symbol_string() == productions[k][0]->get_symbol_string())
                        {

                            for(int l=0; l<sub_prod.size(); l++)
                            {
                                sub_prod[l].insert(sub_prod[l].end(), productions[i].begin() + 1, productions[i].end());
                            }
                            new_prod.erase(new_prod.begin() + i);
                            new_prod.insert(new_prod.end(), sub_prod.begin(), sub_prod.end());
                            break;
                        }
                    }
                }
            }
        }
    }
    rule->set_productions(new_prod);
    //rules[curr_rule_ind] = rule;cout<<rule->get_rule_name()<<endl;
}
void Generate_LL_Grammar::eliminate_left_recursion()
{
    for(int r = 0; r < rules.size(); ++r)
    {
        Rule* rule = rules[r];
        substitute_left_rec(rule);
        vector<vector<Symbol*>> new_productions;
        vector<vector<Symbol*>> new_nonTerminal_productions;
        for(int i=0; i< (int)rule->get_productions().size(); i++)
        {
            vector<Symbol*> production = rule->get_productions()[i];
            vector<Symbol*> new_production;
            vector<Symbol*> new_nonTerminal_production;
            if(production[0]->get_symbol_type() == Symbol::Type::NonTerminal && production[0]->get_symbol_string() == rule->get_rule_name())
            {

                new_nonTerminal_production.insert(new_nonTerminal_production.end(),
                                                  production.begin() + 1, production.end());
                new_nonTerminal_productions.push_back(new_nonTerminal_production);
            }
            else
            {
                new_production.insert(new_production.end(), production.begin(), production.end());
                new_productions.push_back(new_production);
            }
        }
        Rule *new_rule = nullptr;
        Symbol *new_rule_symbol = nullptr;
        if(new_nonTerminal_productions.size() > 0)
        {
            new_rule = new Rule();
            new_rule->set_rule_name(rule->get_rule_name() + "\'");
            new_rule_symbol  = new Symbol(new_rule);
            Symbol *lambda = new Symbol("\\L");
            vector<Symbol*> lambda_vector = {lambda};
            for(vector<Symbol*> &concat_vector : new_nonTerminal_productions)
            {
                concat_vector.push_back(new_rule_symbol);
            }
            for(vector<Symbol*> &concat_vector : new_productions)
            {
                concat_vector.push_back(new_rule_symbol);
            }
            new_nonTerminal_productions.push_back(lambda_vector);
            new_rule->set_productions(new_nonTerminal_productions);
        }
        if(new_productions.size() > 0)
        {
            rule->set_productions(new_productions);
        }
        else
        {
            vector<vector<Symbol *>> vv;
            vector<Symbol *> v;
            v.push_back(new_rule_symbol);
            vv.push_back(v);
            rule->set_productions(vv);
        }
        if(new_rule != nullptr)
        {
            rules.push_back(new_rule);
            add_non_terminal(new_rule_symbol);
        }
    }
}
void Generate_LL_Grammar::eliminate_left_factoring()
{
    for(int r = 0; r < rules.size(); ++r)
    {
        Rule* rule = rules[r];
        substitute_left_fac(rule);
        vector<vector<Symbol*>> productions = rule->get_productions();
        vector<vector<Symbol*>> new_productions;bool has_lambda = false;
        for(int i=0; i< (int)productions.size() - 1; i++)
        {
            vector<Symbol*> curr_concat_prod = productions[i];
            Symbol * current_first_symbol = curr_concat_prod[0];
            for(int j=i+1;  j < (int)productions.size(); j++)
            {
                vector<Symbol*> next_concat_prod = productions[j];
                Symbol * next_first_symbol = next_concat_prod[0];
                if(next_first_symbol->get_symbol_type() == current_first_symbol->get_symbol_type()
                        && next_first_symbol->get_symbol_string() == current_first_symbol->get_symbol_string())
                {
                    vector<Symbol*> new_concat_prod;
                    if(next_concat_prod.size() > 1)
                    {
                        new_concat_prod.insert(new_concat_prod.end(),
                                               next_concat_prod.begin() + 1, next_concat_prod.end());
                        new_productions.push_back(new_concat_prod);
                    }
                    else
                    {
                        new_concat_prod.push_back(new Symbol("\\L"));
                        if(! has_lambda) new_productions.push_back(new_concat_prod);
                        has_lambda=true;
                    }

                    productions.erase(productions.begin() + j);
                }
            }
            if(new_productions.size() > 0)
            {
                vector<Symbol*> new_concat_prod;
                if(curr_concat_prod.size() > 1)
                {
                    new_concat_prod.insert(new_concat_prod.end(),
                                           curr_concat_prod.begin() + 1, curr_concat_prod.end());
                    new_productions.push_back(new_concat_prod);
                }
                else
                {
                    new_concat_prod.push_back(new Symbol("\\L"));
                    if(! has_lambda)new_productions.push_back(new_concat_prod);
                    has_lambda = true;
                }
                curr_concat_prod.erase(curr_concat_prod.begin() + 1, curr_concat_prod.end());


                Rule* new_rule = new Rule();
                new_rule->set_rule_name(rule->get_rule_name() + "\'");
                Symbol *new_rule_symbol = new Symbol(new_rule);
                add_non_terminal(new_rule_symbol);

                new_rule->set_productions(new_productions);
                rules.push_back(new_rule);

                curr_concat_prod.push_back(new_rule_symbol);
                productions[i] = curr_concat_prod;

            }
        }
        rule->set_productions(productions);
    }
}
void Generate_LL_Grammar::add_non_terminal(Symbol* sym)
{
    for (Symbol* s : non_terminals)
    {
        if(s->get_symbol_string() == sym->get_symbol_string() ) return ; // exist before.
    }
    non_terminals.push_back(sym);
}
vector <Rule*> Generate_LL_Grammar::get_rules()
{
    return rules;
}
vector<Symbol*> Generate_LL_Grammar::get_non_terminals()
{
    return non_terminals;
}
