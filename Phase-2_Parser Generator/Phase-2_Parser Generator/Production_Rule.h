#ifndef PRODUCTION_RULE_H
#define PRODUCTION_RULE_H

#include "Rule.h"

class Production_Rule
{
    public:
        Production_Rule();
        vector<Symbol*> get_production_rule();
        void set_production_rule(vector<Symbol*> production);
        virtual ~Production_Rule();

    protected:

    private:
        vector<Symbol*> production;

};

#endif // PRODUCTION_RULE_H
