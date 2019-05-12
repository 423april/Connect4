#ifndef TREENODES_H_INCLUDED
#define TREENODES_H_INCLUDED

#include <iostream>
#include "ConnectFourBoard (2).h"
using namespace std;

extern int num_nodes_generated,num_nodes_expanded,game_path_length,start_time,stop_time;

class tree{
    
public:
    
    //해당 노드의 heuristic_value를 저장한다.
    int heuristic_value;
    /* 각 child에 대응하여 7번 돌기 위해 만든 변수
     */
    int num_children;
    //7개의 자식 노드로의 포인터 변수
    tree *children[7];
    //이 노드를 gaming tree에서 더 확장시킬 것인지 여부를 나타내는 변수
    bool use;
    /*board configuration 담은 변수
     */
    tic *ob;
    
    
    //Member functions of tree class
    tree();
    char ReversePlayer(char player);
    void create_node(char);
    void set_heuristic_value(int);
    void add_all_children();
    bool deep_enough(int);
    int innerEval(char, char, char, char, char, char);
    int evaluation(char);
};
#endif // TREENODES_H_INCLUDED
