#include <iostream>
#include <regex>
#include <stack>
#include <math.h>
#include "GrammerParser.h"

GrammerParser::GrammerParser(string grammerFile)
{
    this -> grammerFile = grammerFile;
}

Graph* GrammerParser::getFullNFA()
{
    Graph *g = new Graph();
    State *start = new State();
    start -> set_state_num(statenum++);
    g -> set_start_state(start);
    readGrammerFile(grammerFile);
    for(int i = 0; i < grammer.size(); i++)
    {
        string line = grammer[i];
        getNFA(line);
    }
    for(int i=0; i< expNames.size(); i++)
    {
        Graph* g2 = expressions[expNames[i]];
        State* st2 = g2->get_start_state();
        start->add_child_state(st2,"");
    }
    setWeights(g);
 //   printGraph(g);
    return g;
}

void GrammerParser::readGrammerFile(string path)
{
    ifstream infile;
    infile.open (path);
    string str;
    while(!infile.eof())
    {
        getline(infile,str);
        if(str.size()>0)
        {
            grammer.push_back(str);
        }
    }
    infile.close();
}

void GrammerParser::getNFA(string line)
{
    regex splitter1("\\{\\s*([^.]+)\\s*\\}");
    regex splitter2("\\[\\s*([^.]+)\\s*\\]");
    regex splitter3 ("\\s*(\\S+)\\s*(=|:)\\s*(.+)");
    regex spaceSplitter("(\\S+)");
    std::smatch match1;
    //cout << line << endl;
    if(regex_match(line,splitter1))
    {
        //cout << "matched {" << endl;
        regex_search(line,match1,splitter1);
        string str = match1[1];
        while(regex_search(str,match1,spaceSplitter))
        {
            string tok = match1[0];
            //cout << "token is " << token << endl;
            State *s = new State();
            Graph* g = new Graph();
            s->set_state_num(statenum++);
            g->set_start_state(s);
            State *prev = s;
            string token = "";
            for(int i = 0; i < tok.length(); i++)
            {
                if(tok[i] != '\\')
                {
                    token.push_back(tok[i]);
                }
            }
            for(int i = 0; i < token.length(); i++)
            {
                string c = "";
                c.push_back(token[i]);
                //cout << c << endl;
                State *s2 = new State();
                s2->set_state_num(statenum++);
                prev->add_child_state(s2, c);
                prev = s2;
            }
            prev->set_accepting(true, token);
            g->set_end_state(prev);
            //printGraph(g);
            expressions[token] = g;
            expNames.push_back(token);
            //       setWeights(g);
            str = match1.suffix().str();
        }
    }
    else if(regex_match(line,splitter2))
    {
        //cout << "matched [" << endl;
        regex_search(line,match1,splitter2);
        string str = match1[1];
        while(regex_search(str,match1,spaceSplitter))
        {
            string tok = match1[0];
            State *s = new State();
            Graph* g = new Graph();
            s->set_state_num(statenum++);
            g->set_start_state(s);
            State *prev = s;
            string token = "";
            for(int i = 0; i < tok.length(); i++)
            {
                if(tok[i] != '\\')
                {
                    token.push_back(tok[i]);
                }
            }
            for(int i = 0; i < token.length(); i++)
            {
                State *s2 = new State();
                s2->set_state_num(statenum++);
                string c = "";
                c.push_back(token[i]);
                //cout << c << endl;
                prev->add_child_state(s2, c);
                prev = s2;
            }
            prev->set_accepting(true, token);
            g->set_end_state(prev);
            //printGraph(g);
            expressions[token] = g;
            expNames.push_back(token);
            //       setWeights(g);
            str = match1.suffix().str();
        }
    }
    else if (regex_match(line,splitter3))
    {
        regex_search(line,match1,splitter3);
        string name = match1[1];
        string str2 = match1[3];
        vector<string> tokens = getTokens(str2);
        stack<string> postfix = getPostfix(tokens);
        //cout << postfix.size()<<endl;
        Graph *g = draw(postfix, match1[1]);
        //cout << "returned" << endl;
        //printGraph(g);
        if(!match1[2].compare("="))
        {
            definitions[name] = g;
            //      setWeights(g);
        }
        else
        {
            expressions[name] = g;
            expNames.push_back(name);
        }
    }
}

