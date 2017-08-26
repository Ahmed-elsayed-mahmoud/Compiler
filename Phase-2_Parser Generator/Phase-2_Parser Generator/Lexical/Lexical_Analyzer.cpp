#include "Lexical_Analyzer.h"

Lexical_Analyzer::Lexical_Analyzer(string filename)
{
    // Grammar parsing and construct NFA
    GrammerParser* gr = new GrammerParser("Lexical/regular_expressions.txt");
    Graph * nfa_graph = gr->getFullNFA();
    vector<string> input = gr->get_weights();
    vector<string> tokens = gr->get_expressions();
    // NFA to DFA
    NFA_To_DFA dfa(nfa_graph, input, tokens);
    vector < vector < DFA_State > > dfa_table = dfa.get_DFA_table();
    vector < DFA_State > dfa_states = dfa.get_DFA_states();

    unordered_map<string, int> acc_tokens_map;
    for(int i=tokens.size()-1; i>=0; i--)
    {
        acc_tokens_map[tokens[i]] = i;
    }

    // Minimization
    Minimization *minimizer = new Minimization(acc_tokens_map);
    vector < vector < DFA_State > > min_dfa = minimizer->minimize_DFA(dfa_table, dfa_states);
    vector < DFA_State > min_states = minimizer->get_minimum_states();

    map<char, int> input_map_table;
    for(int i=0; i<input.size(); i++)
    {
        input_map_table[input[i][0]] = i;
    }

/*  for(auto it = input_map_table.cbegin(); it != input_map_table.cend(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }
*/

    Tokens *token_manager = new Tokens(min_dfa, input_map_table, min_states, filename);
    string token = token_manager->getToken();
    while(token != ""){
        token = token_manager->getToken();
    }
}

/************************ Testing NFA to DFA *********************/
void Lexical_Analyzer::test_dfa()
{
    Graph *g = new Graph();
    g->set_graph_size(8);
    State *s0 = new State(), *s1= new State(),*s2= new State();
    State *s3 = new State(), *s4= new State(),*s5= new State();
    State *s6 = new State(), *s7= new State(), *s8= new State();

    s0->set_state_num(0);
    s1->set_state_num(1);
    s2->set_state_num(2);
    s3->set_state_num(3);
    s4->set_state_num(4);
    s5->set_state_num(5);
    s6->set_state_num(6);
    s7->set_state_num(7);
    s8->set_state_num(8);



    s2->set_accepting(true, "p1");
    s6->set_accepting(true, "p2");
    s8->set_accepting(true, "p3");
    s4->set_accepting(true, "p4");

    s0->add_child_state(s1, "");
    s0->add_child_state(s3, "");
    s0->add_child_state(s7, "");
    s1->add_child_state(s2, "a");
    s3->add_child_state(s4, "a");
    s4->add_child_state(s5, "b");
    s5->add_child_state(s6, "b");
    s7->add_child_state(s8, "c");
    s7->add_child_state(s8, "d");
    s7->add_child_state(s8, "b");
    s8->add_child_state(s8, "b");

    g->set_start_state(s0);


    vector<string>v, input;
    input.push_back("a");
    input.push_back("b");
    input.push_back("c");
    input.push_back("d");
    // p4 has higher priority than p1
    v.push_back("p1");
    v.push_back("p3");
    v.push_back("p2");
    v.push_back("p4");

    NFA_To_DFA dfa(g, input, v);
    vector < vector < DFA_State > > table = dfa.get_DFA_table();
    vector < DFA_State > dfa_states = dfa.get_DFA_states();
    for(int i=0; i<dfa_states.size(); i++)
    {
        cout<<"State  "<<dfa_states[i].get_state_num()<< "  =  " ;
        vector <int> v = dfa_states[i].get_nfa_states_nums();
        for(int j=0; j<v.size(); j++)
        {
            cout << v[j] << "  ,  ";
        }
        cout << " accepted token: " << dfa_states[i].get_accepted_token()<<endl;
    }
    cout <<endl;
    for(int i=0; i<table.size(); i++)
    {
        for(int j=0; j<table[i].size(); j++)
        {
            cout<< table[i][j].get_state_num() <<" | " ;
        }
        cout <<"\n";
    }

}
/************************ Testing Grammar Parser & NFA *********************/
void Lexical_Analyzer::test_grammer_parser()
{
    GrammerParser* gr = new GrammerParser("regular_expressions.txt");
    gr->getFullNFA();
    vector<string> weights = gr->get_weights();
    for(int i=0; i< weights.size(); i++)
    {
        cout << "\t" <<weights[i] << endl;
    }
    cout << "Expression" << endl;
    vector<string> expNames = gr->get_expressions();
    for(int i=0; i< expNames.size(); i++)
    {
        cout << "\t" <<expNames[i] << endl;
    }
}

/************************ Testing Minimization *********************/
void Lexical_Analyzer::test_minimization()
{

}

Lexical_Analyzer::~Lexical_Analyzer()
{

}
