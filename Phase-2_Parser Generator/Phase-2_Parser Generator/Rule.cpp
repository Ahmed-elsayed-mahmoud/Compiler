#include "Rule.h"

Rule::Rule()
{
    // Constructor
}
Rule::Rule(string rule_name, CFG_Parser* cfg)
{
    this->rule_name = rule_name;
    this->cfg = cfg;
    if(!cfg->find_rule_with_lhs(rule_name)) {
        cfg->rules.push_back(this);
    }
    if(! cfg->find_symbol_with_name(rule_name, "non_terminal")){
        cfg->non_terminals.push_back(new Symbol(this));
    }
}

Rule::RuleStringStruct Rule::split_rule_string(string rule) {
    // Finding the assignment operator. "=" or "::=".
    string assign_op = "=";
    size_t assign_op_ind = rule.find(assign_op);
    if(assign_op_ind == string::npos) { // did not find assign op
        cout << "Invalid Rule; must have an assignment operator." <<endl;
        return {};
    }

    string lhs = rule.substr(0, assign_op_ind - 1);
    string rhs = rule.substr(assign_op_ind + 1 + assign_op.length());
    // check valid RHS, LHS
    string s2 = rhs;
    remove_right_left_spaces(s2);
    if(lhs[0] == '=' ){
        cout << "Invalid Rule; must have LHS." <<endl;
        lhs ="";
    }
    if(s2.size() == 0 ){
        cout << "Invalid Rule; must have RHS." <<endl;
    }
    if(s2[0] == '|' ){
        cout << "Invalid Rule; must have a production before '|' operator." <<endl;
        rhs ="";
    }
    RuleStringStruct gst = { lhs, rhs };
    return gst;
}

vector<string> Rule::parse_or_tokens(string str) {
    str += " |"; // To avoid handling last token.

    vector<string> tokens;

    for(int i=0, last=0; i != str.size(); ++i) {
        // Escape backslash character. So we gonna escape the next char.
        // Escape backslash character precedes reserved symbol
        if(str[i] == '\\') {
            ++i;
        } else if(str[i] == '|') {
            string token = str.substr(last, i - 1 - last);
            remove_right_left_spaces(token);
            tokens.push_back(token);
            last = i + 1;
        }
    }
    return tokens;
}



void Rule::parse_productions_from_string(string rhs) {
    vector<string> or_tokens = parse_or_tokens(rhs);

    for(string token : or_tokens){
        productions.push_back(parse_one_token(token));
    }
}

//Assume there exist no terminal symbol with inner spaces. e.g 'i d'
vector<Symbol*> Rule::parse_one_token(string &s) {
    vector<string> tokens = split(s, ' ', true);

    vector<Symbol*> res;

    for(string token : tokens) {
        remove_right_left_spaces(token);
        if(token[0] == '\'') { // Terminal Symbol
            // Removing the quotes from around the terminal token name. 'id' -> id.
            string t = token.substr(1, token.length() - 2);
            Symbol* s = new Symbol(t);
            res.push_back(s);
            if(t != "\\L" && ! cfg->find_symbol_with_name(t, "terminal")){
                cfg->terminals.push_back(s);
            }
        }
        else { // NonTerminal Symbol
            Symbol* s;
            if(Rule* rule = cfg->find_rule_with_lhs(token)) { // Exist in the rules.
                s = new Symbol(rule);
                res.push_back(s);
            }
            else { // Not exist before.
                // Make new GrammarRule with only a name.
                Rule* new_rule = new Rule(token, cfg);
                s = new Symbol(new_rule);
                // Adding the reference to of the new Grammar Rule into the tokens of this Grammar.
                res.push_back(s);
            }
            if(! cfg->find_symbol_with_name(token, "non_terminal")){
                cfg->non_terminals.push_back(s);
            }
        }
    }

    return res;
}

string Rule::get_rule_name()
{
    return rule_name;
}

vector<vector<Symbol*> > Rule::get_productions()
{
    return productions;
}

void Rule::set_rule_name(string name)
{
    this->rule_name = name;
}

void Rule::set_productions(vector<vector<Symbol*> > prod)
{
    this->productions = prod;
}
