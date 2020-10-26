#!/bin/bash


# echo "1" > shared.txt
# g++ -std=c++11 one.cpp -o one

# ./one 8 1


BOARD_SIZE=6
HUMAN_VS_HUMAN=1
AI_VS_HUMAN=2
AI_VS_AI=3

GAME_MODE=$AI_VS_HUMAN

SHARED_FILE=shared.txt

if test -f "$SHARED_FILE" ;then
  rm $SHARED_FILE
fi


if [ $GAME_MODE == $AI_VS_HUMAN ] ;then
  g++ -std=c++11 one.cpp -o one
  python main.py $BOARD_SIZE $GAME_MODE | ./one $BOARD_SIZE 1 | ./one $BOARD_SIZE 2
fi





#
#if [ $GAME_MODE == $AI_VS_AI ]; then
#    ./dummy_player $BOARD_SIZE 2
#fi


