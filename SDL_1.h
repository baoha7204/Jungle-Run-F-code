#ifndef SDL_1
#define SDL_1
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "structs.h"

int processEvents(SDL_Window* window, GameState* gameState);
void load_game(GameState* gameState);
void do_render(GameState* gameState);
void add_physics(GameState* gameState);
void collision_detect(GameState* gameState);
#endif