void GrammerParser::addOperation(char c, stack<string>* in, stack<string>* post)
{
    string operation = "";
    string cha;
    if(in->size()==0)
    {
        operation.push_back(c);
        in->push(operation);
    }
    else
    {
        if(c=='*' || c == '+')
        {
            while(in->size()!=0)
            {
                cha= in->top();
                if(!cha.compare("*") || !cha.compare("+"))
                {
                    in->pop();
                    post->push(cha);
                }
                else
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
                if(in->size()==0)
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
            }
        }
        else if(c=='#')
        {
            while(in->size()!=0)
            {
                cha= in->top();
                if(!cha.compare("#") || !cha.compare("*") || !cha.compare("+"))
                {
                    in->pop();
                    post->push(cha);
                }
                else
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
                if(in->size()==0)
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
            }
        }
        else if(c=='|')
        {
            while(in->size()!=0)
            {
                cha= in->top();
                if(!cha.compare("|") || !cha.compare("#") || !cha.compare("*") || !cha.compare("+"))
                {
                    in->pop();
                    post->push(cha);
                }
                else
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
                if(in->size()==0)
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
            }
        }
    }
}

bool GrammerParser::isOperation(char c)
{
    if(c == '+' ||c == '*' ||c == '|' ||c == '(' ||c == ')' || c == '#')
        return true;
    return false;
}

string GrammerParser::removeSpaces(string str)
{
    string temp = "";
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
        {
            temp += str[i];
        }
    }
    return temp;
}

vector<string> GrammerParser::getTokens(string str2)
{
    string str = "";
    for(int i=0; i<str2.length(); i++)
    {
        char c = str2[i];
        if(isOperation(c))
        {
            str.push_back(c);
            if(str2[i+1] != ' ')
            {
                str.push_back(' ');
            }
        }
        else if(isOperation(str2[i+1]) && str2[i] != '\\')
        {
            str.push_back(c);
            if(c != ' ')
            {
                str.push_back(' ');
            }
        }
        else
        {
            str.push_back(c);
        }
    }
    regex range ("(\\S\\s+-\\s+\\S)|(\\S\\s*-\\s+\\S)|(\\S\\s+-\\s*\\S)");
    string temp = "";
    temp += str;
    std::smatch match1;
    string prev = temp;
    while(regex_search(temp,match1,range))
    {
        string start = match1[0];
        string star = removeSpaces(start);
        str.replace(str.find(start),start.length(),star);
        temp = str;
        if (!temp.compare(prev))
            break;
    }
    vector<string> tokens;
    regex spaceSplitter("(\\S+)");
    while(regex_search(str,match1,spaceSplitter))
    {
        string token = match1[0];
        if(token.length() == 2 && token[0] == '\\' && token[1] == 'L')
        {
            //cout << "vvvvvvvvvvvvvvvvv" << endl;
            token = "";
        }
        /*if(token.length() == 2 && token[0] == '\\')
        {
            //cout << "vvvvvvvvvvvvvvvvv" << endl;
            token.push_back(token[1]);
        }*/
        //cout << "hhhhhhhhh : " << token.length() <<token << endl;
        tokens.push_back(token);
        str = match1.suffix().str();
    }
    return tokens;
}

