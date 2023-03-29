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
	gameState->mode = GAMEMODE_SINGLEPLAYER; // will be handled later, which is selected by user
	gameState->label = NULL;
	gameState->player.x = 220;
	gameState->player.y = 220;
	gameState->player.dx = 0;
	gameState->player.dy = 0;
	gameState->player.flip = 0;
	gameState->player.status = 0;
	gameState->player.onLedge = 0;
	gameState->statusState = STATUS_STATE_LIVES;
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
	load_texture(gameState, gameState->background, "Resource\\Jungle Asset Pack\\parallax background\\plx-%d.png", 5);
	// Create platform
	load_texture(gameState, gameState->platform, "Resource\\Some Bullshit Platform\\Platform %d.png", 5);
		// Platform[5]
	SDL_Surface* surface = IMG_Load("Resource\\Jungle Asset Pack\\jungle tileset\\jungle tileset.png");
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

void do_render(GameState* gameState) {
	SDL_Renderer* renderer = gameState->renderer;
	// Create background
	for (int i = 0; i < 5; i++) {
		SDL_RenderCopy(renderer, gameState->background[i], NULL, NULL);
	}
	// draw Map
	for (int i = 0; i < gameState->map->counter; i++) {
		// draw map
		drawMap(gameState, &gameState->map[i]);
	}
	// draw ledges
	drawLedges(gameState);
	if (gameState->player.status == 0) { // idle 
		int i = animation_smoothness(12, CHARACTER_FRAMES);
		render_character_animation(gameState, gameState->idle_anim, WIDTH_PLAYER_IDLE, HEIGHT_PLAYER_IDLE, i);
	}
	else if (gameState->player.status == 1) { // run
		int i = animation_smoothness(8, CHARACTER_FRAMES);
		render_character_animation(gameState, gameState->run_anim, WIDTH_PLAYER_RUN, HEIGHT_PLAYER_RUN, i);
	}
	else if (gameState->player.status == 2) { // jump
		render_character_animation(gameState, gameState->jump_anim, WIDTH_PLAYER_JUMP, HEIGHT_PLAYER_JUMP, 0);
	}
	else if (gameState->player.status == 3) { // mid air
		render_character_animation(gameState, gameState->jump_anim, WIDTH_PLAYER_JUMP, HEIGHT_PLAYER_JUMP, 1);
	}
	else if (gameState->player.status == 4) { // falling
		int i = animation_smoothness(2, CHARACTER_FRAMES) + 2;
		render_character_animation(gameState, gameState->jump_anim, WIDTH_PLAYER_JUMP, HEIGHT_PLAYER_JUMP, i);
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

void collision_detect_floor(GameState* gameState) {
	// prevent from falling out the window
	// check for collision with any ledges (brick blocks)
	for (int i = 0; i < NUM_OF_LEDGES; i++) {
		float px, py, pw, ph;
		load_player_info(gameState, &px, &py, &pw, &ph);
		float bw = gameState->ledges[i].w, bh = gameState->ledges[i].h, bx = gameState->ledges[i].x, by = gameState->ledges[i].y;
		collision_correction(gameState, px, py, pw, ph, bx, by, bw, bh);
	}
}

void collision_detect_map(GameState* gameState, Map* map) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (map->pos[i][j] != 0) {
				// init info
				float px, py, pw, ph;
				load_player_info(gameState, &px, &py, &pw, &ph);
				float bw = map->ledges[i][j].w, bh = map->ledges[i][j].h, bx = map->ledges[i][j].x, by = map->ledges[i][j].y;
				switch (map->pos[i][j]) {
				case 3:
					bw = map->ledges[i][j].w * 3;
					break;
				}
				// detect collision
				SDL_Rect rectA = { px,py,pw,ph };
				SDL_Rect rectB = { bx,by,bw,bh };
				if (SDL_HasIntersection(&rectA, &rectB)) {
					if (!map->ledges[i][j].isBlocked) {
						if (map->ledges[i][j].isLethal) {
							gameState->player.isTakenDamage = 1;
						}
					}
					else {
						gameState->player.isTakenDamage = 0;
						collision_correction(gameState, px, py, pw, ph, bx, by, bw, bh);
					}
				}
			}
		}
	}
}

void collision_correction(GameState* gameState, float px, float py, float pw, float ph, float bx, float by, float bw, float bh) {
	if (py + ph / 2 > by && py < by + bh) {
		//rubbing against right edge 
		if (px< (bx + bw) && (px + pw)>(bx + bw) && gameState->player.dx < 0) {
			// correct x
			gameState->player.x = bx + bw;

			gameState->player.dx = 0;
		}
		else if ((px + pw) > bx && px < bx && gameState->player.dx > 0) {
			// correct x
			gameState->player.x = bx - pw;

			gameState->player.dx = 0;
		}
	}
	if (px + pw / 2 > bx && px + pw / 2 < bx + bw) {
		// are we bumping our head?
		if (py < (by + bh) && py > by && gameState->player.dy < 0) {
			// correct y
			gameState->player.y = by + bh;

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

			// stop any jump velocity
			gameState->player.dy = 0;
			gameState->player.onLedge = 1;
		}
	}
}

void load_player_info(GameState* gameState, float* px, float* py, float* pw, float* ph) {
	*pw = WIDTH_PLAYER_IDLE * 2, *ph = HEIGHT_PLAYER_IDLE * 2;
	if (gameState->player.status == 0) {
		*pw = WIDTH_PLAYER_IDLE * 2, *ph = HEIGHT_PLAYER_IDLE * 2;
	}
	else if (gameState->player.status == 1) {
		*pw = WIDTH_PLAYER_IDLE * 2, *ph = HEIGHT_PLAYER_IDLE * 2;
	}
	else if (gameState->player.status == 2 || gameState->player.status == 3 || gameState->player.status == 4) {
		*pw = WIDTH_PLAYER_JUMP * 2, *ph = HEIGHT_PLAYER_JUMP * 2;
	}
	else if (gameState->player.status == 5) {
		*pw = WIDTH_PLAYER_LEDGEGRAB * 2, *ph = HEIGHT_PLAYER_LEDGEGRAB * 2;
	}
	*px = gameState->player.x, *py = gameState->player.y;
}

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

int animation_smoothness(int frame, const int frames) {
	size_t animation_speed = SDL_GetTicks64() / frames;
	int i = animation_speed % frame;
	return i;
}

void onTrapHit(GameState* gameState, SDL_Texture* texture, int x, int y) {
	// Set the color modulation to red
	SDL_SetTextureColorMod(texture, 255, 0, 0);
	// Render the texture to the renderer at the specified location
	SDL_Rect dstRect = { x, y, 0, 0 };
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dstRect.w *= 2;
	dstRect.h *= 2;
	SDL_RenderCopyEx(gameState->renderer, texture, NULL, &dstRect, 0, NULL, gameState->player.flip);
	SDL_RenderPresent(gameState->renderer);
	SDL_Delay(5);
	// Reset the color modulation to white
	SDL_SetTextureColorMod(texture, 255, 255, 255);
	SDL_RenderCopyEx(gameState->renderer, texture, NULL, &dstRect, 0, NULL, gameState->player.flip);
}

void render_character_animation(GameState* gameState, SDL_Texture* texture[], const int width, const int height, int frame) {
	if (gameState->player.isTakenDamage) {
		onTrapHit(gameState->renderer, texture[frame], gameState->scrollX + gameState->player.x, gameState->player.y);
	}
	else {
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, width * 2, height * 2 };
		SDL_RenderCopyEx(gameState->renderer, texture[frame], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
}