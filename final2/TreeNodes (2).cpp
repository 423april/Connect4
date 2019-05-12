#include <cstdlib>
#include <ctime>
#include <climits>

#include "TreeNodes (2).h"

using namespace std;

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

/* 이 함수는 7 노드들을 create_node 함수를 이용하여 동적으로 생성한다.(i.e for child's)
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

/* 이 함수는 depth를 자르기 위해 사용된다.
 1st 휴리스틱 값이 설정됐는지를 확인
 2nd 특정 깊이에 도달됐는지 혹은 player가 게임을 이겼는지 확인
 3rd 위의 두 경우 모두 위의 add_all_children 함수를 호출하지 못하고 add_all_children 함수에서 컨트롤이 반환되면 false를 반환-독고준석-
 */
bool tree::deep_enough(int depth){
    
    if(heuristic_value!= INT_MIN) 
        return heuristic_value;
    
    if(depth == 10|| ob->checkPlayerWon()){
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




/* 이 함수는 하나의 노드들 보드 구성을 다른 노드들 보드로 복사한다.
 참조 매개 변수로 복사될 필요가 있는 보드의 구성들이 함수로 넘겨지고 rhs 보드 객체의 내용들을 lhs 보드 객체로 복사한다.-독고준석-
 */void tree::copy_board_status(tic &tc){
    
    for(int i=0;i<ob->row_size;i++){
        for(int j=0;j<ob->col_size;j++){
            this->ob->board[i][j] = tc.board[i][j];
        }
    }
    
    for(int i=0;i<7;i++){
        this->ob->moves[i].col_index = tc.moves[i].col_index;
        this->ob->moves[i].row_index = tc.moves[i].row_index;
    }
}

/* 이 함수는 부모 노드와 같은 휴리스틱 값을 갖는 특정한 child 노드를 리턴한다. i.e 그것은 미래에 이길 가능성이 가장 높은 최적 child 노드의 index(i.e 'i' in our case)를 리턴한다.-독고준석-
 */
int tree::getOptimalNode(){
    
    for(int i=0;i<7;i++){
        if(children[i]->heuristic_value == this->heuristic_value && this->heuristic_value != -2000)
            return i;
    }
    return -1;
}

/* 이 함수는 터미널 노드에 도달했는지 확인하고 display_contents 함수를 호출한다.-독고준석-
 */
void tree::helper(){
    
    char c1,c2,c3,c4;
    
    for(int r=0;r<3;r++){
        for(int c=6;c>2;c--){
            c1 = ob->board[r][c];
            c2 = ob->board[r+1][c-1];
            c3 = ob->board[r+2][c-2];
            c4 = ob->board[r+3][c-3];
            //board[r][c]==board[r+1][c-1]==board[r+2][c-2]==board[r+3][c-3]
            if((c1 == c2 && c2 == c3 && c4 == ' ' && c1 == this->ob->player && this->ob->board[r+2][c-3]!= ' ')){
                //cout<<"RHS matched"<<endl;
                this->ob->board[r+3][c-3] = this->ob->player;
                display_contents();
                exit(0);
            }
        }
    }
    
    //looking form left end of array lhs-diagonal
    for(int r=0;r<3;r++){
        for(int c=0;c<4;c++){
            c1 = ob->board[r][c];
            c2 = ob->board[r+1][c+1];
            c3 = ob->board[r+2][c+2];
            c4 = ob->board[r+3][c+3];
            //board[r][c]==board[r+1][c+1]==board[r+2][c+2]==board[r+3][c+3]
            if((c1 == c2 && c2 == c3 && c4 == ' ' && c1 == this->ob->player && this->ob->board[r+2][c-3]!= ' ')){
                //cout<<"LHS matched"<<endl;
                this->ob->board[r+3][c+3] = this->ob->player;
                display_contents();
                exit(0);
            }
        }
    }
}

/*이 함수는 getOptimalNode 함수를 호출하고 child의 index를 저장하고 이 child 노드들의 보드 구성을 다른 만들어진 보드로 복사한다. i.e 승리 상태에 도달하면 helper 함수가 호출된다.-독고준석-
 */
void tree::move_gen(tic *board_ob,int moves_made){
    int child_number = this->getOptimalNode();
    
    if(child_number != -1){
        //since child number can be 0 - 6 so counter=-1
        *(board_ob) = *(this->children[child_number]->ob);
        if(moves_made == 2 || moves_made == 6 || moves_made == 10 || moves_made == 15){
            cout<<"Board state after "<<moves_made<<" moves:"<<endl;
            board_ob->display_board(board_ob);
        }
    }
    
    else if(child_number == -1)
        this->helper();
}


/* 이 함수는 커넥트포 게임의 결과를 보여주는데 이용된다.-독고준석-
 */
void tree::display_contents(){
    
    clock_t stop_time = clock();
    clock_t total_time = (stop_time - start_time);
    double memory_used = 0.0;
    
    cout<<this->ob->player<<" Won!"<<endl;
    cout<<endl;
    cout<<"Terminal(win) state of the board:"<<endl;
    ob->display_board(ob);
    cout<<endl;
    cout<<"Number of nodes generated: "<<num_nodes_generated<<endl;
    // +1 is for the root node since it is also expanded from the empty board
    cout<<"Number of nodes expanded: "<< num_nodes_expanded + 1 <<endl;
    cout<<"Total length of game path: "<<game_path_length<<endl;
    cout<<"Memory size used by 1 node is: 268 bytes"<<endl;
    cout<<"Total size of memory used by program: "<<(268*num_nodes_generated)<<" bytes"<<endl;
    cout<<"Total execution Time: "<<(total_time) / double(CLOCKS_PER_SEC)<<"'s"<<endl;
    
}


