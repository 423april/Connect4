#ifndef ALPHABETA_H_INCLUDED
#define ALPHABETA_H_INCLUDED

#include <iostream>
#include "TreeNodes (2).h"
using namespace std;

class alphabeta{
    int moves_made;
public:
    
    //alphabeta class의 member functions
    alphabeta();
    //void countmoves();
    int alpha_beta(tree*, int, char, int, int, char);
//rr    
    
};
#endif // ALPHABETA_H_INCLUDED
