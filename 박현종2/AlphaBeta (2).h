#ifndef ALPHABETA_H_INCLUDED
#define ALPHABETA_H_INCLUDED

#include <iostream>
#include "TreeNodes (2).h"
using namespace std;

class alphabeta{
    
public:
    
    //member variables
    char eval_choice;
    
    //Member functions of alphabeta class
    alphabeta();
    char ReversePlayer(char);
    int alpha_beta(tree*, int, char, int, int);
//rr    
    
};
#endif // ALPHABETA_H_INCLUDED
