echo 'building for web'

emcc -sALLOW_MEMORY_GROWTH --use-port=sdl2 --use-port=sdl2_image:formats=png --use-port=sdl2_mixer ./src/*.c -o ../snake-game-builds/web/index.js `sdl2-config --cflags --libs` -I/usr/include/SDL2/ --preload-file=./img/ --preload-file=./snd/
