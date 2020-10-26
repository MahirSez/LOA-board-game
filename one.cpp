#include<bits/stdc++.h>
#define BLACK 1
#define WHITE 2
using namespace std;

int N, my_color, enemy_color;

const int INF = 1e6;

const int max_depth = 5;

bool first_move_done;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

vector< vector<int> > board;


void print_board(const vector< vector<int> >&board_config) {
    for(int i =0 ; i < N ; i++ ) {
        for(int j =0 ; j < N ; j++ ) cout<<board_config[i][j]<<" ";
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


vector< pair<int,int> >get_piece_positions(const vector< vector<int> >&board_config, int color) {
    vector< pair<int,int> >ret;
    for(int i =0 ; i < N ; i++ ) {
        for(int j =0 ; j < N ; j++ ) {
            if(board_config[i][j] == color) ret.push_back({i,j});
        }
    }
    return ret;
}


bool outside_board(int row, int col) {
    return ( row < 0 || row >= N || col < 0 || col >= N );
}

int traverse_tot_pieces(const vector< vector<int> >&board_config,int row,int col,int row_delta,int col_delta) {
	int cnt = 0;
	while(1) {
        row += row_delta;
        col += col_delta;
        if( outside_board(row, col) ) break;
        cnt+= (board_config[row][col] != 0);
    }
	return cnt;
}

int get_tot_piece_in_row(const vector< vector<int> >&board_config,int row,int col) {
	return 1 + traverse_tot_pieces(board_config, row, col, 0, -1) + traverse_tot_pieces(board_config, row, col, 0, 1);
}

int get_tot_piece_in_col(const vector< vector<int> >&board_config,int row,int col) {
    return 1 + traverse_tot_pieces(board_config, row, col, 1, 0) + traverse_tot_pieces(board_config, row, col, -1, 0);
}

int get_tot_piece_in_diagonal1(const vector< vector<int> >&board_config,int row,int col) {
	return 1 + traverse_tot_pieces(board_config, row, col, 1, 1) + traverse_tot_pieces(board_config, row, col, -1, -1);
}

int get_tot_piece_in_diagonal2(const vector< vector<int> >&board_config,int row,int col) {
    return 1 + traverse_tot_pieces(board_config, row, col, 1, -1) + traverse_tot_pieces(board_config, row, col, -1, 1);
}

bool blocking_cell(const vector< vector<int> >&board_config,int row,int col,int dist_left) {

    if( outside_board(row, col) ) return true;
    if( dist_left == 0 ) return board_config[row][col] == my_color;
    if(board_config[row][col] == enemy_color) return true;
    return false;
}

vector< pair<int,int> > traverse_possible_moves(const vector< vector<int> >&board_config,int row,int col,int row_delta,int col_delta,int dist) {
	vector< pair<int,int> > ret;
	for(int i = dist -1 ; i >=0 ; i--) {
	    row += row_delta;
	    col += col_delta;
	    if(blocking_cell(board_config, row,col,i)) return ret;
	    ret.push_back({row,col});
	}
	return ret;
}

vector< pair<int,int> > get_row_moves(const vector< vector<int> >&board_config,int row,int col,int dist) {
    vector< pair<int,int> > ret1  = traverse_possible_moves(board_config, row, col, 0, -1, dist);
    vector< pair<int,int> > ret2  = traverse_possible_moves(board_config, row, col, 0, 1, dist);
    ret1.insert(ret1.end() , ret2.begin(), ret2.end());
    return ret1;
}

vector< pair<int,int> > get_col_moves(const vector< vector<int> >&board_config,int row,int col,int dist) {
    vector< pair<int,int> > ret1  = traverse_possible_moves(board_config, row, col, -1, 0, dist);
    vector< pair<int,int> > ret2  = traverse_possible_moves(board_config, row, col, 1, 0, dist);
    ret1.insert(ret1.end() , ret2.begin(), ret2.end());
    return ret1;
}

vector< pair<int,int> > get_diagonal1_moves(const vector< vector<int> >&board_config,int row,int col,int dist) {
    vector< pair<int,int> > ret1  = traverse_possible_moves(board_config, row, col, 1, 1, dist);
    vector< pair<int,int> > ret2  = traverse_possible_moves(board_config, row, col, -1, -1, dist);
    ret1.insert(ret1.end() , ret2.begin(), ret2.end());
    return ret1;
}

vector< pair<int,int> > get_diagonal2_moves(const vector< vector<int> >&board_config,int row,int col,int dist) {
    vector< pair<int,int> > ret1  = traverse_possible_moves(board_config, row, col, -1, 1, dist);
    vector< pair<int,int> > ret2  = traverse_possible_moves(board_config, row, col, 1, -1, dist);
    ret1.insert(ret1.end() , ret2.begin(), ret2.end());
    return ret1;
}

vector< pair<int,int> > get_possible_moves(const vector< vector<int> >&board_config, int row, int col) {
    vector< pair<int,int> > ret;
    vector< pair<int,int> >tmp;

	int row_move_dist = get_tot_piece_in_row(board_config, row, col);
	tmp = get_row_moves(board_config, row, col, row_move_dist);
	ret.insert(ret.end() , tmp.begin() , tmp.end());

	int col_move_dist = get_tot_piece_in_col(board_config, row, col);
	tmp = get_col_moves(board_config, row, col, col_move_dist);
	ret.insert(ret.end() , tmp.begin() , tmp.end());

    int diagonal1_move_dist = get_tot_piece_in_diagonal1(board_config, row, col);
    tmp = get_diagonal1_moves(board_config, row, col, diagonal1_move_dist);
    ret.insert(ret.end() , tmp.begin() , tmp.end());

    int diagonal2_move_dist = get_tot_piece_in_diagonal2(board_config, row, col);
	tmp = get_diagonal2_moves(board_config, row, col, diagonal2_move_dist);
    ret.insert(ret.end() , tmp.begin() , tmp.end());

	return ret;
}


vector< tuple<int, int, int, int> >get_all_possible_moves(const vector< vector<int> >&board_config, const vector< pair<int,int> > &positions) {

    vector< tuple<int, int, int, int> > ret;

    for(auto x : positions) {
        vector< pair<int, int> > possible_moves = get_possible_moves(board_config, x.first, x.second);
        for(auto y: possible_moves) {
            ret.push_back(make_tuple(x.first, x.second, y.first, y.second));
        }
    }
    return ret;
}

int heuristic(const vector< vector<int> > &board_config, bool maximizing_player) {
    return  0;
}

bool is_terminating_condition(const vector< vector<int> >&board_config) {
    return false;
}


tuple<int,int,int,int,int> minimax(vector< vector<int> > board_config,int depth,int alpha, int beta, bool maximizing_player ) {
    if(depth == 0 || is_terminating_condition(board_config)) return make_tuple( heuristic(board_config, maximizing_player) , -1, -1 ,-1, -1);

    int target_color = maximizing_player? my_color : enemy_color;

    tuple<int,int,int,int,int> ret_val = make_tuple(-INF, -1, -1, -1, -1);
    if(!maximizing_player) ret_val = make_tuple(INF, -1, -1, -1, -1);

    vector< pair<int,int> >positions = get_piece_positions(board_config, target_color);
    vector< tuple<int, int, int, int> > possible_moves = get_all_possible_moves(board_config, positions);

    for(auto x : possible_moves) {

        int frmX, frmY, toX, toY;
        tie(frmX, frmY, toX, toY) = x;

        int store_color = board_config[toX][toY];
        board_config[frmX][frmY] = 0;
        board_config[toX][toY] = target_color;

        tuple<int,int,int,int,int> now_val = minimax(board_config, depth - 1 , alpha, beta, false);

        board_config[toX][toY] = store_color;
        board_config[frmX][frmY] = target_color;

        int eval = get<0>(now_val);
        if( maximizing_player) {
            ret_val = max(ret_val, make_tuple(eval, frmX, frmY, toX, toY) );
            alpha = max(alpha, eval);
        }
        else {
            ret_val = min(ret_val, make_tuple(eval, frmX, frmY, toX, toY) );
            beta = min(beta, eval);
        }
        if( alpha <= beta) break;
    }
    return ret_val;
}


int main(int argc, char* argv[]) {

    init_board(argv[1], argv[2]);

    while(1) {

        if(!my_turn()) continue;

        clock_t start = clock();
        tuple<int,int,int,int,int> ret = minimax(board, max_depth, -INF, INF, true);


        int mxVal ,x1, y1, x2, y2;
        tie(mxVal, x1, y1, x2, y2) = ret;
        write_move(x1, y1, x2, y2);

        fprintf(stderr, "time=%.3ldsec\n", (clock() - start)/CLOCKS_PER_SEC );

    }
    return 0;
}