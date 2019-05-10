//
//  newmain.cpp
//  connect4
//
//  https://github.com/0bit093/Connect_Four-Game 의 공개코드를 참고했다. 해당 코드의 트리 구조와 게임 구동 방식을 이용했다. 
//  차이점은 해당 코드는  AI vs AI의 형식으로 되어있었던 반면 과제는 AI vs Human으로 진행을 해야했기 때문에 이것이 가능하도록 수정했다. 
//  첫번째 착수에 가운데열에 돌을 두지 못하도록 수정했다. 
//

#include <iostream>
#include <ctime>

#include "TreeNodes.h"
#include "ConnectFourBoard.h"
#include "AlphaBeta.h"
using namespace std;

/*TreeNodes.h 와 .cpp와 main 함수에 의해 접근 가능한 변수-독고준석-
 */
int num_nodes_generated, num_nodes_expanded,game_path_length,start_time,stop_time;

/*전역 변수인데 그 이유는
 Minmax() 와 AlphaBeta()에 접근 가능해야하기 때문이다.
-독고준석-
 */
char evalfn_choiceX,evalfn_choiceO;

int main(){
    
    int order;
    int choiceX,choiceO;
    
    cout<<"선공하시겠습니까? 후공하시겠습니까?"<<endl;
    cout<<"  1 - 선공(X)"<<endl;//1이면 선공-독고준석-
    cout<<"  2 - 후공(O)"<<endl;//2이면 후공-독고준석-
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
    board_ob->player = 'X'; //선공은 항상 X-이수민-
    
    alphabeta ab_ob;
    //이긴 경우가 있는지 체크, 없으면 계속 while문 돎-독고준석-
    while(!board_ob->checkPlayerWon()){
        
        tree *head = new tree;
        *(head->ob) = *board_ob;
        //보드가 비었는지 확인, 비었으면 진행-독고준석-
        if(head->ob->isBoardEmpty()){
            if(choiceX == 0 && moves_made == 0){
                int c;
                cout<<"User's turn!"<<endl;
                    cout<<"Fisrt move cannot be column 3!"<<endl;
                    cout<<"select column: ";
                    cin>>c;//원하는 열을 입력 받음-독고준석-
                    cout<<endl;
                    while(c==3){//만약 3을 누르면 다시 입력 받음-독고준석-
                        cout<<"Select again: ";
                        cin>>c;
                        cout<<endl;
                    }

                head->ob->board[0][c] = 'X';//보드에 해당하는 열에 'X'표시-독고준석-
                head->ob->player = 'X';
                
            }
            else if(choiceX == 1 && moves_made == 0){//AI가 선공-이수민-
                int random_col_index = head->ob->makeFirstMove();
                head->ob->board[0][random_col_index] = 'X';
                head->ob->player = 'X';
            }
            //보드 출력-이수민-
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            //다음 순서인 O로 플레이어 바꿔줌-이수민-
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
            
            //보드 출력-이수민-
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            
            //이겼는지 확인-이수민-
            if(head->ob->checkPlayerWon()){
                cout<<"You won!"<<endl;
                return 0;
            }
            
            //다음 순서인 O로 플레이어 바꿔줌-이수민-
            head->ob->player = 'O';
            *(board_ob) = *(head->ob);
        }

        if(head->ob->player == 'X' && choiceX == 1 && moves_made >= 1){//AI 차례, 'X'-이수민-
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
            
            //이겼는지 확인-이수민-
            if(head->ob->checkPlayerWon()){
                cout<<"player "<<head->ob->player<<" won!"<<endl;
                return 0;
            }
            
            head->children[maxindex]->ob->player = 'O';
            *(board_ob) = *(head->children[maxindex]->ob);
        }

        if(head->ob->player == 'O' && choiceO == 0 && moves_made >= 1){//user 차례, 'O'-이수민-
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
            
            //보드 출력-이수민-
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            
            //이겼는지 확인-이수민-
            if(head->ob->checkPlayerWon()){
                cout<<"You won!"<<endl;
                return 0;
            }
            //다음 순서인 X로 플레이어 바꿔줌-이수민-
            head->ob->player = 'X';
            *(board_ob) = *(head->ob);
            
        }

        if(head->ob->player == 'O' && choiceO == 1 && moves_made >= 1){//AI 차례, 'O'-이수민-
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
            
            //이겼는지 확인-이수민-
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
