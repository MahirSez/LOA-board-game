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

int dx[] = {1, 1, 1, 0, 0, -1, -1, -1};
int	dy[] = {-1, 0, 1, 1, -1, 1, 0, -1};

void print_board(const vector< vector<int> >&board_config) {
    for(int i =0 ; i < N ; i++ ) {
        for(int j =0 ; j < N ; j++ ) cerr<<board_config[i][j]<<" ";
        cerr<<endl;
    }
}

vector< vector<int > > v1 = {
    {0, 0, 1, 0},
    {0, 0, 2, 2},
    {2, 1, 0, 2},
    {0, 1, 1, 0}
};

vector< vector<int > > v2 = {
    {0, 0, 0, 0},
    {0, 0, 2, 2},
    {2, 1, 0, 2},
    {0, 1, 1, 0}
};



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


    if(!first_move_done && my_color == BLACK) {
        first_move_done = true;
        return true;
    }


    int x1, y1, x2, y2;
    in>>x1>>y1>>x2>>y2;

//    cerr<<"i got "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;

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

bool blocking_cell(const vector< vector<int> >&board_config,int row,int col,int dist_left, int target_color) {

    if( outside_board(row, col) ) return true;
    if( dist_left == 0 ) return board_config[row][col] == target_color;

    if( board_config[row][col] == 0 ) return false;
    if( board_config[row][col] == target_color ) return false;
    return true;
}

vector< pair<int,int> >traverse_possible_moves(const vector< vector<int> >&board_config,int row,int col,int row_delta,int col_delta,int dist,int
target_color) {
	vector< pair<int,int> > ret;
	for(int i = dist -1 ; i >=0 ; i--) {
	    row += row_delta;
	    col += col_delta;
	    if(blocking_cell(board_config, row,col,i, target_color)) return ret;
	}
    ret.push_back({row,col});
	return ret;
}

vector< pair<int,int> > get_row_moves(const vector< vector<int> >&board_config,int row,int col,int dist,int target_color ) {
    vector< pair<int,int> > ret1  = traverse_possible_moves(board_config, row, col, 0, -1, dist, target_color);
    vector< pair<int,int> > ret2  = traverse_possible_moves(board_config, row, col, 0, 1, dist, target_color);
    ret1.insert(ret1.end() , ret2.begin(), ret2.end());
    return ret1;
}

vector< pair<int,int> > get_col_moves(const vector< vector<int> >&board_config,int row,int col,int dist, int target_color) {
    vector< pair<int,int> > ret1  = traverse_possible_moves(board_config, row, col, -1, 0, dist, target_color);
    vector< pair<int,int> > ret2  = traverse_possible_moves(board_config, row, col, 1, 0, dist, target_color);
    ret1.insert(ret1.end() , ret2.begin(), ret2.end());
    return ret1;
}

vector< pair<int,int> > get_diagonal1_moves(const vector< vector<int> >&board_config,int row,int col,int dist, int target_color) {
    vector< pair<int,int> > ret1  = traverse_possible_moves(board_config, row, col, 1, 1, dist, target_color);
    vector< pair<int,int> > ret2  = traverse_possible_moves(board_config, row, col, -1, -1, dist, target_color);
    ret1.insert(ret1.end() , ret2.begin(), ret2.end());
    return ret1;
}

vector< pair<int,int> > get_diagonal2_moves(const vector< vector<int> >&board_config,int row,int col,int dist, int target_color) {
    vector< pair<int,int> > ret1  = traverse_possible_moves(board_config, row, col, -1, 1, dist, target_color);
    vector< pair<int,int> > ret2  = traverse_possible_moves(board_config, row, col, 1, -1, dist, target_color);
    ret1.insert(ret1.end() , ret2.begin(), ret2.end());
    return ret1;
}

