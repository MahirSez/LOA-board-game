#include<bits/stdc++.h>
#define BLACK 1
#define WHITE 2
using namespace std;

int N, my_color, enemy_color;

const int INF = 1e6;

bool first_move_done;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());



vector< vector<int> > board;


void print_board() {
    for(int i =0 ; i < N ; i++ ) {
        for(int j =0 ; j < N ; j++ ) cout<<board[i][j]<<" ";
        cout<<'\n';
    }
}


void init_board(string s1, string s2){

    
    
    
    N = stoi(s1);
    my_color = stoi(s2);
    enemy_color = (my_color == 1 ? 2 : 1 );

    board = vector< vector<int> > (N , vector<int>(N));

    for(int i =1 ; i < N-1 ; i++ ) {
        board[0][i] = BLACK;
        board[N-1][i] = BLACK;

        board[i][0] = WHITE;
        board[i][N-1] = WHITE;
    }

    // print_board();
}


bool my_turn() {
    ifstream in;
    in.open("shared.txt");
    int now_turn;
    in>>now_turn;
    if( now_turn != my_color ) return false;

    if(!first_move_done && my_color == BLACK) return true;
    first_move_done = true;

    int x1, y1, x2, y2;
    in>>x1>>y1>>x2>>y2;

    board[x1][y1] = 0;
    board[x2][y2] = enemy_color;

    in.close();

    return true;
}

void write_move(int x1, int y1, int x2, int y2) {
    ofstream out;
    out.open("shared.txt");
    out<<0<<'\n';
    out<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<'\n';

    board[x1][y1] = 0;
    board[x2][y2] = my_color;
    out.close();
}

void gen_random_move() {
    int x1 = rng()%N;
    int y1 = rng()%N;
    int x2 = rng()%N;
    int y2 = rng()%N;


    write_move(x1, y1, x2, y2);


}


int main(int argc, char* argv[]) {

    init_board(argv[1], argv[2]);

    while(1) {

        if(!my_turn()) continue;

        // if(x == 0) {    
        //     write_move(0, 1 , 0, N-1); 
        //     x =1;
        // }
        // else write_move(0, 2 , 2,0);


         gen_random_move();

    }
    return 0;
}