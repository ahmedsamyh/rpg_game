all: rpg_game

rpg_game: main.c
	gcc -I./SDL2/include -o rpg_game main.c sdl_helper.c -L./SDL2/lib/Release -lSDL2 -lSDL2-static
