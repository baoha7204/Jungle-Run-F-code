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
	gameState->dt = 0;
	gameState->statusState = STATUS_STATE_GAME;
	gameState->mode = GAMEMODE_SINGLEPLAYER; // will be handled later, which is selected by user
	gameState->difficulty = DIFFICULTY_EASY; // will be handled later, which is selected by user
	gameState->label = NULL;
	gameState->player.x = 320;
	gameState->player.y = 220;
	gameState->player.dx = 0;
	gameState->player.dy = 0;
	gameState->player.flip = 0;
	gameState->player.status = 0;
	gameState->player.onLedge = 0;
	gameState->player.isImmortal = 0;
	gameState->player.immortalStartTime = 0;
	gameState->health_potion_counter = 0;
	// Init giga golem
	gameState->gigaGolem.flip = 0;
	gameState->gigaGolem.d.x = 0;
	gameState->gigaGolem.pos.x = 0;
	gameState->gigaGolem.pos.y = (HEIGHT_WINDOW-HEIGHT_GIGA_GOLEM) / 2;
	if (gameState->difficulty == DIFFICULTY_EASY) {
		gameState->player.lives = MAX_HEALTH_EASY;
	}
	else if (gameState->difficulty == DIFFICULTY_MEDIUM) {
		gameState->player.lives = MAX_HEALTH_MEDIUM;
	} else if (gameState->difficulty == DIFFICULTY_HARD) {
		gameState->player.lives = MAX_HEALTH_HARD;
	}
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
	// Create golem
	load_texture(gameState, gameState->golem, "Resource\\Mecha-stone Golem 0.1\\PNG sheet\\Character_sheet.png", 1);
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
	// Create ceiling trap
	load_texture(gameState, gameState->ceiling_trap, "Resource\\Foozle_2DTR0001_Pixel_Trap_Pack\\Ceiling Trap\\PNGs\\Ceiling Trap - Level %d.png", 1);
	// Create saw trap
	load_texture(gameState, gameState->saw_trap, "Resource\\Foozle_2DTR0001_Pixel_Trap_Pack\\Saw Trap\\PNGs\\Saw Trap - Level %d.png", 1);
	// Create lightning trap
	load_texture(gameState, gameState->lightning_trap, "Resource\\Foozle_2DTR0001_Pixel_Trap_Pack\\Lightning Trap\\PNGs\\Lightning Trap - Level %d.png", 2);
	// Create items
	load_texture(gameState, gameState->items, "Resource\\platformer items\\animated_items.png", 1);
	// Load fonts
	gameState->font = TTF_OpenFont("Resource\\Fonts\\crazy-pixel.ttf", 48);
	if (!gameState->font) {
		SDL_Quit();
		exit(1);
	}
	// create a fixed map
	if (gameState->mode == GAMEMODE_SINGLEPLAYER) {
		drawMap_SingleplayerMode(gameState);
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