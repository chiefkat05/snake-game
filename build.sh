echo "building for temp"

set -e
gcc ./*.c -o main `pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer`

x86_64-w64-mingw32-gcc -Isdlsrc/include -Lsdlsrc/lib -Isdlsrc-image/include -Lsdlsrc-image/lib -Isdlsrc-mixer/include -Lsdlsrc-mixer/lib -o main.exe ./*.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image -lvorbis -logg

emcc --use-port=sdl2 --use-port=sdl2_image:formats=png --use-port=sdl2_mixer ./*.c -o index.js `sdl2-config --cflags --libs` -I/usr/include/SDL2/ --preload-file=./img/ --preload-file=./snd/

./main