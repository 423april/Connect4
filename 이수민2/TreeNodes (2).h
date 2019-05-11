#ifndef TREENODES_H_INCLUDED
#define TREENODES_H_INCLUDED

#include <iostream>
#include "ConnectFourBoard (2).h"
using namespace std;

extern int num_nodes_generated,num_nodes_expanded,game_path_length,start_time,stop_time;

class tree{
    
public:
    
    //stores the heuristic value of the node i.e board
    int heuristic_value;
    /*to iterate through the children of a node and
     there are exactly 7 possible children for each node
     */
    int num_children;
    //this tores the pointers to its 7 children nodes
    tree *children[7];
    /*an object from the class tic which holds the
     board configuration of each node
     */
    tic *ob;
    
    //Member functions of tree class
    tree();
    char ReversePlayer(char player);
    void create_node(char);
    void set_heuristic_value(int);
    void add_all_children();
    bool deep_enough(int);
    int evaluation_m();
    void copy_board_status(tic &);
    void helper();
    void display_contents();
    int getOptimalNode();
    void move_gen(tic *,int);
};
#endif // TREENODES_H_INCLUDED
