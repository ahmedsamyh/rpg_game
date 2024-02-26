all: rpg_game

CFLAGS = `sdl2-config --cflags` -I./include
LIBS = `sdl2-config --static-libs` -lSDL2_ttf
SRC_DIR=./src

rpg_game: $(SRC_DIR)/main.c
	gcc -o rpg_game $(CFLAGS) $(SRC_DIR)/*.c $(LIBS)
