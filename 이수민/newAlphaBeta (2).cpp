#include <iostream>
#include <climits>

#include "AlphaBeta (2).h"
#include "TreeNodes (2).h"
#include "ConnectFourBoard (2).h"
using namespace std;

//생성자
alphabeta::alphabeta(){
    
}


/*  기여: 이수민
    강의자료에서 주어진 알파베타 프루닝 알고리즘을 기반으로 만들어진 함수이다. 이 함수는 게이밍 트리를 생성하여 말단 노드의 평가함수값을 계산하고
    말단으로부터 위로 올라가며 maximizing node에서는 child node의 평가함수값 중 가장 큰 것을 가져오고, minimizing node에서는 child node의
    평가함수 중 가장 작은 것을 가져온다. alpha는 maximinzing player, 여기서는 AI player가 보장할 수 있는 최솟값이고,
    beta는 minimizing player, 여기서는 user player가 보장할 수 있는 최댓값이다.
 
 입력값:
 tob: 현재 node
 depth: 현재 얼만큼 깊이 들어와있는지
 player: alpha_beta를 시작한 root node의 board player
 alpha: AI player가 보장할 수 있는 최솟값
 beta: user player가 보장할 수 있는 최댓값
 aiplayer: 평가함수를 어느 player를 기준으로 계산해야하는지, AI의 player를 알려주는 인자.
 */
int alphabeta::alpha_beta(tree* tob, int depth, char player, int alpha, int beta, char aiplayer){

    //지정한 depth를 도달했거나, 어느 플레이어가 이기는 node에서 평가함수를 계산한다.
    if(tob->deep_enough(depth) && tob->use){
        tob->heuristic_value = tob->evaluation(aiplayer);
//        cout<<"terminal node: "<<endl;
//        cout<<tob->heuristic_value<<endl;
//        tob->ob->display_board(tob->ob);
        return tob->evaluation(aiplayer);
    }

    

    
    //현재의 player가 root node의 player와 같다면 maximizing node이고, 가장 높은 heuristic value를 가지는 child node를 선택해야한다.
    if(tob->ob->player == player){
//        tob->ob->display_board(tob->ob);
//        cout<<tob->heuristic_value<<endl;
        int val = INT_MIN;
        for(int i = 0; i < 7; i++){
            if(tob->children[i] != NULL && tob->children[i]->use && tob->children[i]->ob->moves[i].row_index != -1){
                int temp = alpha_beta(tob->children[i], depth+1, player, alpha, beta, aiplayer);
                val = (temp > val)? temp: val;
                alpha = (val > alpha)?val:alpha;
                if(alpha >= beta)
                    break; //cutoff
            }
        }
        tob->heuristic_value = alpha;
        return alpha;
    }
    ///현재의 player가 root node의 player와 다르다면 minimizing node이고, 가장 작은 heuristic value를 가지는 child node를 선택해야한다.
    else{
//        tob->ob->display_board(tob->ob);
//        cout<<tob->heuristic_value<<endl;
        int val = INT_MAX;
        for(int i = 0; i < 7; i++){
            if(tob->children[i] != NULL && tob->children[i]->use && tob->children[i]->ob->moves[i].row_index != -1){
                int temp = alpha_beta(tob->children[i], depth+1, player, alpha, beta, aiplayer);
                val = (temp < val)? temp: val;
                beta = (val < beta)? val: beta;
                if(beta <= alpha)
                    break; //cutoff
            }
        }
        tob->heuristic_value = beta;
        return beta;
    }
    
}
