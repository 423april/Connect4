//
//  newmain.cpp
//  connect4
//
//  original code from https://github.com/0bit093/Connect_Four-Game
//

#include <iostream>
#include <ctime>
#include <climits>

#include "TreeNodes (2).h"
#include "ConnectFourBoard (2).h"
#include "AlphaBeta (2).h"

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
    
    /* user가 선공을 고르면 X가 user, O가 AI가 된다.
       user가 후공을 고르면 X가 AI, O가 user가 된다.
     */
    
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
            //head->ob->player = 'O';
            *(board_ob) = *(head->ob);
            
        }

        if(head->ob->player == 'O' &&  choiceX == 0 && moves_made >= 1){//user 차례 'X'
            int c;
            int row, col;
            
            //둘 수 없는 자리를 선택했을 경우를 걸러내기 위해 row == -1일때 열을 다시 선택하도록 한다.
            do{
                cout<<"User's turn!"<<endl;
                cout<<"Select Column: ";
                cin>>c;
                cout<<endl;
                head->ob->checkPossibleMoves();
                for(int i = 0; i < 7; i++){
                    if(head->ob->moves[i].col_index == c){
                        row = head->ob->moves[i].row_index;
                        col = head->ob->moves[i].col_index;
                    }
                }
            }while(row == -1);
            
            //선택한 자리에 X를 두고, 플레이어도 바꿔준다.
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
            //보드 업데이트
            *(board_ob) = *(head->ob);
        }

        if(head->ob->player == 'O' && choiceX == 1 && moves_made >= 1){//AI 차례, 'X'
            int max = INT_MIN;
            int maxindex = -1;
            cout<<"AI turn"<<endl;
            clock_t begin = clock();
            
            //알파베타 함수를 실행해서 child node들의 heuristic value를 얻는다.
            //ab_ob.countmoves();
            int x = ab_ob.alpha_beta(head,0,head->ob->player,INT_MIN,INT_MAX, 'X');
            
            //앞에서 알파베타 함수를 실행해서 얻은 child node들의 heuristic value 중, 가장 높은 값을 가진 child를 선택한다.
            for(int i = 6; i >= 0; i--){
                int tempheur = head->children[i]->heuristic_value;
                cout<<i<<": "<<tempheur<<endl;
                if(tempheur >= max && head->children[i]->ob->moves[i].row_index != -1){
                    max = tempheur;
                    maxindex = i;
                }
            }
            clock_t  finish = clock();
            
            cout<<"----------------------------->>>"<<endl;
            double elapsed_secs = double(finish - begin) / CLOCKS_PER_SEC;
            cout<<"걸린 시간: "<<elapsed_secs<<"초"<<endl;
            cout<<"AI가 선택한 열은: ";
            cout<<head->children[maxindex]->ob->moves[maxindex].col_index<<endl;
            cout<<"이번 회차에서 둘 수 있는 수에 대한 평가함수 값이 각각 다음과 같다"<<endl;
            for(int k = 0; k < 7; k++){
                cout<<"열 "<<k<<"의 평가함수 값: "<<head->children[k]->heuristic_value<<endl;
            }
            cout<<maxindex<<"의 평가함수 값이 "<<head->children[maxindex]->heuristic_value<<"로 제일 높기 때문에 선택했다."<<endl;
            cout<<"상세한 이유: "<<head->children[maxindex]->str<<endl;
            head->ob->display_board(head->children[maxindex]->ob);
            
            //이겼는지 확인
            if(head->children[maxindex]->ob->checkPlayerWon()){
                cout<<"AI won!"<<endl;
                return 0;
            }
            
            
            cout<<"----------------------------->>>"<<endl;
            
            //보드 업데이트
            *(board_ob) = *(head->children[maxindex]->ob);
        }

        if(head->ob->player == 'X' && choiceO == 0 && moves_made >= 1){//user 차례, 'O'
            int c;
            int row, col;
            
             //둘 수 없는 자리를 선택했을 경우를 걸러내기 위해 row == -1일때 열을 다시 선택하도록 한다.
            do{
                cout<<"User's turn!"<<endl;
                cout<<"Select Column: ";
                cin>>c;
                cout<<endl;
                head->ob->checkPossibleMoves();
                for(int i = 0; i < 7; i++){
                    if(head->ob->moves[i].col_index == c){
                        row = head->ob->moves[i].row_index;
                        col = head->ob->moves[i].col_index;
                    }
                }
            }while(row == -1);
            
            //선택한 자리에 X를 두고, 플레이어도 바꿔준다.
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
            //보드 업데이트
            *(board_ob) = *(head->ob);
            
        }

        if(head->ob->player == 'X' && choiceO == 1 && moves_made >= 1){//AI 차례, 'O'
            int max = INT_MIN;
            int maxindex = -1;
            cout<<"AI turn"<<endl;
            clock_t begin = clock();
            
            //알파베타 함수를 실행해서 child node들의 heuristic value를 얻는다.
            int x = ab_ob.alpha_beta(head,0,head->ob->player,INT_MIN,INT_MAX, 'O');
            
            //앞에서 알파베타 함수를 실행해서 얻은 child node들의 heuristic value 중, 가장 높은 값을 가진 child를 선택한다.
            for(int i = 6; i >= 0; i--){
                int tempheur = head->children[i]->heuristic_value;
                cout<<i<<": "<<tempheur<<endl;
                if(tempheur >= max && head->children[i]->ob->moves[i].row_index != -1){
                    max = tempheur;
                    maxindex = i;
                }
            }
            clock_t  finish = clock();
            
            cout<<"----------------------------->>>"<<endl;
            double elapsed_secs = double(finish - begin) / CLOCKS_PER_SEC;
            cout<<"걸린 시간: "<<elapsed_secs<<"초"<<endl;
            cout<<"AI가 선택한 열은: ";
            cout<<head->children[maxindex]->ob->moves[maxindex].col_index<<endl;
            cout<<"이번 회차에서 둘 수 있는 수에 대한 평가함수 값이 각각 다음과 같다"<<endl;
            for(int k = 0; k < 7; k++){
                cout<<"열 "<<k<<"의 평가함수 값: "<<head->children[k]->heuristic_value<<endl;
            }
            cout<<maxindex<<"의 평가함수 값이 "<<head->children[maxindex]->heuristic_value<<"로 제일 높기 때문에 선택했다."<<endl;
            cout<<"상세한 이유: "<<head->children[maxindex]->str<<endl;
            head->ob->display_board(head->children[maxindex]->ob);
            
            //이겼는지 확인
            if(head->children[maxindex]->ob->checkPlayerWon()){
                cout<<"AI won!"<<endl;
                return 0;
            }
            
            
            cout<<"----------------------------->>>"<<endl;
            
            //보드 업데이트
            *(board_ob) = *(head->children[maxindex]->ob);
        }
       
   
        moves_made++;
        cout<<"moves made: "<<moves_made<<endl;
        game_path_length++;
    }
    
    return 0;
}
