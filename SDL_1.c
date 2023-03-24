#include "SDL_1.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int processEvents(SDL_Window* window, GameState* gameState) {
	// Walking
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
		gameState->player.status = 1;
		gameState->player.flip = 1;
		gameState->player.dx -= 0.3f;
		if (gameState->player.dx < -2.5) {
			gameState->player.dx = -2.5;
		}
	}
	else if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
		gameState->player.status = 1;
		gameState->player.flip = 0;
		gameState->player.dx += 0.3f;
		if (gameState->player.dx > 2.5) {
			gameState->player.dx = 2.5;
		}
	}
	else if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
		gameState->player.dy += 0.3f;
	}
	else {
		gameState->player.status = 0;
		gameState->player.flip = 0;
		gameState->player.dx *= 0.8f;
		if (fabsf(gameState->player.dx) < 0.1f) {
			gameState->player.dx = 0;
		}
	}
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// Exit program
		switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE:
			if (window) {
				SDL_DestroyWindow(window);
				window = NULL;
				return 1;
			}
			break;
		case SDL_QUIT:
			return 1;
		}
		// Interact with keyboard
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				if (gameState->player.onLedge) {
					gameState->player.onLedge = 0;
					gameState->player.dy = -9;
				}
				break;
			}
		}
	}
}

