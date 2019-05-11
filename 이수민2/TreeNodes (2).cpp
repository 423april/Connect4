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
    
    heuristic_value = -2000;
    num_children = 0;
    for(int i=0;i<7;i++){
        children[i] = NULL;
    }
    //creates pointer for new tic object
    ob = new tic();
    
}

char tree::ReversePlayer(char player){
    if(player == 'X')
        return 'O';
    else
        return 'X';
}

/* this function creates nodes dynamically
 by using the new operator and assigns the
 address of the node returned by new
 operator to the pointer array that is
 member of each node(i.e tree object)
 */
void tree::create_node(char p){
    
    children[num_children++] = new tree();
}


/* sets the heuristic value using the
 variable value to nodes (i.e tree object)
 */
void tree::set_heuristic_value(int value){
    
    heuristic_value = value;
}

/* this function creates 7 nodes(i.e for child's)
 dynamically by using the create_node function
 above, assigns the player for each of those
 nodes and calls check_possible_moves function
 defined in the ConnectFourBoard.cpp file
 for all the possible moves that can be made by
 the player and assigns them
 */
void tree::add_all_children(){
    
    char p;
    int row,col;
    
    for(int i=0;i<7;i++){
        
        create_node('n');
        
        /*does the same task as above create_node('n');
         num_children++;
         this->children[i] = new tree();
         */
        
        *(children[i]->ob) = *(this->ob);
        
        
        /*checks parents node move i.e 'X'/'O' and makes
         child's move with the other character(Player)
         also sets the child's player to be that of
         the opposite of parent nodes player
         */
        
       
        /* keeps a count of the total number of
         nodes that is generated
         */
        num_nodes_generated++;
        children[i]->ob->checkPossibleMoves();
        row = children[i]->ob->moves[i].row_index;
        col = children[i]->ob->moves[i].col_index;
        children[i]->ob->board[row][col] = children[i]->ob->player;
    }
}

/* this function is used for a depth cut-off
 it has 3 cases where in 1st checks for
 whether heuristic value is set or not
 2nd checks whether a particular depth
 has been reached or not or if a player
 has won the game then return true
 3rd case being that both above cases
 failed that calls the above add_all_children
 function and return false once the control
 is returned back from the add_all_children
 function
 */
