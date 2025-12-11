echo 'building for windows'

x86_64-w64-mingw32-gcc -Isdlsrc/include -Lsdlsrc/lib -Isdlsrc-image/include -Lsdlsrc-image/lib -Isdlsrc-mixer/include -Lsdlsrc-mixer/lib -o ../snake-game-builds/windows/main.exe ./*.c -O3 -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image -lvorbis -logg