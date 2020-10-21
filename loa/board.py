import pygame

from loa.constants import *


class Board:
	def __init__(self, screen, board_size):
		self.board_config = [[EMPTY_CELL] * board_size for _ in range(board_size)]
		self.screen = screen
		self.board_size = board_size
		self.board_width = board_size * SQUARE_SIZE
		self.init_pieces()

		self.draw()

	def draw(self):
		self.draw_board()
		self.draw_all_pieces()

	def draw_board(self):
		pygame.draw.rect(self.screen, BROWN, (0, 0, self.board_width, self.board_width))

		for row in range(0, self.board_size):
			for col in range(row % 2, self.board_size, 2):
				pygame.draw.rect(self.screen, CREME_WHITE,
								 (row * SQUARE_SIZE, col * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))

	def init_pieces(self):

		for col in range(1, self.board_size - 1):
			self.board_config[0][col] = BLACK_PIECE
			self.board_config[self.board_size - 1][col] = BLACK_PIECE

		for row in range(1, self.board_size - 1):
			self.board_config[row][0] = WHITE_PIECE
			self.board_config[row][self.board_size - 1] = WHITE_PIECE

	def draw_all_pieces(self):
		for row in range(self.board_size):
			for col in range(self.board_size):
				if self.board_config[row][col] != 0:
					self.draw_piece(row, col, self.board_config[row][col])

	def draw_piece(self, row, col, loa_piece):

		piece_color = WHITE if loa_piece == WHITE_PIECE else BLACK

		circle_centerX = col * SQUARE_SIZE + SQUARE_SIZE // 2
		circle_centerY = row * SQUARE_SIZE + SQUARE_SIZE // 2

		pygame.draw.circle(self.screen, BLACK, (circle_centerX, circle_centerY), SQUARE_SIZE // 2 - 18)
		pygame.draw.circle(self.screen, piece_color, (circle_centerX, circle_centerY), SQUARE_SIZE // 2 - 20)

	def show_possible_moves(self, possible_moves):

		for pos in possible_moves:
			(row, col) = pos

			circle_centerX = col * SQUARE_SIZE + SQUARE_SIZE // 2
			circle_centerY = row * SQUARE_SIZE + SQUARE_SIZE // 2

			pygame.draw.circle(self.screen, BLUE, (circle_centerX, circle_centerY), SQUARE_SIZE // 2 - 40)

	def draw_winner(self, piece_color):
		winner = "Black" if piece_color == BLACK_PIECE else "White"
		winner += " wins"

		pygame.draw.rect(self.screen, BLACK, (0, 0, self.board_width, self.board_width))

		font = pygame.font.SysFont("comicsansms", 50)
		text = font.render(winner, True, (255,255,255))
		self.screen.blit(text, (self.board_width//4, self.board_width//2) )

	def destroy(self):
		self.board_config = [[EMPTY_CELL] * self.board_size for _ in range(self.board_size)]

# def draw_turn_rect(self, turn):
#
# 	rect_color = WHITE if turn == WHITE_PIECE else BLACK
# 	pygame.draw.line(self.screen, BLACK, (0, self.board_width), (self.board_width, self.board_width) ,10)
# 	pygame.draw.rect(self.screen, rect_color, (0, self.board_width, self.board_width , self.board_width))