bool tree::deep_enough(int depth){
    
    if(heuristic_value!=-2000)
        return heuristic_value;
    
    else if(depth == 4 || ob->checkPlayerWon()){
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




int tree::evaluation_m(){
    
    int value = 0;
    char me = this->ob->player;
    char opp = ReversePlayer(this->ob->player);
    char blank = ' ';
    	
    //for lhs diagonal win
    for(int r=0;r<3;r++){
        for(int c=6;c>2;c--){
            
            char c1 = this->ob->board[r][c];
            char c2 = this->ob->board[r+1][c-1];
            char c3 = this->ob->board[r+2][c-2];
            char c4 = this->ob->board[r+3][c-3];

            //4개 다 me일때, 즉 이기는 경우 -> +무한대값 부여
            if(c1 == me && c2== me && c3== me && c4== me){
                value = INT_MAX;
            }
            //3개가 me일때 heuristic에 1400 더해줌
            if((c1 == me && c2== me && c3== me && c4 == blank)||(c1 == me && c2== me && c3== blank && c4 == me)|(c1 == me && c2== blank && c3 == me && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == me)){
                value += 1400 ;
            }
            //2개가 me일때 heuristic에 500 더해줌
            if((c1 == me && c2== me && c3== blank && c4 == blank)||(c1 == me && c2== blank && c3== me && c4 == blank)|(c1 == me && c2== blank && c3 == blank && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == blank)|(c1 == blank && c2== me && c3== blank && c4 == me)|(c1 == blank && c2== blank && c3== me && c4 == me)){
                value += 500 ;
            }
            //1개가 me일때 heuristic에 100 더해줌
            if((c1 == me && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== me && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == me && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == me)){
                value += 100 ;
            }
            //3개가 opp일때 heuristic에서 1400 빼줌
            if((c1 == opp && c2== opp && c3== opp && c4 == blank)||(c1 == opp && c2== opp && c3== blank && c4 == opp)|(c1 == opp && c2== blank && c3 == opp && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == opp)){
                value -= 1400 ;
            }
            //2개가 opp일때 heuristic에서 500 빼줌
            if((c1 == opp && c2== opp && c3== blank && c4 == blank)||(c1 == opp && c2== blank && c3== opp && c4 == blank)|(c1 == opp && c2== blank && c3 == blank && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == blank)|(c1 == blank && c2== opp && c3== blank && c4 == opp)|(c1 == blank && c2== blank && c3== opp && c4 == opp)){
                value -= 500 ;
            }
            //1개가 opp일때 heuristic에서 100 빼줌
            if((c1 == opp && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== opp && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == opp && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == opp)){
                value -= 100 ;
            }
            //4개 다 opp일때, 즉 이기는 경우 -> -무한대값 부여
            if(c1 == opp && c2== opp && c3== opp && c4== opp){
                value = INT_MIN;
            }
        }
    }
    
    
    
    //for rhs diagonal win
    for(int r=0;r<3;r++){
        for(int c=0;c<4;c++){
            char c1 = this->ob->board[r][c];
            char c2 = this->ob->board[r+1][c+1];
            char c3 = this->ob->board[r+2][c+2];
            char c4 = this->ob->board[r+3][c+3];

            //4개 다 me일때, 즉 이기는 경우 -> +무한대값 부여
            if(c1 == me && c2== me && c3== me && c4== me){
                value = INT_MAX;
            }
            //3개가 me일때 heuristic에 1400 더해줌
            if((c1 == me && c2== me && c3== me && c4 == blank)||(c1 == me && c2== me && c3== blank && c4 == me)|(c1 == me && c2== blank && c3 == me && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == me)){
                value += 1400 ;
            }
            //2개가 me일때 heuristic에 500 더해줌
            if((c1 == me && c2== me && c3== blank && c4 == blank)||(c1 == me && c2== blank && c3== me && c4 == blank)|(c1 == me && c2== blank && c3 == blank && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == blank)|(c1 == blank && c2== me && c3== blank && c4 == me)|(c1 == blank && c2== blank && c3== me && c4 == me)){
                value += 500 ;
            }
            //1개가 me일때 heuristic에 100 더해줌
            if((c1 == me && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== me && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == me && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == me)){
                value += 100 ;
            }
            //3개가 opp일때 heuristic에서 1400 빼줌
            if((c1 == opp && c2== opp && c3== opp && c4 == blank)||(c1 == opp && c2== opp && c3== blank && c4 == opp)|(c1 == opp && c2== blank && c3 == opp && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == opp)){
                value -= 1400 ;
            }
            //2개가 opp일때 heuristic에서 500 빼줌
            if((c1 == opp && c2== opp && c3== blank && c4 == blank)||(c1 == opp && c2== blank && c3== opp && c4 == blank)|(c1 == opp && c2== blank && c3 == blank && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == blank)|(c1 == blank && c2== opp && c3== blank && c4 == opp)|(c1 == blank && c2== blank && c3== opp && c4 == opp)){
                value -= 500 ;
            }
            //1개가 opp일때 heuristic에서 100 빼줌
            if((c1 == opp && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== opp && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == opp && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == opp)){
                value -= 100 ;
            }
            //4개 다 opp일때, 즉 이기는 경우 -> -무한대값 부여
            if(c1 == opp && c2== opp && c3== opp && c4== opp){
                value = INT_MIN;
            }

            }
        
            }
    
    
    //가로 모양
    for(int r=0;r<6;r++){
        for(int c=0;c<4;c++){
            char c1 = this->ob->board[r][c];
            char c2 = this->ob->board[r][c+1];
            char c3 = this->ob->board[r][c+2];
            char c4 = this->ob->board[r][c+3];

            //4개 다 me일때, 즉 이기는 경우 -> +무한대값 부여
            if(c1 == me && c2== me && c3== me && c4== me){
                value = INT_MAX;
            }
            //3개가 me일때 heuristic에 1400 더해줌
            if((c1 == me && c2== me && c3== me && c4 == blank)||(c1 == me && c2== me && c3== blank && c4 == me)|(c1 == me && c2== blank && c3 == me && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == me)){
                value += 1400 ;
            }
            //2개가 me일때 heuristic에 500 더해줌
            if((c1 == me && c2== me && c3== blank && c4 == blank)||(c1 == me && c2== blank && c3== me && c4 == blank)|(c1 == me && c2== blank && c3 == blank && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == blank)|(c1 == blank && c2== me && c3== blank && c4 == me)|(c1 == blank && c2== blank && c3== me && c4 == me)){
                value += 500 ;
            }
            //1개가 me일때 heuristic에 100 더해줌
            if((c1 == me && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== me && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == me && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == me)){
                value += 100 ;
            }
            //3개가 opp일때 heuristic에서 1400 빼줌
            if((c1 == opp && c2== opp && c3== opp && c4 == blank)||(c1 == opp && c2== opp && c3== blank && c4 == opp)|(c1 == opp && c2== blank && c3 == opp && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == opp)){
                value -= 1400 ;
            }
            //2개가 opp일때 heuristic에서 500 빼줌
            if((c1 == opp && c2== opp && c3== blank && c4 == blank)||(c1 == opp && c2== blank && c3== opp && c4 == blank)|(c1 == opp && c2== blank && c3 == blank && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == blank)|(c1 == blank && c2== opp && c3== blank && c4 == opp)|(c1 == blank && c2== blank && c3== opp && c4 == opp)){
                value -= 500 ;
            }
            //1개가 opp일때 heuristic에서 100 빼줌
            if((c1 == opp && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== opp && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == opp && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == opp)){
                value -= 100 ;
            }
            //4개 다 opp일때, 즉 이기는 경우 -> -무한대값 부여
            if(c1 == opp && c2== opp && c3== opp && c4== opp){
                value = INT_MIN;
            }            }
        }
    
    
    
    //세로 모양
    for(int r=0;r<3;r++){
        for(int c=0;c<7;c++){
            char c1 = this->ob->board[r][c];
            char c2 = this->ob->board[r+1][c];
            char c3 = this->ob->board[r+2][c];
            char c4 = this->ob->board[r+3][c];

            //4개 다 me일때, 즉 이기는 경우 -> +무한대값 부여
            if(c1 == me && c2== me && c3== me && c4== me){
                value = INT_MAX;
            }
            //3개가 me일때 heuristic에 1400 더해줌
            if((c1 == me && c2== me && c3== me && c4 == blank)||(c1 == me && c2== me && c3== blank && c4 == me)|(c1 == me && c2== blank && c3 == me && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == me)){
                value += 1400 ;
            }
            //2개가 me일때 heuristic에 500 더해줌
            if((c1 == me && c2== me && c3== blank && c4 == blank)||(c1 == me && c2== blank && c3== me && c4 == blank)|(c1 == me && c2== blank && c3 == blank && c4 == me)|(c1 == blank && c2== me && c3== me && c4 == blank)|(c1 == blank && c2== me && c3== blank && c4 == me)|(c1 == blank && c2== blank && c3== me && c4 == me)){
                value += 500 ;
            }
            //1개가 me일때 heuristic에 100 더해줌
            if((c1 == me && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== me && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == me && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == me)){
                value += 100 ;
            }
            //3개가 opp일때 heuristic에서 1400 빼줌
            if((c1 == opp && c2== opp && c3== opp && c4 == blank)||(c1 == opp && c2== opp && c3== blank && c4 == opp)|(c1 == opp && c2== blank && c3 == opp && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == opp)){
                value -= 1400 ;
            }
            //2개가 opp일때 heuristic에서 500 빼줌
            if((c1 == opp && c2== opp && c3== blank && c4 == blank)||(c1 == opp && c2== blank && c3== opp && c4 == blank)|(c1 == opp && c2== blank && c3 == blank && c4 == opp)|(c1 == blank && c2== opp && c3== opp && c4 == blank)|(c1 == blank && c2== opp && c3== blank && c4 == opp)|(c1 == blank && c2== blank && c3== opp && c4 == opp)){
                value -= 500 ;
            }
            //1개가 opp일때 heuristic에서 100 빼줌
            if((c1 == opp && c2== blank && c3== blank && c4 == blank)||(c1 == blank && c2== opp && c3== blank && c4 == blank)|(c1 == blank && c2== blank && c3 == opp && c4 == blank)|(c1 == blank && c2== blank && c3== blank && c4 == opp)){
                value -= 100 ;
            }
            //4개 다 opp일때, 즉 이기는 경우 -> -무한대값 부여
            if(c1 == opp && c2== opp && c3== opp && c4== opp){
                value = INT_MIN;
            }
        
            }
        }
        
        
    
    return value;
}




/*this function copies one nodes board configuration
 to the other nodes board it the board's configuration
 that needs to be copied as a reference parameter
 passed to the function and copies all the contents of the
 rhs board object to the lhs board ob
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

/* this function return a particular child node
 which has the same heuristic value as that of
 its parent node i.e it return the optimal child
 node's index (i.e 'i' in our case) that is
 most probable to win in the future
 */
int tree::getOptimalNode(){
    
    for(int i=0;i<7;i++){
        if(children[i]->heuristic_value == this->heuristic_value && this->heuristic_value != -2000)
            return i;
    }
    return -1;
}

/* this function checks whether the terminal node has been reached
 and calls display_contents function
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

/* this function calls the above getOptimalNode function
 stores the child's index and copies this child node's
 board configuration to other created board by using
 overloaded = operator to copy or else when a terminal node
 i.e a win state is reached it calls the helper function
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


/* this function is used for displaying the
 results of the connect-four game
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


