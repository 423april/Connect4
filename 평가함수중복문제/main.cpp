//
//  newmain.cpp
//  connect4
//
//  original code from https://github.com/0bit093/Connect_Four-Game
//

#include <iostream>
#include <ctime>
#include <climits>
#include "neweval.h"
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
    
    cout<<"�����Ͻðڽ��ϱ�? �İ��Ͻðڽ��ϱ�?"<<endl;
    cout<<"  1 - ����(X)"<<endl;
    cout<<"  2 - �İ�(O)"<<endl;
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
    board_ob->player = 'X'; //������ �׻� X.
    
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
            else if(choiceX == 1 && moves_made == 0){//AI�� ����
                int random_col_index = head->ob->makeFirstMove();
                head->ob->board[0][random_col_index] = 'X';
                head->ob->player = 'X';
            }
            //���� ���
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            //���� ������ O�� �÷��̾� �ٲ���.
            //head->ob->player = 'O';
            *(board_ob) = *(head->ob);
            
        }

        if(head->ob->player == 'O' &&  choiceX == 0 && moves_made >= 1){//user ���� 'X'
            int c;
            int row, col;
            
            //�� �� ���� �ڸ��� �������� ��츦 �ɷ����� ���� row == -1�϶� ���� �ٽ� �����ϵ��� �Ѵ�.
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
            
            //������ �ڸ��� X�� �ΰ�, �÷��̾ �ٲ��ش�.
            head->ob->board[row][col] = 'X';
            head->ob->player = 'X';
            
            //���� ���
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            
            //�̰���� Ȯ��
            if(head->ob->checkPlayerWon()){
                cout<<"You won!"<<endl;
                return 0;
            }
            //���� ������Ʈ
            *(board_ob) = *(head->ob);
        }

        if(head->ob->player == 'O' && choiceX == 1 && moves_made >= 1){//AI ����, 'X'
            int max = INT_MIN;
            int maxindex = -1;
            cout<<"AI turn"<<endl;
            clock_t begin = clock();
            
            //���ĺ�Ÿ �Լ��� �����ؼ� child node���� heuristic value�� ��´�.
            int x = ab_ob.alpha_beta(head,0,head->ob->player,INT_MIN,INT_MAX, 'X');
            
            //�տ��� ���ĺ�Ÿ �Լ��� �����ؼ� ���� child node���� heuristic value ��, ���� ���� ���� ���� child�� �����Ѵ�.
            for(int i = 0; i < 7; i++){
                int tempheur = head->children[i]->heuristic_value;
                if(tempheur > max){
                    max = tempheur;
                    maxindex = i;
                }
            }
            clock_t  finish = clock();
            
            
            
            for(int i=0;i<7;i++)
            {
            	if(head->children[maxindex]->heuristic_value<neweval(head->children[i]))
            	{
            		
            		maxindex=i;
            		*(head->children[maxindex]->ob)=*(head->children[i]->ob);
            		head->children[maxindex]->heuristic_value=neweval(head->children[i]);
            		
				}
			}
            
            cout<<"----------------------------->>>"<<endl;
            double elapsed_secs = double(finish - begin) / CLOCKS_PER_SEC;
            cout<<"�ɸ� �ð�: "<<elapsed_secs<<"��"<<endl;
            cout<<"AI�� ������ ����: ";
            cout<<head->children[maxindex]->ob->moves[maxindex].col_index<<endl;
            cout<<"�̹� ȸ������ �� �� �ִ� ���� ���� ���Լ� ���� ���� ������ ����"<<endl;
            for(int k = 0; k < 7; k++){
                cout<<"�� "<<k<<"�� ���Լ� ��: "<<head->children[k]->heuristic_value<<endl;
            }
            cout<<maxindex<<"�� ���Լ� ���� "<<head->children[maxindex]->heuristic_value<<"�� ���� ���� ������ �����ߴ�."<<endl;
            head->ob->display_board(head->children[maxindex]->ob);
            
            //�̰���� Ȯ��
            if(head->children[maxindex]->ob->checkPlayerWon()){
                cout<<"AI won!"<<endl;
                return 0;
            }
            
            
            cout<<"----------------------------->>>"<<endl;
            
            //���� ������Ʈ
            *(board_ob) = *(head->children[maxindex]->ob);
        }

        if(head->ob->player == 'X' && choiceO == 0 && moves_made >= 1){//user ����, 'O'
            int c;
            int row, col;
            
             //�� �� ���� �ڸ��� �������� ��츦 �ɷ����� ���� row == -1�϶� ���� �ٽ� �����ϵ��� �Ѵ�.
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
            
            //������ �ڸ��� X�� �ΰ�, �÷��̾ �ٲ��ش�.
            head->ob->board[row][col] = 'O';
            head->ob->player = 'O';
            
            //���� ���
            cout<<"Display board: "<<(moves_made+1)<<" moves made"<<endl;
            head->ob->display_board(head->ob);
            
            //�̰���� Ȯ��
            if(head->ob->checkPlayerWon()){
                cout<<"You won!"<<endl;
                return 0;
            }
            //���� ������Ʈ
            *(board_ob) = *(head->ob);
            
        }

        if(head->ob->player == 'X' && choiceO == 1 && moves_made >= 1){//AI ����, 'O'
            int max = INT_MIN;
            int maxindex = -1;
            cout<<"AI turn"<<endl;
            clock_t begin = clock();
            
            //���ĺ�Ÿ �Լ��� �����ؼ� child node���� heuristic value�� ��´�.
            int x = ab_ob.alpha_beta(head,0,head->ob->player,INT_MIN,INT_MAX, 'O');
            
            //�տ��� ���ĺ�Ÿ �Լ��� �����ؼ� ���� child node���� heuristic value ��, ���� ���� ���� ���� child�� �����Ѵ�.
            for(int i = 0; i < 7; i++){
                int tempheur = head->children[i]->heuristic_value;
                if(tempheur > max){
                    max = tempheur;
                    maxindex = i;
                }
            }
            clock_t  finish = clock();
/*
            for(int i=0;i<7;i++)
            {
            	if(head->children[maxindex]->heuristic_value<neweval(head->children[i]))
            	{
            		cout<<"************************"<<endl;;
            		maxindex=i;
            		*(head->children[maxindex]->ob)=*(head->children[i]->ob);
            		head->children[maxindex]->heuristic_value=neweval(head->children[i]);
				}
			}
*/
            
            cout<<"----------------------------->>>"<<endl;
            double elapsed_secs = double(finish - begin) / CLOCKS_PER_SEC;
            cout<<"�ɸ� �ð�: "<<elapsed_secs<<"��"<<endl;
            cout<<"AI�� ������ ����: ";
            cout<<head->children[maxindex]->ob->moves[maxindex].col_index<<endl;
            cout<<"�̹� ȸ������ �� �� �ִ� ���� ���� ���Լ� ���� ���� ������ ����"<<endl;
            for(int k = 0; k < 7; k++){
                cout<<"�� "<<k<<"�� ���Լ� ��: "<<head->children[k]->heuristic_value<<endl;
            }
            cout<<maxindex<<"�� ���Լ� ���� "<<head->children[maxindex]->heuristic_value<<"�� ���� ���� ������ �����ߴ�."<<endl;
            head->ob->display_board(head->children[maxindex]->ob);
            
            //�̰���� Ȯ��
            if(head->children[maxindex]->ob->checkPlayerWon()){
                cout<<"AI won!"<<endl;
                return 0;
            }
            
            
            cout<<"----------------------------->>>"<<endl;
            
            //���� ������Ʈ
            *(board_ob) = *(head->children[maxindex]->ob);
        }
       
   
        moves_made++;
        cout<<"moves made: "<<moves_made<<endl;
        game_path_length++;
    }
    
    return 0;
}
