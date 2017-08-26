#include "BFS.h"

#define sletter "a-z"
#define cletter "A-Z"
#define digit "0-9"
BFS::BFS(Graph* g, string word)
{
    State* start = g ->get_start_state();
    queue<State*> current;
    current.push(start);
    for(int k=0; k < word.length(); k++)
    {
        char c = word[k];
        string cs;
        cs.push_back(c);
        int cursize = current.size();
        for(int j=0; j<cursize; j++)
        {
            State* s = current.front();
            current.pop();
            vector<Edge>* children =  s->get_children();
            int childsize = children->size();
            for(int i = 0; i < childsize; i++)
            {
                Edge e = (*children)[i];
                string weight = e.get_weight();
                if(weight.compare(sletter) && islower(c))
                {
                    current.push(e.getTo());
                }
                else if (weight.compare(cletter) && isupper(c))
                {
                    current.push(e.getTo());
                }
                else if (weight.compare(digit) && isdigit(c))
                {
                    current.push(e.getTo());
                }
                else if (weight.compare(cs))
                {
                    current.push(e.getTo());
                }
            }
        }
    }
    /*  // Constructor
      // vistaed array of size = # of states in the graph
      bool visited[ g->get_graph_size()];
      // mark all visited  = false
      for(int i=0; i < g->get_graph_size(); i++)
          visited[i] = false;
      // ToDo BFS using Queue
      State* start = g->get_start_state();
      visited_states.push_back(*start);
      vector<Edge> *children = start->get_children();
      visited[start->get_state_num()] = true;
      // push all the children of the start state in the queue
      for(int i=0; i < children->size(); i++)
          q.push((*children)[i]);

      while(q.size()!=0)
      {
          Edge edge = q.front();
          q.pop();
          visited_edges.push_back(edge);
          visited_edges_weights.push_back(edge.get_weight());
          pair<State*, State*> p = edge.get_start_end_states();
          if( visited[p.second->get_state_num()] )
              continue;
          visited[p.second->get_state_num()] = true;
          visited_states.push_back(*p.second);
          children = p.second->get_children();
          // push all children in the queue
          for(int i=0; i<children->size(); i++)
          {
              Edge child = (*children)[i];
              q.push((*children)[i]);
          }
      }*/
}

/*
 * Get all the visited states in the BFS
 */
vector <State> BFS::get_visited_states()
{
    return visited_states;
}
/*
 * Get all the visited edges in the BFS
 */
vector <Edge> BFS::get_visited_edges()
{
    return visited_edges;
}
/*
 * Get all the values of the visited edges in the BFS
 */
vector <string> BFS::get_visited_edges_weights()
{
    return visited_edges_weights;
}

BFS::~BFS()
{
    // Destructor
}