void load_game(GameState* gameState) {
	// Init GameState 
	gameState->label = NULL;
	gameState->player.x = 220;
	gameState->player.y = 220;
	gameState->player.dx = 0;
	gameState->player.dy = 0;
	gameState->player.flip = 0;
	gameState->player.status = 0;
	gameState->player.onLedge = 0;
	gameState->statusState = STATUS_STATE_LIVES;
	gameState->player.lives = 3;
	gameState->scrollX = 0;
	// Load images and create rendering pictures from them
	SDL_Surface* surface;
		// Create character with Idle movement
	for (int i = 0; i < 12; i++) {
		char str[128] = "";
		sprintf_s(str, 128, "C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Character\\Idle\\Idle_%d.png", i);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->idle_anim[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
		// Create character with Run movement
	for (int i = 0; i < 8; i++) {
		char str[128] = "";
		sprintf_s(str, 128, "C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Character\\Run\\Run_%d.png", i);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->run_anim[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
		// Create character with Jump movement
	for (int i = 0; i < 4; i++) {
		char str[128] = "";
		sprintf_s(str, 128, "C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Character\\Jump\\Jump_%d.png", i);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->jump_anim[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
		// Create character with ledge grab
	for (int i = 0; i < 6; i++) {
		char str[128] = "";
		sprintf_s(str, 128, "C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Character\\Ledge Grab\\Ledge Grab_%d.png", i);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->ledgeGrab_anim[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
	// Create Golem
	surface = IMG_Load("C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Mecha-stone Golem 0.1\\PNG sheet\\Character_sheet.png");
	if (surface == NULL) {
		SDL_Quit();
		exit(1);
	}
	gameState->golem = SDL_CreateTextureFromSurface(gameState->renderer, surface);
	SDL_FreeSurface(surface);
	// Create background
	for (int i = 0; i < 5; i++) {
		char str[128] = "";
		sprintf_s(str, 128, "C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Jungle Asset Pack\\parallax background\\plx-%d.png", i + 1);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->background[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
	// Create platform
	for (int i = 0; i < 5; i++) {
		char str[128] = "";
		sprintf_s(str, 128, "C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Some Bullshit Platform\\Platform %d.png", i + 1);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->platform[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
	// Load fonts
	gameState->font = TTF_OpenFont("C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Fonts\\crazy-pixel.ttf", 48);
	if (!gameState->font) {
		SDL_Quit();
		exit(1);
	}
	// init ledges
	for (int i = 0; i < NUM_OF_LEDGES; i++) {
		gameState->ledges[i].w = WIDTH_PLATFORM_1;
		gameState->ledges[i].h = HEIGHT_PLATFORM_1;
		gameState->ledges[i].x = i * WIDTH_PLATFORM_1;
		gameState->ledges[i].y = 800 - HEIGHT_PLATFORM_1;
	}
	gameState->ledges[NUM_OF_LEDGES - 1].x = 800;
	gameState->ledges[NUM_OF_LEDGES - 1].y = 400;
	gameState->ledges[NUM_OF_LEDGES - 2].x = 800 + WIDTH_PLATFORM_1;
	gameState->ledges[NUM_OF_LEDGES - 2].y = 400;
}

void do_render(GameState* gameState) {
	SDL_Renderer* renderer = gameState->renderer;
	// Create background
	for (int i = 0; i < 5; i++) {
		SDL_RenderCopy(renderer, gameState->background[i], NULL, NULL);
	}
	// draw ledges
	for (int i = 0; i < NUM_OF_LEDGES; i++) {
		SDL_Rect srcRect = { 341, 172, gameState->ledges[i].w, gameState->ledges[i].h };
		SDL_Rect ledgeRect = { gameState->scrollX + gameState->ledges[i].x, gameState->ledges[i].y , gameState->ledges[i].w, gameState->ledges[i].h };
		SDL_RenderCopyEx(renderer, gameState->platform[0], &srcRect, &ledgeRect, 0, NULL, 0);
	}
	if (gameState->player.status == 0) { // idle 
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_IDLE, HEIGHT_PLAYER_IDLE };
		// animation smoothness
		size_t animation_speed = SDL_GetTicks64() / 120;
		int i = animation_speed % 12;
		SDL_RenderCopyEx(renderer, gameState->idle_anim[i], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	else if (gameState->player.status == 1) { // run
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_RUN, HEIGHT_PLAYER_RUN };
		// animation smoothness
		size_t animation_speed = SDL_GetTicks64() / 120;
		int i = animation_speed % 8;
		SDL_RenderCopyEx(renderer, gameState->run_anim[i], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	else if (gameState->player.status == 2) { // jump
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_JUMP, HEIGHT_PLAYER_JUMP };
		SDL_RenderCopyEx(renderer, gameState->jump_anim[0], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	else if (gameState->player.status == 3) { // mid air
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_JUMP, HEIGHT_PLAYER_JUMP };
		SDL_RenderCopyEx(renderer, gameState->jump_anim[1], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	else if (gameState->player.status == 4) { // falling
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_JUMP, HEIGHT_PLAYER_JUMP };
		size_t animation_speed = SDL_GetTicks64() / 120;
		int i = animation_speed % 2 + 2;
		SDL_RenderCopyEx(renderer, gameState->jump_anim[i], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	// Present the actual renderer
	SDL_RenderPresent(renderer);
}

void add_physics(GameState* gameState) {
	Object* player = &gameState->player;
	player->x += player->dx;
	player->y += player->dy;
	player->dy += GRAVITY;
	if (!player->onLedge) {
		if (player->dy < 0) {
			gameState->player.status = 2;
		}
		else if (player->dy == 0 ) {
			gameState->player.status = 3;
		}
		else {
			gameState->player.status = 4;
		}
	}
	// scrolling
	gameState->scrollX = -gameState->player.x + 320;
	if (gameState->scrollX > 0) {
		gameState->scrollX = 0;
	}
}

void collision_detect(GameState* gameState) {
	// prevent from falling out the window
	// check for collision with any ledges (brick blocks)
	for (int i = 0; i < NUM_OF_LEDGES; i++) {
		float pw = WIDTH_PLAYER_IDLE, ph = HEIGHT_PLAYER_IDLE;
		if (gameState->player.status == 0) {
			pw = WIDTH_PLAYER_IDLE, ph = HEIGHT_PLAYER_IDLE;
		}
		else if (gameState->player.status == 1) {
			pw = WIDTH_PLAYER_IDLE, ph = HEIGHT_PLAYER_IDLE;
		} 
		else if (gameState->player.status == 2 || gameState->player.status == 3 || gameState->player.status == 4 ) {
			pw = WIDTH_PLAYER_JUMP, ph = HEIGHT_PLAYER_JUMP;
		}
		else if (gameState->player.status == 5) {
			pw = WIDTH_PLAYER_LEDGEGRAB, ph = HEIGHT_PLAYER_LEDGEGRAB;
		}
		float px = gameState->player.x, py = gameState->player.y;
		float bw = gameState->ledges[i].w, bh = gameState->ledges[i].h, bx = gameState->ledges[i].x, by = gameState->ledges[i].y;

		if (py + ph / 2 > by && py < by + bh) {
			//rubbing against right edge 
			if (px< (bx + bw) && (px + pw)>(bx + bw) && gameState->player.dx < 0) {
				// correct x
				gameState->player.x = bx + bw;
				px = bx + bw;

				gameState->player.dx = 0;
			}
			else if ((px + pw) > bx && px < bx && gameState->player.dx > 0) {
				// correct x
				gameState->player.x = bx - pw;
				px = bx - pw;

				gameState->player.dx = 0;
			}
		}
		if (px + pw / 2 > bx && px + pw / 2 < bx + bw) {
			// are we bumping our head?
			if (py < (by + bh) && py > by && gameState->player.dy < 0) {
				// correct y
				gameState->player.y = by + bh;
				py = by + bh;

				// stop any jump velocity
				gameState->player.dy = 0;
				//	gameState->player.onLedge = 1;
			}
		}
		// landed on ledge?
		if (px + pw > bx && px < bx + bw) {
			if (py + ph > by && py < by && gameState->player.dy > 0) {
				// correct y
				gameState->player.y = by - ph;
				py = by - ph;

				// stop any jump velocity
				gameState->player.dy = 0;
				gameState->player.onLedge = 1;
			}
		}
	}
}