stack<string> GrammerParser::getPostfix(vector<string> tokens)
{
    stack<string> post;
    stack<string> in;
    string cha,ch;
    for(int j=0; j < tokens.size(); j++)
    {
        string token = tokens[j];
        if(!isOperation(token))
        {
            if (j!= 0 && !isBinaryOp(tokens[j - 1]))
            {
                addOperation('#',&in,&post);
            }
            post.push(token);
        }
        else
        {
            if(!token.compare("("))
            {
                if (j != 0 && tokens[j - 1].compare("|"))
                    addOperation('#',&in,&post);
                in.push(token);
            }
            else if(!token.compare(")"))
            {
                while(true)
                {
                    cha= in.top();
                    in.pop();
                    if(!cha.compare("("))
                    {
                        if (j != tokens.size() - 1 && !isOperation(tokens[j + 1]))
                            addOperation('#',&in,&post);
                        break;
                    }
                    else
                    {
                        post.push(cha);
                    }
                }
            }
            else
            {
                addOperation(token[0], &in, &post);
            }
        }
    }
    while(in.size()!=0)
    {
        ch= in.top();
        in.pop();
        post.push(ch);
    }
    stack<string> postfix;
    while (!post.empty())
    {
        postfix.push(post.top());
        post.pop();
    }
    return postfix;
}

bool GrammerParser::isOperation(string c)
{
    if(!c.compare("+") ||!c.compare("*") ||!c.compare("|") ||!c.compare("(") ||!c.compare(")") ||!c.compare("#"))
        return true;
    return false;
}

bool GrammerParser::isBinaryOp(string c)
{
    if (!c.compare("|") ||!c.compare(")") ||!c.compare("("))
        return true;
    return false;
}

Graph* GrammerParser::draw(stack<string> postfix, string acceptance)
{
    //cout << acceptance << " is drawing" << endl;
    stack<Graph*> st;
    Graph *g1, *g2;
    string token;
    //cout << postfix.size() << endl;
    while(postfix.size()!=0)
    {
        token = postfix.top();
        //cout << "token : " << token << endl;
        postfix.pop();
        if( isOperation(token) )
        {
            //cout << "is operation" << endl;
            Graph* g = new Graph();
            switch(token[0])
            {
            case '+':
                g1 = st.top();
                st.pop();
                g = drawPclosure(g1,acceptance);
                //printGraph(g);
                break;
            case '*':
                g1 = st.top();
                st.pop();
                g = drawKclosure(g1,acceptance);
                //printGraph(g);
                break;
            case '|':
                //cout<<"entered or"<<endl;
                g1 = st.top();
                st.pop();
                g2 = st.top();
                st.pop();
                g = drawOR(g1,g2,acceptance);
                //printGraph(g);
                break;
            case '#':
                g1 = st.top();
                st.pop();
                g2 = st.top();
                st.pop();
                g = drawCO(g1,g2,acceptance);
                //printGraph(g);
                break;
            }
            st.push(g);
        }
        else
        {
            Graph *g = new Graph();
            regex split1("(\\S+)-(\\S+)");
            std::smatch match1;
            if(regex_match(token,split1))
            {
                //cout << "dash found" << endl;
                regex_search(token,match1,split1);
                string f = match1[1];
                string sc = match1[2];
                int first = f[0];
                int second = sc[0];
                State *start = new State();
                start->set_state_num(statenum++);
                g->set_start_state(start);
                State *endst = new State();
                endst->set_state_num(statenum++);
                endst->set_accepting(true, acceptance);
                for(int i=first; i <= second; i++)
                {
                    State* s = new State();
                    s->set_state_num(statenum++);
                    char c = i;
                    string w ;
                    w.push_back(c);
                    start->add_child_state(s,w);
                    s->add_child_state(endst,"");
                }
                g->set_end_state(endst);
                //printGraph(g);
            }
            else
            {
                unordered_map<string,Graph*>::const_iterator found = definitions.find(token);
                if(found != definitions.end())
                {
                    g = clone(definitions[token]);
                }
                else if(token.length()==0)
                {
                    State *s = new State();
                    s->set_state_num(statenum++);
                    g->set_start_state(s);
                    State *e = new State();
                    e->set_state_num(statenum++);
                    e->set_accepting(true,"");
                    g->set_end_state(e);
                    s->add_child_state(e, "");
                    //printGraph(g);
                }
                else
                {
                    State *s = new State();
                    s->set_state_num(statenum++);
                    g->set_start_state(s);
                    State *prev = s;
                    //cout << "token to be added now" << token << endl;
                    for(int i = 0; i < token.length(); i++)
                    {
                        string c = "";
                        if(token[i]  == '\\' && token.length() > 1){
                            continue;
                        }
                        c.push_back(token[i]);
                        State *s2 = new State();
                        s2->set_state_num(statenum++);
                        prev->add_child_state(s2, c);
                        prev = s2;
                    }
                    prev->set_accepting(true, acceptance);
                    g->set_end_state(prev);
                }
            }
            st.push(g);
        }
    }
    //cout << acceptance << " ended drawing" << endl;
    return st.top();
}

