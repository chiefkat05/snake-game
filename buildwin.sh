echo 'building for windows'

# x86_64-w64-mingw32-gcc ./*.c -o main.exe -static `pkg-config --libs --static sdl2 SDL2_image SDL2_mixer` -static-libgcc
x86_64-w64-mingw32-gcc -Isdlsrc/include -Lsdlsrc/lib -Isdlsrc-image/include -Lsdlsrc-image/lib -Isdlsrc-mixer/include -Lsdlsrc-mixer/lib -o main.exe ./*.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image -lvorbis -logg
# x86_64-w64-mingw32-gcc ./*.c -o main.exe -lsdl2
