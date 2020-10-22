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
				pygame.draw.rect(self.screen, CREME_WHITE, (row * SQUARE_SIZE, col * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))

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

	def get_center_of_cell(self, pos):
		(row, col) = pos
		return col * SQUARE_SIZE + SQUARE_SIZE // 2, row * SQUARE_SIZE + SQUARE_SIZE // 2

	def draw_piece(self, row, col, loa_piece):

		piece_color = WHITE if loa_piece == WHITE_PIECE else BLACK
		center_of_cell = self.get_center_of_cell((row, col))
		pygame.draw.circle(self.screen, BLACK, center_of_cell, SQUARE_SIZE // 2 - 18)
		pygame.draw.circle(self.screen, piece_color, center_of_cell, SQUARE_SIZE // 2 - 20)

	def draw_blue_circles(self, possible_moves):

		for pos in possible_moves:
			center_of_cell = self.get_center_of_cell(pos)
			pygame.draw.circle(self.screen, BLUE, center_of_cell, SQUARE_SIZE // 2 - 40)

	def draw_blue_line(self, pos1, pos2):
		center_of_cell1 = self.get_center_of_cell(pos1)
		center_of_cell2 = self.get_center_of_cell(pos2)
		pygame.draw.line(self.screen, BLUE, center_of_cell1, center_of_cell2, 3)

	def draw_winner(self, piece_color):
		winner = "Black" if piece_color == BLACK_PIECE else "White"
		winner += " wins"

		font = pygame.font.SysFont("comicsansms", self.board_width // 10)
		text = font.render(winner, True, (255, 255, 255))
		self.screen.blit(text, (self.board_width // 4, 0))
