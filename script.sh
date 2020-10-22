mkfifo pipe
g++ dummy_player.cpp -o dummy_player
./dummy_player < pipe  | python main.py > pipe
rm dummy_player
rm pipe