#include "Matcher.h"

Matcher::Matcher( vector < vector <string> > table ,unordered_map<string , int> *terminals , unordered_map<string , int> *non_terminals,string start_symbol) {

    this->table = table;
    this->terminals = terminals;
    this->non_terminals = non_terminals;
    this->start_symbol = start_symbol;
}

vector<string> Matcher::start_matcher(vector <string>  tokens) {

    vector <string> output;
    stack <string> validation_stack;
    validation_stack.push("$");
    validation_stack.push(start_symbol);
    int tokens_size = tokens.size();
    int tokens_idx = 0;

    while (validation_stack.size() > 0 && tokens.size() > tokens_idx) {
        //Found a match
        if (validation_stack.top() == tokens[tokens_idx]) {
            validation_stack.pop();
            tokens_idx ++ ;
            continue;
        }

        //Try to find entry in table
        int row = (*non_terminals)[validation_stack.top()] -1;
        int col = (*terminals)[tokens[tokens_idx]] -1;

        //Found terminal on top of the stack that does not match input token
        if (row == -1) {
            output.push_back("Error: illegal missing "+validation_stack.top());
            validation_stack.pop();
        }
        else if (row != -1 && col != -1) {
            //Found a table entry
            string production = table[row][col];

            if (production == "") {
                //Empty entry which is error
                output.push_back("Error: illegal discard "+tokens[tokens_idx]);
                tokens_idx ++;
            }
            else if (production == "Synch") {
                //Found sync
                validation_stack.pop();
            }
            else if (production == "\\L") {
                //Fount epsilon
                output.push_back(validation_stack.top()+" -> "+production);
                validation_stack.pop();
            }
            else {
                output.push_back(validation_stack.top()+" -> "+production);
                validation_stack.pop();
                //Update the stack
                stringstream s_stream(production);
                istream_iterator<string> start(s_stream);
                istream_iterator<string> end;
                vector<string> temp_productions (start, end);
                for (int i = temp_productions.size()-1; i >= 0; i--) {
                    validation_stack.push(temp_productions[i]);
                }
            }
        }
        else {
            //That's no match for the input
            output.push_back("Not acceptance");
            return output;
        }
    }

    if (!validation_stack.empty()) {
        //Stack is not empty but no input symbols exist
        do {
            output.push_back("Error: illegal missing "+validation_stack.top());
            validation_stack.pop();
        } while (!validation_stack.empty());
    }
    else if (tokens_idx < tokens.size()) {
        //Input not consumed entirely
        do {
            output.push_back("Error: illegal discard "+tokens[tokens_idx]);
            tokens_idx ++ ;
        } while (tokens_idx < tokens.size());
    }

    output.push_back("Accepted");
    return output;
}

Matcher::~Matcher() {
    //Empty destructor
}
