#include <cstdlib>
#include <ctime>
#include <climits>

#include "TreeNodes (2).h"

using namespace std;

//original code from https://github.com/0bit093/Connect_Four-Game

/* a constructor for the tree class that
 initializes all member variables of tree
 class
 */
tree::tree(){
    
    heuristic_value = INT_MIN;
    num_children = 0;
    for(int i=0;i<7;i++){
        children[i] = NULL;
    }
    
    use = false;
    
    //creates pointer for new tic object
    ob = new tic();
    
    
}

/* 기여: 이수민
    char 'O'가 들어오면 'X'를 반환, char 'X'가 들어오면 'O'를 반환하는 함수이다.
 */
char tree::ReversePlayer(char player){
    if(player == 'X')
        return 'O';
    else
        return 'X';
}

/* 이 함수는 새로운 연산자를 이용하여 노드들을 동적으로 생성하고
 새로운 연산자에 의해 리턴된 노드의 주소를 각 노드의 멤버인 포인터 배열에 할당한다.(i.e 트리 객체)-독고준석-
 */
void tree::create_node(char p){
    
    children[num_children++] = new tree();
}


/* 변수 값을 이용하여 휴리스틱 값을 노드에 설정한다.(i.e 트리 객체)-독고준석-
 */
void tree::set_heuristic_value(int value){
    
    heuristic_value = value;
}

/*
 이 함수는 7 노드들을 create_node 함수를 이용하여 동적으로 생성한다.(i.e for child's)
 그리고 각 노드에 대해 플레이어를 할당하고
 ConnectFourBoard.cpp 파일에 정의된 check_possible_moves 함수를 호출하여
 플레이어가 수행 할 수 있는 모든 동작을 호출하고 할당한다.-독고준석-
 */
void tree::add_all_children(){
    
    char p;
    int row,col;
    
    for(int i=0;i<7;i++){
        
        create_node('n');
        
        /*같은 작업을 한다 as above create_node('n');
         num_children++;
         this->children[i] = new tree();-독고준석-
         */
        
        *(children[i]->ob) = *(this->ob);
        
        
        /*부모 노드 move를 확인한다. i.e 'X'/'O' 그리고 다른 character(Player)와 함께 child의move를 만든다.-독고준석-
         */
        if(this->ob->player == 'X'){
            p = 'O';
            this->children[i]->ob->player = p;
        }
        
        else if(ob->player == 'O'){
            p = 'X';
            this->children[i]->ob->player = p;
        }
        
        else{
            p = 'X';
            this->children[i]->ob->player = p;
        }

        
        num_nodes_generated++;
        children[i]->ob->checkPossibleMoves();
        
        //row index가 -1인 child의 use 값을 false로 설정해 해당 노드가 game tree에 추가되지 않도록 한다.
        if(children[i]->ob->moves[i].row_index != -1){
            row = children[i]->ob->moves[i].row_index;
            col = children[i]->ob->moves[i].col_index;
            children[i]->ob->board[row][col] = p;
            children[i]->use = true;
        }
        else{
            children[i]->use = false;
            continue;
        }
    }
}

/* 
 이 함수는 depth를 자르기 위해 사용된다.
 1st 휴리스틱 값이 설정됐는지를 확인
 2nd 특정 깊이에 도달됐는지 혹은 player가 게임을 이겼는지 확인
 3rd 위의 두 경우 모두 위의 add_all_children 함수를 호출하지 못하고 add_all_children 함수에서 컨트롤이 반환되면 false를 반환-독고준석-
 */
bool tree::deep_enough(int depth){
    
    if(heuristic_value!= INT_MIN) 
        return heuristic_value;
    
    if(depth == 8|| ob->checkPlayerWon()){
        if(ob->checkPlayerWon()){
            /*so that the last terminal node is
             also added to the tree i.e
             board containing win configuration */
            this->add_all_children();
        }
        return true;
    }
    
    else{
        num_nodes_expanded++;
        this->add_all_children();
        return false;
    }
}

/* 기여: 이수민
 heuristic idea from https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=2ahUKEwip6MnbmZXiAhWHyYsBHTPXDh4QFjAAegQIAhAC&url=http%3A%2F%2Fgit.mrman.de%2Fai-c4%2Fblob%2Fmaster%2Fs3505919.pdf&usg=AOvVaw0qFJY71PTq8cfSFhUF_i6O
 winning line을 세어주는 아이디어에서 기반한다. 자기돌이 winning line이 있을 때마다 돌 개수에 비례해서 미리 정해진 점수를 더해주고, 상대방의 winning line이 있을 때마다 돌 개수에 비례해서 미리 정해진 점수를 빼준다. 더 자세한 사항은 보고서에 추가.
 */
