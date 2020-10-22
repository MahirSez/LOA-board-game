import pygame
from loa.constants import *
from loa.game import Game

pygame.init()
pygame.font.init()

BOARD_SIZE = 6
BOARD_WIDTH = SQUARE_SIZE * BOARD_SIZE

SCREEN = pygame.display.set_mode((BOARD_WIDTH, BOARD_WIDTH))
pygame.display.set_caption("Lines Of Action(LOA) Game")




def main():
    run = True
    clock = pygame.time.Clock()
    game = Game(SCREEN, BOARD_SIZE)
    while run:
        clock.tick(FPS)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            if event.type == pygame.MOUSEBUTTONDOWN:
                col, row = event.pos
                row = row // SQUARE_SIZE
                col = col // SQUARE_SIZE
                game.select(row, col)
        pygame.display.update()
    pygame.quit()



main()
