#ifndef SDL_1
#define SDL_1
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "structs.h"

int animation_smoothness(int frame, const int frames);
void add_physics(GameState* gameState);
int processEvents(SDL_Window* window, GameState* gameState);
void onTrapHit(GameState* gameState, SDL_Texture* texture, int x, int y);
void do_render(GameState* gameState);
void render_character_animation(GameState* gameState, SDL_Texture* texture[], const int width, const int height, int frame);
#endif
