all: rpg_game

CFLAGS = `sdl2-config --cflags`
LIBS = `sdl2-config --static-libs` -lSDL2_ttf

rpg_game: main.c
	gcc -o rpg_game $(CFLAGS) *.c $(LIBS)

run: rpg_game
	./rpg_game.exe

srun: rpg_game
	start ./rpg_game.exe
