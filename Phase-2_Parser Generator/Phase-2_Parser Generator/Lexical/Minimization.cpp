#include "Minimization.h"


Minimization::Minimization(unordered_map<string, int> tokensPriorities)
{
    this->tokensPriorities = tokensPriorities;
}

vector< vector <DFA_State> > Minimization:: minimize_DFA (vector < vector <DFA_State> > DFA,vector <DFA_State> states)
{

    vector < vector <DFA_State> > sets;

    int acceptanceNum = tokensPriorities.size();

    //First partition to final and non-final states
    vector <DFA_State> finalStates [acceptanceNum];
    vector <DFA_State> nonFinalStates;

    for (int i = 0; i < states.size(); i++) {
        if (!states[i].is_accepting_state()) {
            nonFinalStates.push_back(states[i]);
        }
        else {
            int idx = tokensPriorities[states[i].get_accepted_token()];
            finalStates[idx].push_back(states[i]);
        }
    }

    //Add first two sets
    for (int i = 0; i < acceptanceNum; i++) {
        sets.push_back(finalStates[i]);
    }
    sets.push_back(nonFinalStates);

    //Use state number as key associated with value of set number
    map <int, int> setNumber;

    for (int i = 0; i < acceptanceNum; i++)
    {
        for (int j = 0; j < finalStates[i].size(); j++) {
            setNumber.insert(pair<int, int> (finalStates[i][j].get_state_num(), i));
        }
    }
    for (int i = 0; i < nonFinalStates.size(); i++)
    {
        setNumber.insert(pair<int, int> (nonFinalStates[i].get_state_num(), acceptanceNum));
    }

    bool proceed = true;

    while (proceed)
    {

        int setsSize = sets.size();
        proceed = false;

        //Iterate over all sets
        for (int setIdx = 0; setIdx < setsSize; setIdx++)
        {
            //Iterate over all elements in set
            for (int elemIdx = 0; elemIdx < sets[0].size(); elemIdx++)
            {

                vector <DFA_State> currentSet;
                currentSet.push_back(sets[0][elemIdx]);

                //Iterate over next elements in the same set
                for (int elem2Idx = elemIdx+1; elem2Idx < sets[0].size(); elem2Idx++)
                {

                    int firstElement = sets[0][elemIdx].get_state_num();
                    int secondElement = sets[0][elem2Idx].get_state_num();
                    bool failed = false;

                    //Iterate over all alphabet of DFA
                    for (int alpha = 0; alpha < DFA[0].size(); alpha++)
                    {
                        //Check if transitions are in the same set
                        int firstTransition = DFA[firstElement][alpha].get_state_num();
                        int secondTransition = DFA[secondElement][alpha].get_state_num();
                        if (setNumber[firstTransition] != setNumber[secondTransition])
                        {
                            failed = true;
                            break;
                        }
                    }

                    if (!failed)
                    {
                        //The two tested states belong to the same set
                        currentSet.push_back(sets[0][elem2Idx]);

                        //Remove set's excluded element
                        sets[0].erase(sets[0].begin()+elem2Idx);

                        elem2Idx--;
                        //proceed = false;
                    }
                    else
                    {
                        proceed = true;
                    }
                }

                //Update set numbers
                for (int i = 0; i < currentSet.size(); i++)
                {
                    setNumber[currentSet[i].get_state_num()] = sets.size();
                }

                //Add current set to new level sets
                sets.push_back(currentSet);

                //Remove the processed element
                sets[0].erase(sets[0].begin()+elemIdx);

                elemIdx--;
            }

            //Remove the processed set
            sets.erase(sets.begin());
        }
    }

    //Use first element in each set as a representative
    map <int, int> stateRepresentatives;

    for(int i = 0 ; i < sets.size() ; i++)
    {
        int setSize = sets[i].size();
        if (setSize == 1)
        {
            stateRepresentatives.insert(pair<int, int> (sets[i][0].get_state_num(), sets[i][0].get_state_num()));
        }
        else
        {
            for (int j = 0; j < setSize; j++)
            {
                stateRepresentatives.insert(pair<int, int> (sets[i][j].get_state_num(), sets[i][0].get_state_num()));
            }
        }
    }

    map <int, int> newMatch;
    int counter = 0;
    for (int i = 0; i < stateRepresentatives.size(); i++) {
        if (stateRepresentatives[i] == i) {
            newMatch.insert(pair<int, int> (i, counter));
            counter++;
        }
    }

    //Generate minimized DFA
    counter = 0;
    //Get new minimized states
    for (int i = 0; i < DFA.size(); i++)
    {
        if (stateRepresentatives[i] != i)
        {
            continue;
        }
        DFA_State tempState(DFA.size());
        tempState.set_state_num(counter);
        if (states[i].is_accepting_state())
        {
            tempState.set_accepting(true, states[i].get_accepted_token());
        }
        min_states.push_back(tempState);
        counter++;
    }

    for (int i = 0; i < DFA.size(); i++)
    {
        vector <DFA_State> row ;
        if (stateRepresentatives[i] != i)
        {
            continue;
        }

        for(int j = 0 ; j < DFA[i].size() ; j++)
        {
            int state = DFA[i][j].get_state_num();
            //Empty transition
            if (state == -1)
            {
                DFA_State emptyState(DFA.size());
                emptyState.set_state_num(-1);
                row.push_back(emptyState);
            }
            else
            {
                int stateNum = newMatch[stateRepresentatives[state]];
                row.push_back(min_states[stateNum]);
                //row.push_back(min_states[stateRepresentatives[state]]);
            }
        }

        min_DFA.push_back(row);
    }


    tables.open("Lexical/tables.txt");

    tables << "DFA transition table :" << endl << "*******************************" << endl;
    printTransitionTable(DFA);

    tables << "*************************************************************************************************************" << endl;

    tables << "Minimized states :" << endl << "*******************************" << endl;
    for (int i = 0; i < sets.size(); i++) {
        for (int j = 0; j < sets[i].size(); j++) {
            tables << sets[i][j].get_state_num() << " ";
        }
        tables << endl;
    }

    tables << "*************************************************************************************************************" << endl;

    tables << "Minimized DFA table :" << endl << "*******************************" << endl;
    printTransitionTable(min_DFA);

    return min_DFA;
}

void Minimization::printTransitionTable(vector< vector<DFA_State> > table)
{

    const int width = 5;
    for (int i = 0; i < table.size(); i++)
    {
        printTable(i, width);
        printTable('|', 2);
        for (int j = 0; j < table[i].size(); j++)
        {
            printTable(table[i][j].get_state_num(), width);
        }
        tables << endl;
    }
}

template<typename T> void Minimization::printTable (T t, int width)
{

    const char separator = ' ';
    tables << left << setw(width) << setfill(separator) << t;
}

vector <DFA_State> Minimization::get_minimum_states()
{

    return min_states;
}
Minimization::~Minimization()
{
    //Empty destructor
}
