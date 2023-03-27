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
void collision_detect_floor(GameState* gameState);
void collision_detect_map(GameState* gameState, Map* map);
void collision_correction(GameState* gameState, float px, float py, float pw, float ph, float bx, float by, float bw, float bh);
void load_player_info(GameState* gameState, float* px, float* py, float* pw, float* ph);
#endif
