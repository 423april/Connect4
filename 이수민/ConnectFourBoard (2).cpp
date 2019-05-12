#include <iostream>
#include <ctime>
#include <cstdlib>

#include "ConnectFourBoard (2).h"
using namespace std;

/* original code from https://github.com/0bit093/Connect_Four-Game
 이 클래스의 모든 멤버 변수들을 초기화하는 생성자가 새로운 연산자를 이용하여 커넥트포 보드를 위한 2차원 배열을 동적으로 생성한다.
 그리고 모든 셀들을 빈 character로 초기화하고 타입 Move의 구조 변수를 동적으로 만든다.
 row_index와 col_index를 -1로 할당한다. player char 변수는 n으로 할당한다.-독고준석-
 */
tic::tic(){
    //7열 6행-독고준석-
    row_size = 6;
    col_size = 7;
    //보드 생성-독고준석
    board = new char* [row_size];
    for(int i=0;i<row_size;i++)
        board[i] = new char [col_size];
    //보드 비우기-독고준석-
    for(int i=0;i<row_size;i++){
        for(int j=0;j<col_size;j++)
            board[i][j]=' ';
    }
    
    moves = new Move[7];
    for(int i=0;i<7;i++){
        moves[i].col_index = -1;
        moves[i].row_index = -1;
    }
    
    player = 'n';
}

/* original code from https://github.com/0bit093/Connect_Four-Game
 boolean 함수는 보드가 비어있지 않다면 false를 리턴하고 그렇지 않다면 true를 리턴함-독고준석-
 */
bool tic::isBoardEmpty(){
    
    for(int i=0;i<row_size;i++){
        for(int j=0;j<col_size;j++){
            if(board[i][j]!=' ')
                return false;
        }
    }
    return true;
}

/* 기여: 이수민
 첫번째 열을 고를때 사용하는 함수이다. 랜덤으로 열을 고르되, 가운데열(3열)을 고르면 다시 선정한다.
 */
int tic::makeFirstMove(){
    
    int random_col;
    srand((unsigned int)time(NULL));
    do{
        random_col = rand()%7;
    }while(random_col == 3); //column index 3 일때 다시 선정.
    return random_col;
}

/* 기여: 이수민
 현재 보드에서 사용 가능한 모든 수를 찾아내어, moves에 저장한다. 만약 열이 가득 차 있어서 사용할 수 없다면, row_index값을 -1로 넣어준다.
 */
void tic::checkPossibleMoves(){
    
        for(int col = 0; col < 7; col++){
            for(int row = 0; row < 6; row++){
                //밑에서부터 확인해서 빈칸을 만나면 그 좌표를 넣어주고, 해당 col의 row들을 확인하는 두번째 for문을 나간다.
                if(board[row][col] == ' '){
                    this->moves[col].col_index = col;
                    this->moves[col].row_index = row;
                    break;
                }
                else if(row == 5 && board[row][col] != ' '){
                    this->moves[col].col_index = col;
                    this->moves[col].row_index = -1;
            }
        }
    }

}

/* original code from https://github.com/0bit093/Connect_Four-Game
 이것은 player에 의해 만들어지는 모든 가능한 move들을 확인하기 위해 이용된다. 첫 번째 경우는 0번째 행에 빈 셀들이 있는지를 확인한다.
 만약 존재한다면 그 열 위치를 move의 구조 멤버 변수의 col_index에 저장한다.
 두 번째 경우는 모든 다른 빈 셀들을 확인한다. 비었다면, 그 아래의 셀이 비었는지를 확인한다. 비었다면, 이 위치를 저장하지 않고
 그렇지 않다면 이 위치를 구조 move의 col_index와 row_index에 저장한다.-독고준석-
 */
