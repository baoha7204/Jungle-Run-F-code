#include "load.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void load_texture(GameState* gameState, SDL_Texture* texture[], const char filePath[], int frame) {
	SDL_Surface* surface;
	for (int i = 0; i < frame; i++) {
		char str[128] = "";
		sprintf_s(str, 128, filePath, i + 1);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		texture[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
}

void load_background(GameState* gameState, SDL_Texture* texture[], const char filePath[], const int frame, int index) {
	SDL_Surface* surface;
	for (int i = 0; i < frame; i++) {
		char str[128] = "";
		sprintf_s(str, 128, filePath, index + 1);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		texture[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
}

void load_game(GameState* gameState) {
	srand(time(NULL));
	// Init GameState 
	gameState->mode = GAMEMODE_SINGLEPLAYER; // will be handled later, which is selected by user
	gameState->statusState = STATUS_STATE_LIVES;
	gameState->label = NULL;
	gameState->player.x = 220;
	gameState->player.y = 220;
	gameState->player.dx = 0;
	gameState->player.dy = 0;
	gameState->player.flip = 0;
	gameState->player.status = 0;
	gameState->player.onLedge = 0;
	gameState->player.lives = 2;
	gameState->scrollX = 0;
	gameState->map->counter = 0;
	gameState->player.isTakenDamage = 0;
	// Load images and create rendering pictures from them
		// Create character with Idle movement
	load_texture(gameState, gameState->idle_anim, "Resource\\Character\\Idle\\Idle_%d.png", 12);
	// Create character with Run movement
	load_texture(gameState, gameState->run_anim, "Resource\\Character\\Run\\Run_%d.png", 8);
	// Create character with Jump movement
	load_texture(gameState, gameState->jump_anim, "Resource\\Character\\Jump\\Jump_%d.png", 4);
	// Create background
	SDL_Surface* surface;
	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			gameState->background[i].scrollSpeed = PLX_BACKGROUND_1;
		}
		else if (i == 1) {
			gameState->background[i].scrollSpeed = PLX_BACKGROUND_2;
		}
		else if (i == 2) {
			gameState->background[i].scrollSpeed = PLX_BACKGROUND_3;
		}
		else if (i == 3) {
			gameState->background[i].scrollSpeed = PLX_BACKGROUND_4;
		}
		else if (i == 4) {
			gameState->background[i].scrollSpeed = PLX_BACKGROUND_5;
		}
		// Set initial positions of background layers
		gameState->background[i].defaultX = 0;
		gameState->background[i].previousX = -WIDTH_WINDOW;
		gameState->background[i].nextX = WIDTH_WINDOW;
		// Create texture
		load_background(gameState, gameState->background[i].layer, "Resource\\Jungle Asset Pack\\parallax background\\plx-%d.png", 3, i);
	}
	// Create platform
	load_texture(gameState, gameState->platform, "Resource\\Some Bullshit Platform\\Platform %d.png", 5);
	// Platform[5]
	surface = IMG_Load("Resource\\Jungle Asset Pack\\jungle tileset\\jungle tileset.png");
	if (surface == NULL) {
		SDL_Quit();
		exit(1);
	}
	gameState->platform[5] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
	SDL_FreeSurface(surface);
	// Create fire trap
	load_texture(gameState, gameState->fire_trap, "Resource\\Foozle_2DTR0001_Pixel_Trap_Pack\\Fire Trap\\PNGs\\Fire Trap - Level %d.png", 3);
	// Load fonts
	gameState->font = TTF_OpenFont("Resource\\Fonts\\crazy-pixel.ttf", 48);
	if (!gameState->font) {
		SDL_Quit();
		exit(1);
	}
	// create a fixed map
	if (gameState->mode == GAMEMODE_SINGLEPLAYER) {
		Map* map = &gameState->map;
		// init ledges
		gameState->ledges[0].w = WIDTH_PLATFORM_3;
		gameState->ledges[0].h = HEIGHT_PLATFORM_3;
		gameState->ledges[0].x = 0;
		gameState->ledges[0].y = 800 - HEIGHT_PLATFORM_3;
		gameState->ledges[0].isLethal = 0;
		gameState->ledges[0].isBlocked = 1;
		for (int i = 1; i < NUM_OF_LEDGES; i++) {
			int additionalDis = 0;
			if (i == 3) { // lv0
				additionalDis = WIDTH_WINDOW;
				loadMap(&map[map->counter++], lv0, gameState->ledges[i - 1].x + WIDTH_PLATFORM_3, 0);
			}
			else if (i == 5) { // lv1
				additionalDis = WIDTH_WINDOW;
				loadMap(&map[map->counter++], lv1, gameState->ledges[i - 1].x + WIDTH_PLATFORM_3, 0);
			}
			gameState->ledges[i].x = gameState->ledges[i - 1].x + WIDTH_PLATFORM_3 + additionalDis;
			gameState->ledges[i].y = 800 - HEIGHT_PLATFORM_3;
			gameState->ledges[i].w = WIDTH_PLATFORM_3;
			gameState->ledges[i].h = HEIGHT_PLATFORM_3;
			gameState->ledges[i].isLethal = 0;
			gameState->ledges[i].isBlocked = 1;
		}
	}
}

void load_player_info(GameState* gameState, float* px, float* py, float* pw, float* ph) {
	*pw = WIDTH_PLAYER_IDLE * 2, * ph = HEIGHT_PLAYER_IDLE * 2;
	if (gameState->player.status == 0) {
		*pw = WIDTH_PLAYER_IDLE * 2, * ph = HEIGHT_PLAYER_IDLE * 2;
	}
	else if (gameState->player.status == 1) {
		*pw = WIDTH_PLAYER_IDLE * 2, * ph = HEIGHT_PLAYER_IDLE * 2;
	}
	else if (gameState->player.status == 2 || gameState->player.status == 3 || gameState->player.status == 4) {
		*pw = WIDTH_PLAYER_JUMP * 2, * ph = HEIGHT_PLAYER_JUMP * 2;
	}
	*px = gameState->player.x, * py = gameState->player.y;
}