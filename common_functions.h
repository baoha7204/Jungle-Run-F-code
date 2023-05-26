#ifndef COMMON_FUNCTIONS
#define COMMON_FUNCTIONS
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "structs.h"
#include <stdbool.h>

void gamePlay(GameState* gameState, SDL_Renderer* renderer, SDL_Window* window);
int animation_smoothness(int frame, const int frames);
void add_physics(GameState* gameState);
bool processEvents(SDL_Window* window, GameState* gameState);
void onTrapHit(GameState* gameState, SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);
void detectPlayer(GameState* gameState, Map* map, int row, int col, float dt, float px, float py, float pw, float ph);
void updateGolem(Map* map, int row, int col, float dt);
void updateBoss(GameState* gameState, float dt);
int touchBoss(GameState* gameState);
void Over(GameState* gameState, SDL_Renderer* renderer, const char str[], SDL_Color fg, bool* done);
void immortal_events(GameState* gameState);
void clean(GameState* gameState, SDL_Renderer* renderer, SDL_Window* window);
#endif