Graph* GrammerParser::clone(Graph* g)
{
    unordered_map<int,State*> clones;
    State* oldst = g->get_start_state();
    int endnum = 0, oldnum = oldst->get_state_num();
    string endAcceptance = "";
    Graph* newg = new Graph();
    vector<pair<string,pair<int,int>>> edges;
    queue<State*> current;
    if(oldst->get_visit())
    {
        return newg;
    }
    oldst->set_visit(true);
    current.push(oldst);
    while(!current.empty())
    {
        State* tmp = current.front();
        //cout << "state removed : " <<tmp->get_state_num() << endl;
        State* newst = new State();
        newst->set_state_num(statenum++);
        clones[tmp->get_state_num()] = newst;
        if(tmp->get_state_num() == oldnum)
        {
            newg->set_start_state(newst);
        }
        //cout << " state : " << clones[tmp->get_state_num()]->get_state_num();
        //cout << "   with : " << tmp->get_state_num() << endl;
        current.pop();
        vector<Edge>* children = tmp->get_children();
        if(children->empty())
        {
            endnum = tmp->get_state_num();
            endAcceptance = tmp->get_accepted_token();
        }
        for(int i=0; i < children->size(); i++)
        {
            Edge e = (*children)[i];
            State* to = e.getTo();
            if(!to->get_visit())
            {
                current.push(to);
                to->set_visit(true);
                //dcout << "child added : " << to->get_state_num() << endl;
            }
            pair<int,int> x;
            x.first = tmp->get_state_num();
            x.second = to->get_state_num();
            pair<string,pair<int,int>> p;
            p.first = e.get_weight();
            p.second = x;
            edges.push_back(p);
            //cout << p.second.first << ":" << p.second.second << "->>" << p.first << endl;
        }
    }
    for(int i=0; i < edges.size(); i++)
    {
        int from = edges[i].second.first;
        int to = edges[i].second.second;
        string weight = edges[i].first;
        clones[from]->add_child_state(clones[to],weight);
        //cout << clones[from]->get_state_num() << ":" << clones[to]->get_state_num() << "->>" << weight << endl;
    }
    //cout << endl;
    clones[endnum]->set_accepting(true, endAcceptance);
    newg->set_end_state(clones[endnum]);
    g->clear_visited();
    return newg;
}

Graph* GrammerParser::drawOR(Graph* g1, Graph* g2, string acceptance)
{
    Graph* g = new Graph();
    State* startst = new State();
    State* endst = new State();
    startst->set_state_num(statenum++);
    endst->set_state_num(statenum++);
    endst->set_accepting(true,acceptance);
    g->set_start_state(startst);
    g->set_end_state(endst);
    State* startg1 = g1->get_start_state();
    State* endg1 = g1->get_end_state();
    State* startg2 = g2->get_start_state();
    State* endg2 = g2->get_end_state();
    endg1->set_accepting(false,"");
    endg2->set_accepting(false,"");
    startst->add_child_state(startg1,"");
    startst->add_child_state(startg2,"");
    endg1->add_child_state(endst,"");
    endg2->add_child_state(endst,"");
    return g;
}

