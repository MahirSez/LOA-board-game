#include<bits/stdc++.h>
#define BLACK 1
#define WHITE 2
using namespace std;

int N, my_color, enemy_color;

const int INF = 1e9;
const int WIN_VAL = 1e5;
const int LOSE_VAL = -1e5;

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
    if(color == BLACK) {
        for(int i =0 ; i < N ; i++ ) {
            for(int j =0 ; j < N ; j++ ) {
                if(board_config[i][j] == color) ret.push_back({i,j});
            }
        }
    }
    else if(color == WHITE) {
        for(int j = 0 ; j < N ; j++) {
            for(int i = N-1 ; i >=0 ; i--) {
                if(board_config[i][j] == color) ret.push_back({i,j});
            }
        }
    }
    
    else assert(0);
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

int eval_fun_connected_component(const vector< vector<int> > &board_config) {

    int ret = 0;

    for(int i =0 ; i < N ; i++ ) {
        for(int j =0 ; j < N ; j++ ) {
            if(board_config[i][j] == my_color) {
                for(int k =0 ; k < 8 ; k++ ) {
                    int ii = i + dx[k];
                    int jj = j + dy[k];
                    if(!outside_board(ii,jj) && board_config[ii][jj] == my_color) ret++;
                }
            }
            else if(board_config[i][j] == enemy_color) {
                for(int k =0 ; k < 8 ; k++ ) {
                    int ii = i + dx[k];
                    int jj = j + dy[k];
                    if(!outside_board(ii,jj) && board_config[ii][jj] == enemy_color) ret--;
                }
            }
        }
    }
    return ret;
}

int get_shortest_dist_sum(const vector< pair<int,int> > &pos) {
    int dist_sum = 0;

    for(int i =0 ; i < pos.size() ; i++ ) {
        for(int j= i +1 ; j < pos.size(); j++ ) {
            int dist1 = abs(pos[i].first - pos[j].first);
            int dist2 = abs(pos[i].second - pos[j].second);
            dist_sum += max(dist1, dist2);

        }
    }
    return dist_sum;
}

int get_dist_sum_from_center_of_gravity(const vector< pair<int,int> > &pos) {
    if(pos.size() == 0 ) return 0;

    int x_sum = 0 , y_sum = 0;
    for(auto xx : pos) x_sum += xx.first , y_sum += xx.second;
    int center_x = x_sum / pos.size();
    int center_y = y_sum / pos.size();

    int dist_sum = 0;

    for(auto xx : pos) {
        int dist1 = abs(xx.first - center_x);
        int dist2 = abs(xx.second - center_y);
        dist_sum += max(dist1 , dist2);
    }

    return dist_sum;
}
int eval_fun_shortest_distance_sum(const vector< vector<int> > &board_config)  {

    vector< pair<int,int> >my_pos = get_piece_positions(board_config, my_color);
    vector< pair<int,int> >enemy_pos = get_piece_positions(board_config, enemy_color);
    int my_dist_sum = get_shortest_dist_sum(my_pos);
    int enemy_dist_sum = get_shortest_dist_sum(enemy_pos);
    return enemy_dist_sum - my_dist_sum;
}

int eval_fun_distance_from_center_of_gravity(const vector< vector<int> > &board_config) {

    vector< pair<int,int> >my_pos = get_piece_positions(board_config, my_color);
    vector< pair<int,int> >enemy_pos = get_piece_positions(board_config, enemy_color);
    int my_dist_sum = get_dist_sum_from_center_of_gravity(my_pos);
    int enemy_dist_sum = get_dist_sum_from_center_of_gravity(enemy_pos);
    return enemy_dist_sum - my_dist_sum;
}

int eval_fun_mobility_sum(const vector< vector<int> > &board_config) {

    vector< pair<int,int> >my_pos = get_piece_positions(board_config, my_color);
    vector< tuple<int, int, int, int> > my_possible_moves = get_all_possible_moves(board_config, my_pos, my_color );

    vector< pair<int,int> >enemy_pos = get_piece_positions(board_config, enemy_color);
    vector< tuple<int, int, int, int> > enemy_possible_moves = get_all_possible_moves(board_config, enemy_pos, enemy_color );

    return my_possible_moves.size() - enemy_possible_moves.size();
}


