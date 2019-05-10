#include <iostream>

#include "AlphaBeta.h"
#include "TreeNodes.h"
#include "ConnectFourBoard.h"
using namespace std;

//기능이 없는 생성자-독고준석-
alphabeta::alphabeta(){
    eval_choice = ' ';
}

//플레이어를 바꿔주기 위한 함수이다. X이면 O로, O면 X로.-이수민-
char alphabeta::ReversePlayer(char player){
    if(player == 'X')
        return 'O';
    else
        return 'X';
}

/* 이 함수는 교재에서 주어진 알고리즘으로부터 실행된다. 만약 player가 이 함수에 의해 리턴된 move를 한다면 이 함수는 미래에 더 이길 가능성이 높은 노드를 찾는데 이용된다.
 3 경우가 있다. 
1st 기본 경우
2nd player ‘X’에 대해 재귀적인 경우 i.e Max player
3rd player 'Y'에 대해 재귀적인 경우 i.e Min player
-독고준석-
 */
int alphabeta::alpha_beta(tree* tob, int depth, char player, int alpha, int beta){
    
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
        return beta;
    }
    
}