vector< pair<int,int> > get_possible_moves(const vector< vector<int> >&board_config, int row, int col,int target_color) {
    vector< pair<int,int> > ret;
    vector< pair<int,int> >tmp;

    

	int row_move_dist = get_tot_piece_in_row(board_config, row, col);
	tmp = get_row_moves(board_config, row, col, row_move_dist, target_color);
	ret.insert(ret.end() , tmp.begin() , tmp.end());

	int col_move_dist = get_tot_piece_in_col(board_config, row, col);
	tmp = get_col_moves(board_config, row, col, col_move_dist, target_color);
	ret.insert(ret.end() , tmp.begin() , tmp.end());

    int diagonal1_move_dist = get_tot_piece_in_diagonal1(board_config, row, col);
    tmp = get_diagonal1_moves(board_config, row, col, diagonal1_move_dist, target_color);
    ret.insert(ret.end() , tmp.begin() , tmp.end());

    int diagonal2_move_dist = get_tot_piece_in_diagonal2(board_config, row, col);
	tmp = get_diagonal2_moves(board_config, row, col, diagonal2_move_dist, target_color);
    ret.insert(ret.end() , tmp.begin() , tmp.end());


//    if(board_config == v1) {
//    print_board(board_config);
//        cerr<<"whyyy "<<row<<" "<<col<<endl;
//        for(auto x : ret) cerr<<x.first<<" "<<x.second<<endl;
//        cerr<<endl;
//    }

	return ret;
}


vector< tuple<int, int, int, int> >get_all_possible_moves(const vector< vector<int> >&board_config, const vector< pair<int,int> > &positions, int target_color) {

    vector< tuple<int, int, int, int> > ret;

    for(auto x : positions) {
        vector< pair<int, int> > possible_moves = get_possible_moves(board_config, x.first, x.second, target_color);
        for(auto y: possible_moves) {
            ret.push_back(make_tuple(x.first, x.second, y.first, y.second));
        }
    }
    return ret;
}

int heuristic_fun(const vector< vector<int> > &board_config) {

    int target_color = my_color;
    int ret = 0;
    // int tot_piece = 0;


    for(int i =0 ; i < N ; i++ ) {
        for(int j =0 ; j < N ; j++ ) {
            if(board_config[i][j] != target_color) continue;
            for(int k =0 ; k < 8 ; k++ ) {
                int ii = i + dx[k];
                int jj = j + dy[k];
                if(!outside_board(ii,jj) && board_config[ii][jj] == target_color) ret++;
            }
        }
    }
//    print_board(board_config);
//    cerr<<maximizing_player<< " "<<ret<<'\n';
//    cerr<<endl;
    return ret;
}

bool is_terminating_condition(vector< vector<int> >board_config) {

	int black_cnt = 0, white_cnt = 0;
	queue<tuple<int,int, int> >q;
	int tot_white = 0, tot_black = 0;

	for(int i =0 ;i <N ;i++ ) {
	    for(int j =0 ; j < N ; j++ ) {
	        tot_white += (board_config[i][j] == WHITE);
	        tot_black += (board_config[i][j] == BLACK);
	        if(black_cnt == 0 && board_config[i][j] == BLACK) {
	            black_cnt = 1;
	            q.push(make_tuple(BLACK, i,j));
	            board_config[i][j] = 0;
	        }
	        else if(white_cnt == 0 && board_config[i][j] == WHITE) {
	            white_cnt = 1;
	            q.push(make_tuple(WHITE, i,j));
	            board_config[i][j] = 0;
	        }
	    }
	}

	while(!q.empty()) {
	    tuple<int,int, int>top = q.front();
	    int top_color , row, col;
	    tie(top_color, row, col) = top;
	    q.pop();


	    for(int i =0 ; i < 8 ; i++ ) {
	        int xx = row + dx[i];
	        int yy = col + dy[i];
	        if(outside_board(xx,yy)) continue;
	        if(board_config[xx][yy] != top_color) continue;

            if(top_color == WHITE) white_cnt++;
            else if(top_color == BLACK) black_cnt++;
            else assert(0);
            board_config[xx][yy] = 0;
            q.push(make_tuple(top_color, xx, yy));
	    }
	}
	return (white_cnt == tot_white || black_cnt == tot_black);
}




class result {
public:
    int val;
    int frmX, frmY;
    int toX, toY;

    stack< vector< vector<int> > > stak;

    result(int _val, int _frmX, int _frmY, int _toX, int _toY ) : val(_val) , frmX(_frmX) , frmY(_frmY) , toX(_toX) , toY(_toY) {}
};

