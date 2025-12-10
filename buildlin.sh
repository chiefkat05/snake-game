echo "building for linux"

gcc ./*.c -o main `pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer`