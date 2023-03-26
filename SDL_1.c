#include "SDL_1.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "map.h"

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
			case SDLK_SPACE:
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
	srand(time(NULL));
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
		sprintf_s(str, 128, "Resource\\Character\\Idle\\Idle_%d.png", i);
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
		sprintf_s(str, 128, "Resource\\Character\\Run\\Run_%d.png", i);
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
		sprintf_s(str, 128, "Resource\\Character\\Jump\\Jump_%d.png", i);
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
		sprintf_s(str, 128, "Resource\\Character\\Ledge Grab\\Ledge Grab_%d.png", i);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->ledgeGrab_anim[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
	// Create Golem
	surface = IMG_Load("Resource\\Mecha-stone Golem 0.1\\PNG sheet\\Character_sheet.png");
	if (surface == NULL) {
		SDL_Quit();
		exit(1);
	}
	gameState->golem = SDL_CreateTextureFromSurface(gameState->renderer, surface);
	SDL_FreeSurface(surface);
	// Create background
	for (int i = 0; i < 5; i++) {
		char str[128] = "";
		sprintf_s(str, 128, "Resource\\Jungle Asset Pack\\parallax background\\plx-%d.png", i + 1);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->background[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
	// Create platform
	for (int i = 0; i < 6; i++) {
		char str[128] = "";
		sprintf_s(str, 128, "Resource\\Some Bullshit Platform\\Platform %d.png", i + 1);
		surface = IMG_Load(str); // declare a surface = main computer memory
		if (surface == NULL) {
			SDL_Quit();
			exit(1);
		}
		gameState->platform[i] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
		SDL_FreeSurface(surface);
	}
	// Load fonts
	gameState->font = TTF_OpenFont("Resource\\Fonts\\crazy-pixel.ttf", 48);
	if (!gameState->font) {
		SDL_Quit();
		exit(1);
	}
	// init ledges
	int temp = rand() % 5 + 3;
	gameState->ledges[0].w = WIDTH_PLATFORM_3;
	gameState->ledges[0].h = HEIGHT_PLATFORM_3;
	gameState->ledges[0].x = 0;
	gameState->ledges[0].y = 800 - HEIGHT_PLATFORM_3;
	for (int i = 1; i < NUM_OF_LEDGES; i++) {
		if (i % temp == 0) {
			// load Map
			loadMap(gameState, lv1, gameState->ledges[i - 1].x);
			/*int additionalDis = 0;
			do {
				additionalDis = rand() % 350;
			} while (additionalDis < 200);*/
			int additionalDis = WIDTH_WINDOW;
			gameState->ledges[i].x = gameState->ledges[i - 1].x + WIDTH_PLATFORM_3 + additionalDis;
		}
		else {
			gameState->ledges[i].x = gameState->ledges[i - 1].x + WIDTH_PLATFORM_3;
		}
		gameState->ledges[i].w = WIDTH_PLATFORM_3;
		gameState->ledges[i].h = HEIGHT_PLATFORM_3;
		gameState->ledges[i].y = 800 - HEIGHT_PLATFORM_3;
	}
}

void do_render(GameState* gameState) {
	SDL_Renderer* renderer = gameState->renderer;
	// Create background
	for (int i = 0; i < 5; i++) {
		SDL_RenderCopy(renderer, gameState->background[i], NULL, NULL);
	}
	// draw platform[1]
	/*SDL_Rect srcRect[3];
	srcRect[0] = (SDL_Rect){ 307,246,108,28 };
	srcRect[1] = (SDL_Rect){ 414, 175, 54, 99 };
	srcRect[2] = (SDL_Rect){ 467, 246, 68, 28 };*/
	// draw ledges
	for (int i = 0; i < NUM_OF_LEDGES; i++) {
		SDL_Rect srcRect = { 217, 116, gameState->ledges[i].w, gameState->ledges[i].h };
		SDL_Rect ledgeRect = { gameState->scrollX + gameState->ledges[i].x, gameState->ledges[i].y , gameState->ledges[i].w, gameState->ledges[i].h };
		SDL_RenderCopyEx(renderer, gameState->platform[2], &srcRect, &ledgeRect, 0, NULL, 0);
	}
	// Draw map
	drawMap(gameState);
	if (gameState->player.status == 0) { // idle 
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_IDLE*2, HEIGHT_PLAYER_IDLE*2};
		// animation smoothness
		size_t animation_speed = SDL_GetTicks64() / 120;
		int i = animation_speed % 12;
		SDL_RenderCopyEx(renderer, gameState->idle_anim[i], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	else if (gameState->player.status == 1) { // run
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_RUN * 2, HEIGHT_PLAYER_RUN * 2 };
		// animation smoothness
		size_t animation_speed = SDL_GetTicks64() / 120;
		int i = animation_speed % 8;
		SDL_RenderCopyEx(renderer, gameState->run_anim[i], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	else if (gameState->player.status == 2) { // jump
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_JUMP*2, HEIGHT_PLAYER_JUMP*2 };
		SDL_RenderCopyEx(renderer, gameState->jump_anim[0], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	else if (gameState->player.status == 3) { // mid air
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_JUMP*2, HEIGHT_PLAYER_JUMP*2 };
		SDL_RenderCopyEx(renderer, gameState->jump_anim[1], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
	else if (gameState->player.status == 4) { // falling
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, WIDTH_PLAYER_JUMP*2, HEIGHT_PLAYER_JUMP*2 };
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
		float pw = WIDTH_PLAYER_IDLE*2, ph = HEIGHT_PLAYER_IDLE*2;
		if (gameState->player.status == 0) {
			pw = WIDTH_PLAYER_IDLE*2, ph = HEIGHT_PLAYER_IDLE*2;
		}
		else if (gameState->player.status == 1) {
			pw = WIDTH_PLAYER_IDLE*2, ph = HEIGHT_PLAYER_IDLE*2;
		} 
		else if (gameState->player.status == 2 || gameState->player.status == 3 || gameState->player.status == 4 ) {
			pw = WIDTH_PLAYER_JUMP*2, ph = HEIGHT_PLAYER_JUMP*2;
		}
		else if (gameState->player.status == 5) {
			pw = WIDTH_PLAYER_LEDGEGRAB*2, ph = HEIGHT_PLAYER_LEDGEGRAB*2;
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