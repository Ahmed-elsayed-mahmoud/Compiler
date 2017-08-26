#include "Symbol.h"

Symbol::Symbol(Rule* rule)
{
    this->grammar_rule = rule;
    this->symbol_string = rule->get_rule_name();
    this->type = NonTerminal;
}

Symbol::Symbol(string symbol_string)
{
    this->grammar_rule = NULL;
    this->symbol_string = symbol_string;
    this->type = Terminal;
}

Symbol::Symbol()
{
    this->grammar_rule = NULL;
    this->symbol_string = "Synch";
    this->type = Synch;
}

Rule* Symbol::get_symbol_rule()
{
    return grammar_rule;
}

Symbol::Type Symbol::get_symbol_type()
{
    return type;
}

string Symbol::get_symbol_string()
{
    return symbol_string;
}


