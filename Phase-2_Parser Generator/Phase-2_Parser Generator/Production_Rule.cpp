#include "Production_Rule.h"

Production_Rule::Production_Rule()
{

}

vector<Symbol*> Production_Rule::get_production_rule(){
    return production;
}

void Production_Rule::set_production_rule(vector<Symbol*> production){
    this->production = production;
}

Production_Rule::~Production_Rule()
{
    //dtor
}
