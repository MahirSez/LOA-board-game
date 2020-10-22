from loa.board import Board
from loa.constants import *
import collections


class Game:
	def __init__(self, screen, board_size):
		self.board = Board(screen, board_size)
		self.turn = BLACK_PIECE
		self.opponent = WHITE_PIECE
		self.prev_selection = EMPTY_CELL
		self.prev_selected_coordinate = None
		self.game_over = False

	def select(self, row, col):

		if self.game_over or self.outside_board(row, col):
			return

		if self.board.board_config[row][col] == self.turn:
			self.board.draw()

			possible_moves = self.get_possible_moves(row, col)
			self.board.draw_blue_circles(possible_moves)

			self.change_turn(False, row, col)

		elif self.prev_selection == self.turn and self.is_valid_move(row, col):
			self.simulate_move(row, col)
			self.board.draw()
			self.board.draw_blue_circles([self.prev_selected_coordinate, (row, col)])
			self.board.draw_blue_line(self.prev_selected_coordinate, (row, col))

			self.change_turn(True, row, col)

		print()
		for rows in self.board.board_config:
			print(rows)
		print()

	def outside_board(self, row, col):
		return True if row < 0 or row >= self.board.board_size or col < 0 or col >= self.board.board_size else False

	def change_turn(self, turn_complete, row, col):

		if turn_complete:
			self.check_terminating_condition()

			self.opponent = self.turn
			self.turn = WHITE_PIECE if self.turn == BLACK_PIECE else BLACK_PIECE
			self.prev_selection = EMPTY_CELL
			self.prev_selected_coordinate = None
		else:

			self.prev_selection = self.turn
			self.prev_selected_coordinate = (row, col)

	def check_terminating_condition(self):

		win = self.check_if_win(self.turn)
		if not win:
			self.check_if_win(self.opponent)

	def check_if_win(self, piece_color):

		pos = self.get_any_pos(piece_color)
		assert pos is not None
		component_size = self.run_bfs(pos, piece_color)
		total_piece = sum(row.count(piece_color) for row in self.board.board_config)

		if component_size == total_piece:
			self.game_over = True
			self.board.draw_winner(piece_color)
			return True
		return False

	def run_bfs(self, pos, piece_color):

		connected_component = 0
		dx = [1, 1, 1, 0, 0, -1, -1, -1]
		dy = [-1, 0, 1, 1, -1, 1, 0, -1]

		visited = set()
		queue = collections.deque([pos])
		visited.add(pos)

		while queue:
			top = queue.popleft()
			connected_component += 1
			x = top[0]
			y = top[1]

			for i in range(8):
				xx = x + dx[i]
				yy = y + dy[i]

				if not self.outside_board(xx, yy) and self.board.board_config[xx][yy] == piece_color and (xx, yy) not in visited:
					visited.add((xx, yy))
					queue.append((xx, yy))

		return connected_component

	def get_any_pos(self, piece_color):

		for i in range(self.board.board_size):
			for j in range(self.board.board_size):
				if self.board.board_config[i][j] == piece_color:
					return i, j

		assert False

	def is_valid_move(self, row, col):
		prev_row, prev_col = self.prev_selected_coordinate
		assert prev_row is not None
		assert prev_col is not None

		possible_moves = self.get_possible_moves(prev_row, prev_col)
		return (row, col) in possible_moves

	def simulate_move(self, row, col):
		prev_row, prev_col = self.prev_selected_coordinate
		self.board.board_config[prev_row][prev_col] = EMPTY_CELL
		self.board.board_config[row][col] = self.turn

	def get_possible_moves(self, row, col):
		possible_moves = [(row, col)]

		row_move_dist = self.get_tot_piece_in_row(row, col)
		possible_moves += self.get_row_moves(row, col, row_move_dist)

		col_move_dist = self.get_tot_piece_in_col(row, col)
		possible_moves += self.get_col_moves(row, col, col_move_dist)

		diagonal1_move_dist = self.get_tot_piece_in_diagonal1(row, col)
		possible_moves += self.get_diagonal1_moves(row, col, diagonal1_move_dist)

		diagonal2_move_dist = self.get_tot_piece_in_diagonal2(row, col)
		possible_moves += self.get_diagonal2_moves(row, col, diagonal2_move_dist)
		return possible_moves

	def blocking_cell(self, row, col, dist_left):

		if self.outside_board(row, col):
			return True
		if dist_left == 0:
			return True if self.board.board_config[row][col] == self.turn else False
		if self.board.board_config[row][col] == self.opponent:
			return True
		return False

	def traverse_possible_moves(self, row, col, row_delta, col_delta, dist):
		possible_move = []
		for _ in range(dist - 1, -1, -1):
			row += row_delta
			col += col_delta
			if self.blocking_cell(row, col, _):
				return possible_move
		possible_move += [(row, col)]
		return possible_move

	def get_row_moves(self, row, col, dist):
		return self.traverse_possible_moves(row, col, 0, -1, dist) + self.traverse_possible_moves(row, col, 0, 1, dist)

	def get_col_moves(self, row, col, dist):
		return self.traverse_possible_moves(row, col, -1, 0, dist) + self.traverse_possible_moves(row, col, 1, 0, dist)

	def get_diagonal1_moves(self, row, col, dist):
		return self.traverse_possible_moves(row, col, 1, 1, dist) + self.traverse_possible_moves(row, col, -1, -1, dist)

	def get_diagonal2_moves(self, row, col, dist):
		return self.traverse_possible_moves(row, col, -1, 1, dist) + self.traverse_possible_moves(row, col, 1, -1, dist)

	def traverse_tot_pieces(self, row, col, row_delta, col_delta):
		cnt = 0
		while True:
			row += row_delta
			col += col_delta
			if self.outside_board(row, col):
				break
			cnt += 1 if self.board.board_config[row][col] != EMPTY_CELL else 0
		return cnt

	def get_tot_piece_in_row(self, row, col):
		return 1 + self.traverse_tot_pieces(row, col, 0, -1) + self.traverse_tot_pieces(row, col, 0, 1)

	def get_tot_piece_in_col(self, row, col):
		return 1 + self.traverse_tot_pieces(row, col, 1, 0) + self.traverse_tot_pieces(row, col, -1, 0)

	def get_tot_piece_in_diagonal1(self, row, col):
		return 1 + self.traverse_tot_pieces(row, col, 1, 1) + self.traverse_tot_pieces(row, col, -1, -1)

	def get_tot_piece_in_diagonal2(self, row, col):
		return 1 + self.traverse_tot_pieces(row, col, 1, -1) + self.traverse_tot_pieces(row, col, -1, 1)