int tree::innerEval(char c1, char c2, char c3, char c4, char me, char opp){
    char blank = ' ';
    int goodvalue = 0;
    int value = 0;

    
    //3개가 me일때 value에 50 더해줌
    if((c1 == me && c2== me && c3== me && c4 == blank)||(c1 == me && c2== me && c3== blank && c4 == me)|(c1 == me && c2== blank && c3 == me && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == me)){
        value += 50 ;
    }
    //2개가 me일때 value에 15 더해줌
    if((c1 == me && c2== me && c3== blank && c4 == blank)||(c1 == me && c2== blank && c3== me && c4 == blank)|(c1 == me && c2== blank && c3 == blank && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == blank)|(c1 == blank && c2== me && c3== blank && c4 == me)|(c1 == blank && c2== blank && c3== me && c4 == me)){
        value += 15 ;
    }
    //1개가 me일때 value에 3 더해줌
    if((c1 == me && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== me && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == me && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == me)){
        value += 3 ;
    }
    //3개가 opp일때 value에서 80 빼줌
    if((c1 == opp && c2== opp && c3== opp && c4 == blank)||(c1 == opp && c2== opp && c3== blank && c4 == opp)|(c1 == opp && c2== blank && c3 == opp && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == opp)){
        value -= 50 ;
    }
    //2개가 opp일때 value에서 30 빼줌
    if((c1 == opp && c2== opp && c3== blank && c4 == blank)||(c1 == opp && c2== blank && c3== opp && c4 == blank)|(c1 == opp && c2== blank && c3 == blank && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == blank)|(c1 == blank && c2== opp && c3== blank && c4 == opp)|(c1 == blank && c2== blank && c3== opp && c4 == opp)){
        value -= 15 ;
    }
    //1개가 opp일때 value에서 10 빼줌
    if((c1 == opp && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== opp && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == opp && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == opp)){
        value -= 3 ;
    }
    //4개 다 me일때, 즉 이기는 경우 -> +1000000 부여
    if(c1 == me && c2== me && c3== me && c4== me){
        value += 1000000;
    }
    
    //4개 다 opp일때, 즉 이기는 경우 -> -1000000 부여
    if(c1 == opp && c2== opp && c3== opp && c4== opp){
        value -= 1000000;
    }
    
    return value;
}


/* 기여: 이수민
 왼쪽 대각선, 오른쪽 대각선, 가로, 세로 모양별로 계산한 board의 heuristic value의 일부를 innerEval() 함수를 통해 계산하고, eval 변수에 더해주어 최종적으로는 보드의 전체 heuristic value를 얻는다. */
int tree::evaluation(char aiplayer){
    int eval = 0;
    int temp = 0;
    int linecount = 0;
    char me = aiplayer;
    char opp = ReversePlayer(aiplayer);
    char blank = ' ';
    
    //for lhs diagonal win
    for(int r=0;r<3;r++){
        for(int c=6;c>2;c--){
            
            char c1 = this->ob->board[r][c];
            char c2 = this->ob->board[r+1][c-1];
            char c3 = this->ob->board[r+2][c-2];
            char c4 = this->ob->board[r+3][c-3];
            
            temp = innerEval(c1, c2, c3, c4, me, opp);
            eval += temp;
            
        }
    }
    
    
    
    //for rhs diagonal win
    for(int r=0;r<3;r++){
        for(int c=0;c<4;c++){
            char c1 = this->ob->board[r][c];
            char c2 = this->ob->board[r+1][c+1];
            char c3 = this->ob->board[r+2][c+2];
            char c4 = this->ob->board[r+3][c+3];
            
            temp = innerEval(c1, c2, c3, c4, me, opp);
            eval += temp;
            
        }
        
    }
    
    
    //가로 모양
    for(int r=0;r<6;r++){
        for(int c=0;c<4;c++){
            char c1 = this->ob->board[r][c];
            char c2 = this->ob->board[r][c+1];
            char c3 = this->ob->board[r][c+2];
            char c4 = this->ob->board[r][c+3];
            
            temp = innerEval(c1, c2, c3, c4, me, opp);
            eval += temp;
            }
    }
    
    
    
    //세로 모양
    for(int r=0;r<3;r++){
        for(int c=0;c<7;c++){
            char c1 = this->ob->board[r][c];
            char c2 = this->ob->board[r+1][c];
            char c3 = this->ob->board[r+2][c];
            char c4 = this->ob->board[r+3][c];
            
            temp = innerEval(c1, c2, c3, c4, me, opp);
            eval += temp;
            
        }
    }
    
    return eval;
}



