#include <iostream>

#include "AlphaBeta (2).h"
#include "TreeNodes (2).h"
#include "ConnectFourBoard (2).h"
using namespace std;

//a constructor with no functionality
alphabeta::alphabeta(){
    eval_choice = ' ';
}

char alphabeta::ReversePlayer(char player){
    if(player == 'X')
        return 'O';
    else
        return 'X';
}

/* this function is implemented from the algorithm given
 in Russell & Norvig text book.
 this function is used to find the node which is more probable to win
 in the future if the player plays the move returned by this function.
 
 in this function there are 3 cases 1st being base case , 2nd being recursive case
 for player 'X' i.e Max player and the 3rd being recursive case for player 'Y'
 i.e Min player
 
 More complete description of how this function works is explained in project report
 */
int alphabeta::alpha_beta(tree* tob, int depth, char player, int alpha, int beta){
    
    //cout<<depth<<endl;
    //gets executed if my eval function is selected by user
    if(eval_choice == 'M' || eval_choice == 'm'){
        if(tob->deep_enough(depth))
            return tob->evaluation_m();
    }
    
    //gets executed if santosh's eval fn is chosen
    else if(eval_choice == 'S' || eval_choice == 's'){
        if(tob->deep_enough(depth))
            return tob->evaluation_s();
    }
    
    //gets executed if harsha's eval fn is used
    else{
        if(tob->deep_enough(depth))
            return tob->evaluation_h();
    }
    
    //maximizing player
    if(tob->ob->player == player){
        for(int i = 0; i < 7; i++){
            if(tob->children[i] != NULL){
                int val = alpha_beta(tob->children[i], depth+1, ReversePlayer(player), alpha, beta);
                alpha = (val > alpha)?val:alpha;
                if(alpha >= beta)
                    break; //cutoff
            }
        }
        tob->heuristic_value = alpha;
        //cout<<alpha<<endl;
        return alpha;
    }
    //minimizing player
    else{
        for(int i = 0; i < 7; i++){
            if(tob->children[i] != NULL){
                int val = alpha_beta(tob->children[i], depth+1, ReversePlayer(player), alpha, beta);
                beta = (val < beta)? val: beta;
                if(beta <= alpha)
                    break; //cutoff
            }
        }
        tob->heuristic_value = beta;
        //cout<<beta<<endl;
        return beta;
    }
    
}