int evaluation_function(const vector< vector<int> > &board_config) {
    // return eval_fun_mobility_sum(board_config);
    // return eval_fun_connected_component(board_config);
    // return eval_fun_distance_from_center_of_gravity(board_config);
    return eval_fun_shortest_distance_sum(board_config);
}

int is_game_over(vector< vector<int> >board_config, bool last_move_by_me) {

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
            board_config[xx][yy] = 0;
            q.push(make_tuple(top_color, xx, yy));
	    }
	}


	int white_won = (white_cnt == tot_white);
	int black_won = (black_cnt == tot_black);

	int last_moved_color = last_move_by_me ? my_color : enemy_color;

    if(last_moved_color == WHITE && white_won) return WHITE;
    if(last_moved_color == BLACK && black_won) return BLACK;
    if(white_won) return WHITE;
    if(black_won) return BLACK;
    return 0;
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

pair<int,int> evaluate_terminating_condition(vector< vector<int> > board_config,int depth, bool last_move_by_me) {
    pair<int,int>ret = {0,0};
    int game_winner = is_game_over(board_config, last_move_by_me);

    if(game_winner == my_color)  ret = { (depth+1)*WIN_VAL + evaluation_function(board_config),1};
    else if(game_winner == enemy_color)  ret = { (depth+1)*LOSE_VAL + evaluation_function(board_config),1};
    else if(depth == 0 ) {
        int eval = evaluation_function(board_config);
        ret =  {eval,1};
    }
    return ret;
}


result minimax(vector< vector<int> > board_config,int depth,int alpha, int beta, bool maximizing_player ) {

    pair<int,int>is_leaf = evaluate_terminating_condition(board_config, depth, !maximizing_player);
    if(is_leaf.second == 1 ) {
        result terminal_result = result(  is_leaf.first , -1, -1 ,-1, -1);
        terminal_result.stak.push(board_config);
        return terminal_result;
    }

    int target_color = maximizing_player? my_color : enemy_color;


    result ret_result = maximizing_player ?  result(-INF, -1, -1, -1, -1) :  result(INF, -1, -1, -1, -1);

    vector< pair<int,int> >positions = get_piece_positions(board_config, target_color);

    vector< tuple<int, int, int, int> > possible_moves = get_all_possible_moves(board_config, positions,target_color );


    for(auto x : possible_moves) {

        int frmX, frmY, toX, toY;
        tie(frmX, frmY, toX, toY) = x;


        vector< vector<int > > tmp_board = board_config;



        int store_color = board_config[toX][toY];
        board_config[frmX][frmY] = 0;
        board_config[toX][toY] = target_color;


        result now_val = minimax(board_config, depth - 1 , alpha, beta, !maximizing_player);


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

        if( alpha >= beta) break;
    }
    ret_result.stak.push(board_config);
    return ret_result;
}


int main(int argc, char* argv[]) {

    init_board(argv[1], argv[2]);
    int max_depth = (N == 8 ? 4 : 5 );

    while(1) {

        if(!my_turn()) continue;

        clock_t start = clock();

        
        result ret = minimax(board, max_depth, -INF, INF, true);


        int mxVal = ret.val;
        int x1 = ret.frmX;
        int y1 = ret.frmY;
        int x2 = ret.toX;
        int y2 = ret.toY;

        cerr<<"game plan ->"<<endl;
        cerr<<ret.stak.size()<<endl;

        while(!ret.stak.empty()) {
            print_board(ret.stak.top());
            cerr<<endl;
            ret.stak.pop();
        }


        write_move(x1, y1, x2, y2);

        cerr<<"my color= "<<my_color<<" depth = "<<max_depth<<", maxVal = "<<mxVal<<" , coord= "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
        double time_taken =1.0*(clock() - start)/CLOCKS_PER_SEC;
        cerr<< time_taken<<"s"<<endl;
    }
    return 0;
}