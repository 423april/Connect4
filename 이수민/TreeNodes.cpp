#include <cstdlib>
#include <ctime>

#include "TreeNodes.h"
using namespace std;

/*트리 클래스의 모든 멤버 변수들을 초기화하는 트리 클래스를 위한 생성자-독고준석-
 */
tree::tree(){
    
    heuristic_value = -2000;
    num_children = 0;
    for(int i=0;i<7;i++){
        children[i] = NULL;
    }
    //tic 객체를 위한 포인터를 생성-독고준석-
    ob = new tic();
    
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
            p = 'X';
            this->children[i]->ob->player = p;
        }
        
        else if(ob->player == 'O'){
            p = 'O';
            this->children[i]->ob->player = p;
        }
        
        //if(ob->player == 'n')
        else{
            p = 'X';
            this->children[i]->ob->player = p;
        }
        /* 생성된 노드들의 총 수를 세는 것을 유지한다.-독고준석-
         */
        num_nodes_generated++;
        children[i]->ob->checkPossibleMoves();
        row = children[i]->ob->moves[i].row_index;
        col = children[i]->ob->moves[i].col_index;
        children[i]->ob->board[row][col] = p;
    }
}

/* 이 함수는 depth를 자르기 위해 사용된다.
 1st 휴리스틱 값이 설정됐는지를 확인
 2nd 특정 깊이에 도달됐는지 혹은 player가 게임을 이겼는지 확인
 3rd 위의 두 경우 모두 위의 add_all_children 함수를 호출하지 못하고 add_all_children 함수에서 컨트롤이 반환되면 false를 반환-독고준석-
 */
bool tree::deep_enough(int depth){
    
    if(heuristic_value!=-2000)
        return heuristic_value;
    
    else if(depth == 2 || ob->checkPlayerWon()){
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

/* this is my (Madhusudhan) Evaluation function
 which is briefly described in my project report
 it creates a 2-d array same as connect-four board
 dimensions, assigns values to each cells of the
 board, depending on whether the player is Max/Min
 assigns values to them and returns the sum of
 initial_value and the value
 */

int tree::evaluation_m(){
    
    int evaluationTable[6][7] ={
        {3, 4, 5, 7, 5, 4, 3},
        {4, 6, 8, 10, 8, 6, 4},
        {5, 8, 11, 13, 11, 8, 5},
        {5, 8, 11, 13, 11, 8, 5},
        {4, 6, 8, 10, 8, 6, 4},
        {3, 4, 5, 7, 5, 4, 3}
    };
    
    //since the sum of values of half board is 138.
    int initial_value = 138;
    int value = 0;
    
    if(ob->player == 'X'){
        //for lhs diagonal win
        for(int r=0;r<3;r++){
            for(int c=6;c>2;c--){
                char c1 = ob->board[r][c];
                char c2 = ob->board[r+1][c-1];
                char c3 = ob->board[r+2][c-2];
                char c4 = ob->board[r+3][c-3];
                if((c1 == 'X' && c2 == 'X') || (c2 == 'X' && c3 == 'X') || (c3 == 'X' && c4 == ' ')){
                    value = 100;
                }
            }
        }
        
        //for rhs diagonal win
        for(int r=0;r<3;r++){
            for(int c=0;c<4;c++){
                char c1 = ob->board[r][c];
                char c2 = ob->board[r+1][c+1];
                char c3 = ob->board[r+2][c+2];
                char c4 = ob->board[r+3][c+3];
                if((c1 == 'X' && c2 == 'X') || (c2 == 'X' && c3 == 'X') || (c3 == 'X' && c4 == ' ')){
                    value = 100;
                }
            }
        }
        
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 7; j++){
                if (this->ob->board[i][j] == 'X')
                    value += evaluationTable[i][j];
                else if (this->ob->board[i][j] == 'O')
                    value -= evaluationTable[i][j];
            }
        }
    }
    
    if(ob->player == 'O'){
        
        for(int r=0;r<3;r++){
            for(int c=6;c>2;c--){
                char c1 = ob->board[r][c];
                char c2 = ob->board[r+1][c-1];
                char c3 = ob->board[r+2][c-2];
                char c4 = ob->board[r+3][c-3];
                if((c1 == 'O' && c2 == 'O') || (c2 == 'O' && c3 == 'O') || (c3 == 'O' && c4 == ' ')){
                    value = -100;
                }
            }
        }
        
        //for rhs diagonal win
        for(int r=0;r<3;r++){
            for(int c=0;c<4;c++){
                char c1 = ob->board[r][c];
                char c2 = ob->board[r+1][c+1];
                char c3 = ob->board[r+2][c+2];
                char c4 = ob->board[r+3][c+3];
                if((c1 == 'O' && c2 == 'O') || (c2 == 'O' && c3 == 'O') || (c3 == 'O' && c4 == ' ')){
                    value = -100;
                }
            }
        }
        
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 7; j++){
                if(this->ob->board[i][j] == 'X')
                    value += evaluationTable[i][j];
                else if (this->ob->board[i][j] == 'O')
                    value -= evaluationTable[i][j];
            }
        }
    }
    return initial_value + value;
}