bool tic::checkPlayerWon(){
    
    bool win=false;
    char c1,c2,c3,c4;
    
    //for checking row wins
    for(int r=0;r<row_size;r++){
        for(int c=0;c<4;c++){
            if(!win){
                c1 = board[r][c];
                c2 = board[r][c+1];
                c3 = board[r][c+2];
                c4 = board[r][c+3];
                //checking for board[r][c]==board[r][c+1]==board[r][c+2]==board[r][c+3]
                if(c1!=' '||c2!=' '||c3!=' '||c4!=' '){ //not blank
                    if((c1 == c2)&& (c2 == c3) && (c3 == c4)){//모두 일치
                        win = true;
                        break;
                    }
                }
            }
        }
    }
    
    //for checking col wins
    if(!win){
        for(int r=0;r<3;r++){
            for(int c=0;c<col_size;c++){
                c1 = board[r][c];
                c2 = board[r+1][c];
                c3 = board[r+2][c];
                c4 = board[r+3][c];
                //checking for board[r][c]==board[r+1][c]==board[r+2][c]==board[r+3][c]
                if(c1!=' '||c2!=' '||c3!=' '||c4!=' '){//not blank
                    if((c1 == c2)&& (c2 == c3) && (c3 == c4)){//모두 일치
                        win = true;
                        break;
                    }
                }
            }
        }
    }
    
    //looking from right end of array rhs-diagonal
    /* ---O
       --O-
       -O--
       O--- 이런 모양 대각선 찾기
     */
    if(!win){
        for(int r=0;r<3;r++){
            for(int c=6;c>2;c--){
                c1 = board[r][c];
                c2 = board[r+1][c-1];
                c3 = board[r+2][c-2];
                c4 = board[r+3][c-3];
                //checking for board[r][c]==board[r+1][c-1]==board[r+2][c-2]==board[r+3][c-3]
                if(c1!=' '||c2!=' '||c3!=' '||c4!=' '){
                    if((c1 == c2)&& (c2== c3) && (c3== c4)){
                        //cout<<"win"<<endl;
                        win=true;
                        break;
                    }
                }
            }
        }
    }
    //looking form left end of array lhs-diagonal
    /*
     O---
     -O--
     --O-
     ---O 이런 모양 대각선 찾기
     */
    if(!win){
        for(int r=0;r<3;r++){
            for(int c=0;c<4;c++){
                c1 = board[r][c];
                c2 = board[r+1][c+1];
                c3 = board[r+2][c+2];
                c4 = board[r+3][c+3];
                //checking for board[r][c]==board[r+1][c+1]==board[r+2][c+2]==board[r+3][c+3]
                if(c1!=' '||c2!=' '||c3!=' '||c4!=' '){
                    if((c1 == c2)&& (c2== c3) && (c3== c4)){
                        //cout<<"win"<<endl;
                        win=true;
                        break;
                    }
                }
            }
        }
    }
    return win;
}

/* original code from https://github.com/0bit093/Connect_Four-Game
 overloading = operator to copy one
 board to another board object
 */
void tic::operator=(tic rhs){
    
    for(int i=0;i<row_size;i++){
        for(int j=0;j<col_size;j++){
            this->board[i][j] = rhs.board[i][j];
        }
    }
    
    for(int i=0;i<7;i++){
        this->moves[i].row_index = rhs.moves[i].row_index;
        this->moves[i].col_index = rhs.moves[i].col_index;
    }
    this->player = rhs.player;
}

/* original code from https://github.com/0bit093/Connect_Four-Game
 this function is used for displaying
 board contents
 */
void tic::display_board(tic *x){
    cout<<endl;
    cout<<"C0 "<<" C1 "<<" C2 "<<" C3 "<<" C4 "<<" C5 "<<" C6"<<endl;
    for(int i=5;i>=0;i--){
        for(int j=0;j<7;j++){
            cout<<x->board[i][j]<<" | ";
        }
        cout<<endl;
        for(int k=0;k<7;k++)
            cout<<"----";
        cout<<endl;
    }
    cout<<endl;
}
