#include <iostream>
#include <ctime>
#include <cstdlib>

#include "ConnectFourBoard.h"
using namespace std;

/* 이 클래스의 모든 멤버 변수들을 초기화하는 생성자가 새로운 연산자를 이용하여 커넥트포 보드를 위한 2차원 배열을 동적으로 생성한다.
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

/*boolean 함수는 보드가 비어있지 않다면 false를 리턴하고 그렇지 않다면 true를 리턴함-독고준석-
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

/* 이 함수는 랜덤으로 한 열을 고름으로써 첫 번째 move를 만드는데 이용된다.-독고준석-
 */
int tic::makeFirstMove(){
    
    int random_col;
    srand((unsigned int)time(NULL));
    do{
        random_col = rand()%7;
    }while(random_col == 3); //column index 3 일때 다시 선정.
    return random_col;
}

/*이것은 player에 의해 만들어지는 모든 가능한 move들을 확인하기 위해 이용된다. 첫 번째 경우는 0번째 행에 빈 셀들이 있는지를 확인한다.
만약 존재한다면 그 열 위치를 move의 구조 멤버 변수의 col_index에 저장한다.
두 번째 경우는 모든 다른 빈 셀들을 확인한다. 비었다면, 그 아래의 셀이 비었는지를 확인한다. 비었다면, 이 위치를 저장하지 않고 
그렇지 않다면 이 위치를 구조 move의 col_index와 row_index에 저장한다.-독고준석-
 */
void tic::checkPossibleMoves(){
    
    int c=0;
    
    //0번째 행에 빈 셀들이 있는지 확인한다.-독고준석-
    for(int col=0;col<col_size;col++){
        if(board[0][col]==' '){
            this->moves[c].row_index = 0;
            this->moves[c].col_index = col;
            c++;
        }
    }
    
    //빈 공간에 대한 다른 모든 행을 확인-독고준석-
    for(int row=1;row<row_size;row++){
        for(int col=0;col<col_size;col++){
            if(board[row][col]==' '){
                if(board[row-1][col]!=' '){
                    (moves+c)->col_index=col;
                    (moves+c)->row_index=row;
                    c++;
                }
            }
        }
    }
}

/* 이것은 사용자가 이기면 true, 그렇지 않으면 false를 리턴하는 boolean 함수이다. 4가지 경우를 고려한다. 
1st 한 행에 연속적인 4
 2nd 수직으로 연속적인 4
 3rd 오른쪽 대각선으로 승리
 4th 왼쪽 대각선으로 승리
-독고준석-
 */
bool tic::checkPlayerWon(){
    
    bool win=false;
    char c1,c2,c3,c4;
    
    //행으로 이긴 것을 확인-독고준석-
    for(int r=0;r<row_size;r++){
        for(int c=0;c<4;c++){
            if(!win){
                c1 = board[r][c];
                c2 = board[r][c+1];
                c3 = board[r][c+2];
                c4 = board[r][c+3];
                //board[r][c]==board[r][c+1]==board[r][c+2]==board[r][c+3]를 확인-독고준석-
                if(c1!=' '||c2!=' '||c3!=' '||c4!=' '){ //not blank
                    if((c1 == c2)&& (c2 == c3) && (c3 == c4)){//모두 일치
                        win = true;
                        break;
                    }
                }
            }
        }
    }
    
    //열로 이긴 것을 확인-독고준석-
    if(!win){
        for(int r=0;r<3;r++){
            for(int c=0;c<col_size;c++){
                c1 = board[r][c];
                c2 = board[r+1][c];
                c3 = board[r+2][c];
                c4 = board[r+3][c];
                //board[r][c]==board[r+1][c]==board[r+2][c]==board[r+3][c]를 확인-독고준석-
                if(c1!=' '||c2!=' '||c3!=' '||c4!=' '){//비지 않음-독고준석-
                    if((c1 == c2)&& (c2 == c3) && (c3 == c4)){//모두 일치
                        win = true;
                        break;
                    }
                }
            }
        }
    }
    
    /* ---O
       --O-
       -O--
       O--- 이런 모양 대각선 찾기-이수민-
     */
    if(!win){
        for(int r=0;r<3;r++){
            for(int c=6;c>2;c--){
                c1 = board[r][c];
                c2 = board[r+1][c-1];
                c3 = board[r+2][c-2];
                c4 = board[r+3][c-3];
                //board[r][c]==board[r+1][c-1]==board[r+2][c-2]==board[r+3][c-3]를 확인-독고준석-
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
    /*
     O---
     -O--
     --O-
     ---O 이런 모양 대각선 찾기-이수민-
     */
    if(!win){
        for(int r=0;r<3;r++){
            for(int c=0;c<4;c++){
                c1 = board[r][c];
                c2 = board[r+1][c+1];
                c3 = board[r+2][c+2];
                c4 = board[r+3][c+3];
                //board[r][c]==board[r+1][c+1]==board[r+2][c+2]==board[r+3][c+3]를 확인-독고준석-
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

/* overloading = operator to copy one
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

/* 이 함수는 보드 내용을 보여주는데 사용됨-독고준석-
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
