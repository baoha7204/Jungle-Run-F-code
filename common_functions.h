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
void detectPlayer(GameState* gameState, Map* map, int row, int col, float dt, float px, float py, float pw, float ph);
void updateGolem(Map* map, int row, int col, float dt);
void updateBoss(GameState* gameState, float dt);
int touchBoss(GameState* gameState);
int Over(GameState* gameState);
void immortal_events(GameState* gameState);
#endif
