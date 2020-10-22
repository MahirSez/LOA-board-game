#include<bits/stdc++.h>
#define BLACK 1
#define WHITE 2
using namespace std;

const int N = 8;

int board[N][N];


void print_board() {
    for(int i =0 ; i < N ; i++ ) {
        for(int j =0 ; j < N ; j++ ) cout<<board[i][j]<<" ";
        cout<<'\n';
    }
}

void init(){
    for(int i =1 ; i < N-1 ; i++ ) {
        board[0][i] = BLACK;
        board[N-1][i] = BLACK;

        board[i][0] = WHITE;
        board[i][N-1] = WHITE;
    }

    // print_board();
}


void gen_random_move() {
    

    for(int i =0  ;i < N ; i++ ) {
        for(int j =0 ; j < N ; j++ ) {
            if(board[i][j] == WHITE) {
                
                cout<<i<<" "<<j<<endl;
                board[i][j] = 0;
                return;
            }
        }
    }
    assert(0);

}

int main() {

    init();
    string s;
    getline(cin,s);
    cerr<<s<<endl;
    getline(cin,s);
    cerr<<s<<endl;

    while(1) {

        int x1  ,y1, x2, y2;

        cin>>x1>>y1>>x2>>y2;
        
        board[x1][y1] = 0;
        board[x2][y2] = BLACK;



        gen_random_move();


        int n;
        cin>>n;
        while(n--) cin>>x2>>y2;
        cout<<x2<<" "<<y2<<endl;
        board[x2][y2] = WHITE;
        
    }
    return 0;
}