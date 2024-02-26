all: rpg_game embed

CFLAGS=`sdl2-config --cflags` -I./include -ggdb
LIBS=`sdl2-config --static-libs` -lSDL2_ttf
SRC_DIR=./src
TOOL_DIR=./tools

rpg_game: $(SRC_DIR)/main.c
	gcc -o rpg_game $(CFLAGS) $(SRC_DIR)/*.c $(LIBS)

embed: $(TOOL_DIR)/embed.c
	gcc -o embed $(TOOL_DIR)/embed.c -I./include

run: rpg_game
	./rpg_game
