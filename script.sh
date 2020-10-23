mkfifo pipe
g++ dummy_player.cpp -o dummy_player
./dummy_player < pipe  | python main.py > pipe
# python main.py < pipe | ./dummy_player 1 > pipe | ./dummy_player 0 > pipe



rm dummy_player
rm pipe