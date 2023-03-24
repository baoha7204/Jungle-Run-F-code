#ifndef SDL_1
#define SDL_1
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define WIDTH_WINDOW 1200
#define HEIGHT_WINDOW 800
#define WIDTH_PLAYER_IDLE 19
#define HEIGHT_PLAYER_IDLE 32
#define WIDTH_PLAYER_RUN 21
#define HEIGHT_PLAYER_RUN 33
#define WIDTH_PLAYER_LEDGEGRAB 20
#define HEIGHT_PLAYER_LEDGEGRAB 40
#define WIDTH_PLAYER_JUMP 20
#define HEIGHT_PLAYER_JUMP 35
#define WIDTH_PLATFORM_1 235
#define HEIGHT_PLATFORM_1 55
#define WIDTH_PLATFORM_3 336
#define HEIGHT_PLATFORM_3 159
#define WIDTH_PLATFORM_5 402
#define HEIGHT_PLATFORM_5 62
#define NUM_OF_LEDGES 100
#define GRAVITY 0.12f
#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

typedef struct {
	float x, y;
	float dx, dy;
	short lives;
	char* name;
	int onLedge; // 0 = false, 1 = true
	int flip, status; // 0 = idle, 1 = run, 2 = jump, 3 = mid air, 4 = falling, 5 = ledge grab
} Object;

typedef struct {
	int x, y, w, h;
} Ledge;

typedef struct {
	// Scroll offset
	float scrollX;
	// Players
	Object player;
	// Status State
	int statusState;
	// Ledges
	Ledge ledges[NUM_OF_LEDGES];
	// Images
		// Character animation
	SDL_Texture* idle_anim[12];
	SDL_Texture* jump_anim[4];
	SDL_Texture* ledgeGrab_anim[6];
	SDL_Texture* run_anim[8];
		// Golem
	SDL_Texture* golem;
		// Platform
	SDL_Texture* platform[5];
		// Background
	SDL_Texture* background[5];
		// Label
	SDL_Texture* label;
	// Fonts
	TTF_Font* font;
	// Renderer
	SDL_Renderer* renderer;
} GameState;

int processEvents(SDL_Window* window, GameState* gameState);
void load_game(GameState* gameState);
void do_render(GameState* gameState);
void add_physics(GameState* gameState);
void collision_detect(GameState* gameState);
#endif
