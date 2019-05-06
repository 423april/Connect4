//
//  newmain.cpp
//  connect4
//
//  Created by April Lee on 03/05/2019.
//  Copyright © 2019 April Lee. All rights reserved.
//

#include <iostream>
#include <ctime>

#include "TreeNodes.h"
#include "ConnectFourBoard.h"
#include "AlphaBeta.h"
using namespace std;

/*variable that can be accessed by files
 TreeNodes.h and .cpp, and main function
 */
int num_nodes_generated, num_nodes_expanded,game_path_length,start_time,stop_time;

/*made as a global variable since it must be accessible to
 Minmax() fn and also AlphaBeta() fn
 */
char evalfn_choiceX,evalfn_choiceO;

int main(){
    
    int order;
    int choiceX,choiceO;
    
    cout<<"선공하시겠습니까? 후공하시겠습니까?"<<endl;
    cout<<"  1 - 선공(X)"<<endl;
    cout<<"  2 - 후공(O)"<<endl;
    cout<<endl;
    cin>>order;
    cout<<endl;
    
    if(order == 1){
        choiceX = 0; //user bit = 0
        choiceO = 1;
    }else{
        choiceX = 1;
        choiceO = 0;
    }
    
    int moves_made = 0;
    tic *board_ob = new tic;
    
    cout<<endl;
    cout<<"initial board state before starting game with(Initial Node config/Root Node) "<<moves_made<<" moves made"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    
    board_ob->display_board(board_ob);
    board_ob->player = 'X'; //선공은 항상 X.
    
    alphabeta ab_ob;
    
    while(!board_ob->checkPlayerWon()){
        
        tree *head = new tree;
        *(head->ob) = *board_ob;
        
        if(head->ob->isBoardEmpty()){
            if(choiceX == 0 && moves_made == 0){
                int c;
                cout<<"User's turn!"<<endl;
                    cout<<"Fisrt move cannot be column 3!"<<endl;
                    cout<<"select column: ";
                    cin>>c;
                    cout<<endl;
                    while(c==3){
                        cout<<"Select again: ";
                        cin>>c;
                        cout<<endl;
                    }

                head->ob->board[0][c] = 'X';
                head->ob->player = 'X';
                
            }
            else if(choiceX == 1 && moves_made == 0){//AI가 선공
                int random_col_index = head->ob->makeFirstMove();
                head->ob->board[0][random_col_index] = 'X';
                head->ob->player = 'X';
            }
            //보드 출력
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            //다음 순서인 O로 플레이어 바꿔줌.
            head->ob->player = 'O';
            *(board_ob) = *(head->ob);
            
        }

        if(head->ob->player == 'X' &&  choiceX == 0 && moves_made >= 1){//user 차례 'X'
            int c;
            int row, col;
            cout<<"User's turn!"<<endl;
            cout<<"Select Column: ";
            cin>>c;
            cout<<endl;
            head->ob->checkPossibleMoves();
            for(int i = 0; i < 7; i++){
                if(head->ob->moves[i].col_index != -1 && head->ob->moves[i].col_index == c){
                    row = head->ob->moves[i].row_index;
                    col = head->ob->moves[i].col_index;
                }
            }
            head->ob->board[row][col] = 'X';
            head->ob->player = 'X';
            
            //보드 출력
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            
            //이겼는지 확인
            if(head->ob->checkPlayerWon()){
                cout<<"You won!"<<endl;
                return 0;
            }
            
            //다음 순서인 O로 플레이어 바꿔줌.
            head->ob->player = 'O';
            *(board_ob) = *(head->ob);
        }

        if(head->ob->player == 'X' && choiceX == 1 && moves_made >= 1){//AI 차례, 'X'
            int max = -2000;
            int maxindex = -1;
            cout<<"AI turn"<<endl;
            ab_ob.eval_choice = 'M';
            int x = ab_ob.alpha_beta(head,0,head->ob->player,-1000,1000);
            for(int i = 0; i < 7; i++){
                if(head->children[i]->heuristic_value > max){
                    max = head->children[i]->heuristic_value;
                    maxindex = i;
                }
            }
            head->ob->display_board(head->children[maxindex]->ob);
            
            //이겼는지 확인
            if(head->ob->checkPlayerWon()){
                cout<<"player "<<head->ob->player<<" won!"<<endl;
                return 0;
            }
            
            head->children[maxindex]->ob->player = 'O';
            *(board_ob) = *(head->children[maxindex]->ob);
        }

        if(head->ob->player == 'O' && choiceO == 0 && moves_made >= 1){//user 차례, 'O'
            int c;
            int row, col;
            cout<<"User's turn!"<<endl;
            cout<<"Select Column: ";
            cin>>c;
            cout<<endl;
            head->ob->checkPossibleMoves();
            for(int i = 0; i < 7; i++){
                if(head->ob->moves[i].col_index != -1 && head->ob->moves[i].col_index == c){
                    row = head->ob->moves[i].row_index;
                    col = head->ob->moves[i].col_index;
                }
            }
            head->ob->board[row][col] = 'O';
            head->ob->player = 'O';
            
            //보드 출력
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            
            //이겼는지 확인
            if(head->ob->checkPlayerWon()){
                cout<<"You won!"<<endl;
                return 0;
            }
            //다음 순서인 X로 플레이어 바꿔줌.
            head->ob->player = 'X';
            *(board_ob) = *(head->ob);
            
        }

        if(head->ob->player == 'O' && choiceO == 1 && moves_made >= 1){//AI 차례, 'O'
            int max = -2000;
            int maxindex = -1;
            cout<<"AI turn"<<endl;
            ab_ob.eval_choice = 'M';
            int x = ab_ob.alpha_beta(head,0,head->ob->player,-1000,1000);
            for(int i = 0; i < 7; i++){
                if(head->children[i]->heuristic_value > max){
                    max = head->children[i]->heuristic_value;
                    maxindex = i;
                }
            }
            head->ob->display_board(head->children[maxindex]->ob);
            
            //이겼는지 확인
            if(head->ob->checkPlayerWon()){
                cout<<"player "<<head->ob->player<<" won!"<<endl;
                return 0;
            }
            
            head->children[maxindex]->ob->player = 'X';
            *(board_ob) = *(head->children[maxindex]->ob);
        }
        
        moves_made++;
        cout<<moves_made<<endl;
        game_path_length++;
    }
    
    return 0;
}
