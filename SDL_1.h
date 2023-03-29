#ifndef SDL_1
#define SDL_1
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "structs.h"

void load_texture(GameState* gameState, SDL_Texture* texture[], const char filePath[], int frame);
int processEvents(SDL_Window* window, GameState* gameState);
void load_game(GameState* gameState);
void do_render(GameState* gameState);
void add_physics(GameState* gameState);
void collision_detect_floor(GameState* gameState);
void collision_detect_map(GameState* gameState, Map* map);
void collision_correction(GameState* gameState, float px, float py, float pw, float ph, float bx, float by, float bw, float bh);
void load_player_info(GameState* gameState, float* px, float* py, float* pw, float* ph);
int animation_smoothness(int frame, const int frames);
void onTrapHit(GameState* gameState, SDL_Texture* texture, int x, int y);
void render_character_animation(GameState* gameState, SDL_Texture* texture[], const int width, const int height, int frame);
#endif
