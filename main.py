import pygame
from loa.constants import *
from loa.game import Game

pygame.init()
pygame.font.init()

BOARD_SIZE = 8
BOARD_WIDTH = SQUARE_SIZE * BOARD_SIZE

SCREEN = pygame.display.set_mode((BOARD_WIDTH, BOARD_WIDTH))
pygame.display.set_caption("Lines Of Action(LOA) Game")




def get_board_cell(pos):
    (row, col) = pos
    return row//SQUARE_SIZE, col//SQUARE_SIZE


def main():
    run = True
    game = Game(SCREEN, BOARD_SIZE)
    while run:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                col, row = get_board_cell(event.pos)
                game.select(row, col)

            elif not game.game_over and AI_vs_HUMAN_MODE and game.turn == WHITE_PIECE:
                row1, col1 = map(int, input().split())
                game.select(row1, col1)
                row2, col2 = map(int, input().split())
                game.select(row2, col2)

        pygame.display.update()
    pygame.quit()


main()
