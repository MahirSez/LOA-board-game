import pygame
import sys
from loa.constants import *
from loa.game import Game

pygame.init()
pygame.font.init()

BOARD_SIZE = int(sys.argv[1])
GAME_MODE = int(sys.argv[2])

BOARD_WIDTH = SQUARE_SIZE * BOARD_SIZE

SCREEN = pygame.display.set_mode((BOARD_WIDTH, BOARD_WIDTH))
pygame.display.set_caption("Lines Of Action(LOA) Game")

now_turn = 1


def read_file():
	ret = []
	f = open(SHARED_FILE, "r")
	line = f.readline().split()
	if len(line) == 0:
		return ret
	if line[0] == "0":
		ret = list(map(int, f.readline().split()))
	f.close()
	return ret


def write_file(line):
	f = open(SHARED_FILE, "w")
	f.write(str(line))
	f.close()


def init_players():
	if GAME_MODE == HUMAN_VS_HUMAN:  # human_vs_human
		return
	write_file(now_turn)


def get_board_cell(pos):
	(row, col) = pos
	return row // SQUARE_SIZE, col // SQUARE_SIZE


def change_turn():
	global now_turn
	now_turn = (((now_turn - 1) ^ 1) + 1)


def debug(line):
	print(line, file=sys.stderr)


def main():
	global now_turn
	run = True
	game = Game(SCREEN, BOARD_SIZE)
	while run:

		move = read_file()
		if len(move) == 4:

			if game.board.board_config[ move[0]][ move[1]] != now_turn:
				write_file(now_turn)
				continue

			game.select(move[0], move[1])
			turn_changed = game.select(move[2], move[3])

			if turn_changed != (-1, -1):

				change_turn()
				line = str(now_turn) + "\n"
				line += str(move[0]) + " " + str(move[1]) + " " + str(move[2]) + " " + str(move[3])
				write_file(line)
			else:
				write_file(now_turn)
			continue

		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				run = False
			elif GAME_MODE != AI_VS_AI and event.type == pygame.MOUSEBUTTONDOWN:
				col, row = get_board_cell(event.pos)
				turn_changed = game.select(row, col)
				if turn_changed != (-1, -1):
					change_turn()
					line = str(now_turn) + "\n"
					line += str(turn_changed[0]) + " " + str(turn_changed[1]) + " " + str(row) + " " + str(col)
					# debug(line)
					write_file(line)

		pygame.display.update()
	pygame.quit()


init_players()
main()
