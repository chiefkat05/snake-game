echo "building for linux"

gcc ./*.c -o ../snake-game-builds/linux/main -O3 `pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer`