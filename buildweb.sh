echo 'building for web'

emcc --use-port=sdl2 --use-port=sdl2_image:formats=png --use-port=sdl2_mixer ./*.c -o ../snake-game-builds/web/index.js -O3 `sdl2-config --cflags --libs` -I/usr/include/SDL2/ --preload-file=./img/ --preload-file=./snd/