//santosh's eval fn
int tree::evaluation_s(){
    int value = 0;
    //2 check
    if(ob->player == 'X'){
        for(int i=0;i<6;i++){
            for(int j=0;j<4;j++){
                //for continuous row
                //first checking whether not everything is empty
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    if((ob->board[i][j] == ob->board[i][j+1]) && (ob->board[i][j+2]==' ')){
                        value = 100;
                    }
                }
            }
        }
        
        for(int i=0;i<3;i++){
            for(int j=0;j<7;j++){
                //for continuous col
                //first checking whether not everything is empty
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    if((ob->board[i][j] == ob->board[i+1][j]) && (ob->board[i+2][j]==' ')){
                        value = 100;
                    }
                }
            }
        }
        
    }
    
    else if(ob->player == 'O'){
        for(int i=0;i<6;i++){
            for(int j=0;j<4;j++){
                //for continuous row
                //first checking whether not everything is empty
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    if((ob->board[i][j] == ob->board[i][j+1]) && (ob->board[i][j+2]==' ')){
                        value = -100;
                    }
                }
                
                
            }
        }
        for(int i=0;i<3;i++){
            for(int j=0;j<7;j++){
                //for continuous col
                //first checking whether not everything is empty
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    if((ob->board[i][j] == ob->board[i+1][j]) && (ob->board[i+2][j]==' ')){
                        value = -100;
                    }
                }
            }
        }
        
    }
    
    return value;
}

//harsha's eval fn
int tree::evaluation_h(){
    int value = 0;
    //2 check
    if(ob->player == 'X'){
        for(int i=0;i<6;i++){
            for(int j=0;j<4;j++){
                //for continuous row
                //first checking whether not everything is empty
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    if((ob->board[i][j] == ob->board[i][j+1]) && (ob->board[i][j+2]==' ')){
                        value = 100;
                    }
                }
            }
        }
        
        for(int i=0;i<3;i++){
            for(int j=0;j<7;j++){
                //for continuous col
                //first checking whether not everything is empty
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    if((ob->board[i][j] == ob->board[i+1][j]) && (ob->board[i+2][j]==' ')){
                        value = 100;
                    }
                }
            }
        }
        
        
        //3 check
        //for continuous row
        for(int i=0;i<6;i++){
            for(int j=0;j<4;j++){
                
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    
                    if((ob->board[i][j] == ob->board[i][j+1] == ob->board[i][j+2] && ob->board[i][j] == 'X') && (ob->board[i][j+3]==' ') || (ob->board[i][j+1] == ob->board[i][j+2] == ob->board[i][j+3] && ob->board[i][j] == 'X') && (ob->board[i][j+4]!= ' ')){
                        value = 100;
                    }
                }
            }
        }
        
        
        //3 check
        //for continuous col
        
        for(int i=0;i<3;i++){
            
            for(int j=0;j<7;j++){
                
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    
                    if((ob->board[i][j] == ob->board[i+1][j] == ob->board[i+2][j] && (ob->board[i][j] == 'X') && (ob->board[i+3][j] == ' ') || ob->board[i+1][j] == ob->board[i+2][j] == ob->board[i+3][j]) && (ob->board[i][j] == 'X') && (ob->board[i+4][j] == ' ')){
                        value = 100;
                    }
                }
            }
        }
    }
    
    else if(ob->player == 'O'){
        for(int i=0;i<6;i++){
            for(int j=0;j<4;j++){
                //for continuous row
                //first checking whether not everything is empty
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    if((ob->board[i][j] == ob->board[i][j+1]) && (ob->board[i][j+2]==' ')){
                        value = -100;
                    }
                }
                
                
            }
        }
        for(int i=0;i<3;i++){
            for(int j=0;j<7;j++){
                //for continuous col
                //first checking whether not everything is empty
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    if((ob->board[i][j] == ob->board[i+1][j]) && (ob->board[i+2][j]==' ')){
                        value = -100;
                    }
                }
            }
        }
        
        //3 check
        //for continuous row
        for(int i=0;i<6;i++){
            for(int j=0;j<4;j++){
                
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    
                    if((ob->board[i][j] == ob->board[i][j+1] == ob->board[i][j+2] && ob->board[i][j] == 'O') && (ob->board[i][j+3]==' ') || (ob->board[i][j+1] == ob->board[i][j+2] == ob->board[i][j+3] && ob->board[i][j] == 'O') && (ob->board[i][j+4]!= ' ')){
                        value = -100;
                    }
                }
            }
        }
        
        
        //3 check
        //for continuous col
        
        for(int i=0;i<3;i++){
            for(int j=0;j<7;j++){
                
                if(ob->board[i][j]!= ' ' || ob->board[i][j+1]!= ' ' || ob->board[i][j+2]!= ' ' || ob->board[i][j+3]!= ' '){
                    
                    if((ob->board[i][j] == ob->board[i+1][j] == ob->board[i+2][j]) && (ob->board[i][j] == 'O') && (ob->board[i+3][j] == ' ') || ob->board[i+1][j] == ob->board[i+2][j] == ob->board[i+3][j] && (ob->board[i][j] == 'O') && (ob->board[i+4][j] == ' ')){
                        value = -100;
                    }
                }
            }
        }
    }
    
    return value;
}

/* 이 함수는 하나의 노드들 보드 구성을 다른 노드들 보드로 복사한다. 
참조 매개 변수로 복사될 필요가 있는 보드의 구성들이 함수로 넘겨지고 rhs 보드 객체의 내용들을 lhs 보드 객체로 복사한다.-독고준석-
 */
void tree::copy_board_status(tic &tc){
    
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
        //child 숫자가 0 – 6일 수 있으므로 counter=-1-독고준석-
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
    // +1은 빈 보드에서 확장되므로 루트 노드를 위한 것이다.-독고준석-
    cout<<"Number of nodes expanded: "<< num_nodes_expanded + 1 <<endl;
    cout<<"Total length of game path: "<<game_path_length<<endl;
    cout<<"Memory size used by 1 node is: 268 bytes"<<endl;
    cout<<"Total size of memory used by program: "<<(268*num_nodes_generated)<<" bytes"<<endl;
    cout<<"Total execution Time: "<<(total_time) / double(CLOCKS_PER_SEC)<<"'s"<<endl;
    
}


