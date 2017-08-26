#include "Tokens.h"

Tokens::Tokens (vector <vector <DFA_State> > table, map <char, int> alphabet, vector <DFA_State> states,string infile)
{

    this->transitionTable = table;
    this->alphabet = alphabet;
    this->states = states;
    this->currentStateNum = 0;

    inputFile.open(infile);
    outputFile.open("Lexical/outputFile.txt");
    errorFile.open("Lexical/errorFile.txt");
    symbolFile.open("Lexical/symbolFile.txt");
}

string Tokens::getToken ()
{

    char readChar;
    while ((readChar = check_char()) != '\0')
    {

        //Case spaces or tabs
        if (readChar == ' ' || readChar == '\t' || readChar == '\n')
        {
            //Last state was accepting state
            if (!input.empty())
            {
                if (input.back().first.is_accepting_state())
                {
                    currentStateNum = 0;
                    //Construct the token
                    string token = "";
                    string accepted = input.back().first.get_accepted_token();
                    int tokenLength = input.size();

                    for (int i = 0; i < tokenLength; i++)
                    {
                        token = input.back().second + token;
                        input.erase(input.begin()+input.size()-1);
                    }
                    symbolFile << token << endl;
                    outputFile << accepted << endl;
                    return token;
                }
            }
            continue;
        }

        //Check incoming character in alphabet
        int alphaIdx = get_alpha_idx(readChar);

        //Invalid input character
        if (alphaIdx == -1)
        {
            errorFile << readChar << endl;
            continue;
        }
        else
        {
            //Check if there is another path from current state
            if (transitionTable[currentStateNum][alphaIdx].get_state_num() != -1)
            {
                DFA_State tempState = transitionTable[currentStateNum][alphaIdx];
                currentStateNum = tempState.get_state_num();
                input.push_back(pair <DFA_State, char> (tempState, readChar));
            }
            else
            {
                //Did not find valid state then return to nearest valid state
                readHistory.push(pair <DFA_State, int> (states[currentStateNum], readChar));
                currentStateNum = 0;
                while (!input.empty() && !input.back().first.is_accepting_state())
                {
                    readHistory.push(input.back());
                    input.erase(input.begin()+input.size()-1);
                }

                //If the input is empty then no accepting states found
                if (input.empty())
                {
                    char error = readHistory.top().second;
                    readHistory.pop();
                    errorFile << error << endl;
                }
                else
                {
                    //Found an accepting state while backtracking
                    string token = "";
                    string accepted = input.back().first.get_accepted_token();
                    int tokenLength = input.size();
                    for (int i = 0; i < tokenLength; i++)
                    {
                        token = input.back().second + token;
                        input.erase(input.begin()+input.size()-1);
                    }
                    symbolFile << token << endl;
                    outputFile << accepted << endl;
                    return token;
                }
            }
        }
    }

    if (input.empty())
    {
        string token = "";
        return token;
    }
    else
    {
        while (!input.empty() && !input.back().first.is_accepting_state())
        {
            readHistory.push(input.back());
            input.erase(input.begin()+input.size()-1);
        }
        currentStateNum = 0;
        if (input.empty())
        {
            char error = readHistory.top().second;
            readHistory.pop();
            errorFile << error << endl;
            return getToken();
        }
        else
        {
            //Found an accepting state while backtracking
            string token = "";
            string accepted = input.back().first.get_accepted_token();
            int tokenLength = input.size();
            for (int i = 0; i < tokenLength; i++)
            {
                token = input.back().second + token;
                input.erase(input.begin()+input.size()-1);
            }
            symbolFile << token << endl;
            outputFile << accepted << endl;
            return token;
        }
    }
}

char Tokens::check_char ()
{

    //Check if there is an input left in the stack
    if (!readHistory.empty())
    {
        char readC = readHistory.top().second;
        readHistory.pop();
        return readC;
    }
    else
    {
        char readC = -1;
        //read without skipping white spaces
        inputFile >> noskipws >> readC;
        if (readC >= 0)
        {
            return readC;
        }
        else
        {
            return '\0';
        }
    }
}

int Tokens::get_alpha_idx (char test)
{

    if (alphabet.find(test) == alphabet.end())
    {
        return -1;
    }
    return alphabet[test];
}

Tokens::~Tokens()
{
    inputFile.close();
    outputFile.close();
    errorFile.close();
}
