#ifndef LOAD
#define LOAD
#include "common_functions.h"
void load_texture(GameState* gameState, SDL_Renderer* renderer, SDL_Texture* texture[], const char filePath[], int frame);
void load_background(GameState* gameState, SDL_Renderer* renderer, SDL_Texture* texture[], const char filePath[], int frame, int index);
void load_game(GameState* gameState, SDL_Renderer* renderer);
void load_player_info(GameState* gameState, float* px, float* py, float* pw, float* ph);

#endif