Graph* GrammerParser::drawCO(Graph* g1, Graph* g2, string acceptance)
{
    Graph* g = new Graph();
    State* startg1 = g1->get_start_state();
    State* endg1 = g1->get_end_state();
    State* startg2 = g2->get_start_state();
    State* endg2 = g2->get_end_state();
    g->set_start_state(startg2);
    endg2->set_accepting(false,"");
    endg1->set_accepting(true,acceptance);
    g->set_end_state(endg1);
    endg2->add_child_state(startg1,"");
    return g;
}

Graph* GrammerParser::drawKclosure(Graph* g1, string acceptance)
{
    Graph* g = new Graph();
    State* startst = new State();
    State* endst = new State();
    startst->set_state_num(statenum++);
    endst->set_state_num(statenum++);
    endst->set_accepting(true,acceptance);
    g->set_start_state(startst);
    g->set_end_state(endst);
    State* startg1 = g1->get_start_state();
    State* endg1 = g1->get_end_state();
    endg1->set_accepting(false,"");
    endg1->add_child_state(startg1,"");
    startst->add_child_state(endst,"");
    endg1->add_child_state(endst,"");
    startst->add_child_state(startg1,"");
    return g;
}

Graph* GrammerParser::drawPclosure(Graph* g1, string acceptance)
{
    Graph* g = new Graph();
    State* startst = new State();
    State* endst = new State();
    startst->set_state_num(statenum++);
    endst->set_state_num(statenum++);
    endst->set_accepting(true,acceptance);
    g->set_start_state(startst);
    g->set_end_state(endst);
    State* startg1 = g1->get_start_state();
    State* endg1 = g1->get_end_state();
    endg1->set_accepting(false,"");
    endg1->add_child_state(startg1,"");
    endg1->add_child_state(endst,"");
    startst->add_child_state(startg1,"");
    return g;
}

void GrammerParser::printGraph(Graph* g)
{
    cout << "started printing" <<endl;
    State* oldst = g->get_start_state();
    queue<State*> current;
    current.push(oldst);
    while(!current.empty())
    {
        State* tmp = current.front();
        cout << "I am " << tmp->get_state_num() << endl;
        current.pop();
        tmp->set_visit(true);
        vector<Edge>* children = tmp->get_children();
        for(int i=0; i < children->size(); i++)
        {
            Edge e = (*children)[i];
            State* to = e.getTo();
            cout << "My Child " << to->get_state_num();
            cout << "  length of weight : " << e.get_weight().length();
            cout << "  weight : " << e.get_weight() << endl;
            if(!to->get_visit())
            {
                current.push(to);
            }
        }
    }
    //cout<< "clearing" << endl;
    g->clear_visited();
    //cout << "ended printing" <<endl;
    //cout << endl;
    //cout << endl;
}

vector<string> GrammerParser::get_weights()
{
    return weights;
}

void GrammerParser::setWeights(Graph* g)
{
    State* oldst = g->get_start_state();
    queue<State*> current;
    current.push(oldst);
    while(!current.empty())
    {
        State* tmp = current.front();
        current.pop();
        if(tmp->get_visit())
        {
            continue;
        }
        tmp->set_visit(true);
        vector<Edge>* children = tmp->get_children();
        for(int i=0; i < children->size(); i++)
        {
            Edge e = (*children)[i];
            State* to = e.getTo();
            current.push(to);
            string weight = e.get_weight();
            if(weight.length() > 0)
            {
                if(!check_for_weight(weight))
                {
                    weights.push_back(weight);
                }
            }
        }
    }
    g->clear_visited();
}

bool GrammerParser::check_for_weight(string weight)
{
    if(weight.length() == 0)
    {
        return true;
    }
    unordered_map<string,Graph*>::const_iterator found = definitions.find(weight);
    if(found != definitions.end())
    {
        return true;
    }
    for(int i = 0; i< weights.size(); i++)
    {
        if(!weight.compare(weights[i]))
        {
            return true;
        }
    }
    return false;
}

vector<string> GrammerParser::get_expressions()
{
    return expNames;
}

GrammerParser::~GrammerParser()
{
    //dtor
}