result max(result &r1, result &r2) {
    return r1.val >= r2.val ? r1 : r2;
}

result min( result &r1, result &r2) {
    return r1.val <= r2.val ? r1 : r2;
}

result minimax(vector< vector<int> > board_config,int depth,int alpha, int beta, bool maximizing_player ) {
    if(depth == 0 || is_terminating_condition(board_config)) {
        result terminal_result = result( heuristic_fun(board_config) , -1, -1 ,-1, -1);
        terminal_result.stak.push(board_config);
        return terminal_result;
    }

    int target_color = maximizing_player? my_color : enemy_color;


    result ret_result = maximizing_player ?  result(-INF, -1, -1, -1, -1) :  result(INF, -1, -1, -1, -1);

    vector< pair<int,int> >positions = get_piece_positions(board_config, target_color);
    vector< tuple<int, int, int, int> > possible_moves = get_all_possible_moves(board_config, positions,target_color );

//    if(board_config == v1) {
//        cerr<<target_color<<endl;
//        print_board(board_config);
//        cerr<<"positions->"<<endl;
//
//        for(auto x : positions) {
//            cerr<<x.first<<" "<<x.second<<endl;
//        }
//        cerr<<endl;
//        cerr<<"moves->"<<endl;
//
//         for(auto x : possible_moves) {
//             int x1 , y1, x2, y2;
//             tie(x1, y1, x2, y2) = x;
//             cerr<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
//         }
//         cerr<<endl;
//     }

    // for(auto x : positions) {
    //     cerr<<x.first<<" "<<x.second<<endl;
    // }
    // cerr<<endl;
    


    // cout<<target_color<<" "<<enemy_color<<endl;
    // cerr<<possible_moves.size()<<endl;

    

    for(auto x : possible_moves) {

        int frmX, frmY, toX, toY;
        tie(frmX, frmY, toX, toY) = x;


        vector< vector<int > > tmp_board = board_config;



        int store_color = board_config[toX][toY];
        board_config[frmX][frmY] = 0;
        board_config[toX][toY] = target_color;


        result now_val = minimax(board_config, depth - 1 , alpha, beta, !maximizing_player);


        if(tmp_board == v1 && board_config == v2) {
            print_board(tmp_board);
            cerr<<endl;
            print_board(board_config);
            cerr<<endl;
            cerr<<frmX<<" "<<frmY<<" "<<toX<<" "<<toY<<endl;
            assert(0);
        }


        int eval = now_val.val;
        result now_result = result(eval, frmX, frmY, toX, toY);
        now_result.stak = now_val.stak;


        board_config[toX][toY] = store_color;
        board_config[frmX][frmY] = target_color;


        if( maximizing_player) {
            ret_result = max(ret_result, now_result);
            alpha = max(alpha, eval);
        }
        else {
            ret_result = min(ret_result, now_result );
            beta = min(beta, eval);
        }

//        if( now_result == ret_result ) {
//            if( !stak.empty() && stak.top().first == depth) stak.pop();
//            stak.push({depth, board_config});
//        }

        if( alpha >= beta) break;
    }
    ret_result.stak.push(board_config);
    return ret_result;
}


int main(int argc, char* argv[]) {

    init_board(argv[1], argv[2]);

    while(1) {

        if(!my_turn()) continue;

        clock_t start = clock();

        
        result ret = minimax(board, max_depth, -INF, INF, true);


        int mxVal = ret.val;
        int x1 = ret.frmX;
        int y1 = ret.frmY;
        int x2 = ret.toX;
        int y2 = ret.toY;

        // if(x1 ==-1 || y1 == -1 || x2 == -1 || y2 == -1 ) break;

        cerr<<ret.stak.size()<<endl;

        while(!ret.stak.empty()) {
            print_board(ret.stak.top());
            cerr<<endl;
            ret.stak.pop();
        }
        

        write_move(x1, y1, x2, y2);

        cerr<<"my color= "<<my_color<<" , maxVal = "<<mxVal<<" , coord= "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
        cerr<<1.0*(clock() - start)/CLOCKS_PER_SEC <<"s"<<endl;
        

    }
    return 0;
}