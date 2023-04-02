#include "map.h"

void loadMap(Map* map, int arr[MAP_HEIGHT][MAP_WIDTH], float x, float y) {
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			map->pos[row][col] = arr[row][col];
			map->ledges[row][col].x = x + col * WIDTH_PLATFORM_6;
			map->ledges[row][col].y = y + row * HEIGHT_PLATFORM_6;
			map->ledges[row][col].w = WIDTH_PLATFORM_6;
			map->ledges[row][col].h = HEIGHT_PLATFORM_6;
			map->ledges[row][col].isLethal = 0;
			map->ledges[row][col].isBlocked = 1;
			map->ledges[row][col].isMoved = 0;
			switch (map->pos[row][col]) {
			case 3:
				map->ledges[row][col].w = WIDTH_PLATFORM_6 * 3;
				break;
			case 4:
				map->ledges[row][col].isBlocked = 0;
				break;
			case 5:
				map->ledges[row][col].isBlocked = 0;
				break;
			case 6:
				map->ledges[row][col].y = y + (row - 1)* HEIGHT_PLATFORM_6;
				map->ledges[row][col].h = HEIGHT_PLATFORM_6 * 2;
				map->ledges[row][col].isBlocked = 0;
				break;
			case 7:
				map->ledges[row][col].isLethal = 1;
				map->ledges[row][col].isBlocked = 0;
				break;
			case 8:
			case 9:
			case 10:
				map->ledges[row][col].y = y + (row - 2) * HEIGHT_PLATFORM_6;
				map->ledges[row][col].h = HEIGHT_PLATFORM_6 * 3;
				map->ledges[row][col].isBlocked = 0;
				break;
			case 11:
			case 12:
				map->ledges[row][col].isBlocked = 0;
				break;
			}
		}
	}
}

void drawMap(GameState* gameState, Map* map) {
	SDL_Rect srcRect;
	int type = 0;
	int smoothness = 0;
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			type = map->pos[row][col];
			switch (type) {
			case 1:
				srcRect = (SDL_Rect) {63, 128, WIDTH_PLATFORM_6, HEIGHT_PLATFORM_6};
				drawTilesMap(gameState, *map, srcRect, gameState->platform[5], row, col, 0, 0);
				break;
			case 2:
				srcRect = (SDL_Rect){ 463, 144, WIDTH_PLATFORM_6, HEIGHT_PLATFORM_6 };
				drawTilesMap(gameState, *map, srcRect, gameState->platform[5], row, col, 0, 0);
				break;
			case 3:
				srcRect = (SDL_Rect){ 320, 144, 113, 49 };
				drawTilesMap(gameState, *map, srcRect, gameState->platform[5], row, col, 0, 0);
				break;
			case 4: // fire trap lv1
				smoothness = animation_smoothness(9, FIRETRAP_FRAMES_LV1);
				srcRect = (SDL_Rect){ 8 + smoothness * 32, 31, 16, 33 };
				if (smoothness == 4 || smoothness == 5) {
					map->ledges[row][col].isLethal = 1;
				}
				else {
					map->ledges[row][col].isLethal = 0;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->fire_trap[0], row, col, 0, 0);
				break;
			case 5: // fire trap lv2
				smoothness = animation_smoothness(9, FIRETRAP_FRAMES_LV2);
				srcRect = (SDL_Rect){ 3 + smoothness * 32, 25, 26, 40 };
				if (smoothness == 4 || smoothness == 5) {
					map->ledges[row][col].isLethal = 1;
				}
				else {
					map->ledges[row][col].isLethal = 0;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->fire_trap[1], row, col, 0, 0);
				break;
			case 6: // fire trap lv3
				smoothness = animation_smoothness(9, FIRETRAP_FRAMES_LV3);
				srcRect = (SDL_Rect){ 3 + smoothness * 32, 0, 26, 64 };
				if (smoothness == 4 || smoothness == 5 || smoothness == 6 || smoothness == 7 || smoothness == 8 || smoothness == 9) {
					map->ledges[row][col].isLethal = 1;
				}
				else {
					map->ledges[row][col].isLethal = 0;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->fire_trap[2], row, col, 0, 0);
				break;
			case 7: // ceiling trap lv1 but always lethal and stable
				srcRect = (SDL_Rect){ 16, 0, 33, 20 };
				drawTilesMap(gameState, *map, srcRect, gameState->ceiling_trap[0], row, col, 180, 0);
				break;
			case 8: // ceiling trap lv1 speed low
				smoothness = animation_smoothness(14, CEILINGTRAP_FRAMES_LV1_LOW);
				srcRect = (SDL_Rect){ 15 + smoothness * 64, 0, 34, 64 };
				if (smoothness == 1) {
					map->ledges[row][col].isLethal = 1;
				}
				else if (smoothness == 2 || smoothness == 3 || smoothness == 4 || smoothness == 5 || smoothness == 6 || smoothness == 7) {
					map->ledges[row][col].isLethal = 0;
					map->ledges[row][col].isBlocked = 1;
				}
				else {
					map->ledges[row][col].isLethal = 0;
					map->ledges[row][col].isBlocked = 0;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->ceiling_trap[0], row, col, 0, 0);
				break;
			case 9: // ceiling trap lv1 speed medium
				smoothness = animation_smoothness(14, CEILINGTRAP_FRAMES_LV1_MEDIUM);
				srcRect = (SDL_Rect){ 15 + smoothness * 64, 0, 34, 64 };
				if (smoothness == 1) {
					map->ledges[row][col].isLethal = 1;
				}
				else if (smoothness == 2 || smoothness == 3 || smoothness == 4 || smoothness == 5 || smoothness == 6 || smoothness == 7) {
					map->ledges[row][col].isLethal = 0;
					map->ledges[row][col].isBlocked = 1;
				}
				else {
					map->ledges[row][col].isLethal = 0;
					map->ledges[row][col].isBlocked = 0;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->ceiling_trap[0], row, col, 0, 0);
				break;
			case 10: // ceiling trap lv1 speed fast
				smoothness = animation_smoothness(14, CEILINGTRAP_FRAMES_LV1_FAST);
				srcRect = (SDL_Rect){ 15 + smoothness * 64, 0, 34, 64 };
				if (smoothness == 1) {
					map->ledges[row][col].isLethal = 1;
				}
				else if (smoothness == 2 || smoothness == 3 || smoothness == 4 || smoothness == 5 || smoothness == 6 || smoothness == 7) {
					map->ledges[row][col].isLethal = 0;
					map->ledges[row][col].isBlocked = 1;
				}
				else {
					map->ledges[row][col].isLethal = 0;
					map->ledges[row][col].isBlocked = 0;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->ceiling_trap[0], row, col, 0, 0);
				break;
			case 11: // saw trap lv1 in the floor
				smoothness = animation_smoothness(16, SAWTRAP_FRAMES_LV1);
				srcRect = (SDL_Rect){ 13 + smoothness * 64, 0, 39, 35 };
				if (smoothness == 0 || smoothness == 7 || smoothness == 14 || smoothness == 15) {
					map->ledges[row][col].isLethal = 0;
				}
				else {
					map->ledges[row][col].isLethal = 1;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->saw_trap[0], row, col, 180, 0);
				break;
			case 12: // saw trap lv1 in the ceiling
				smoothness = animation_smoothness(16, SAWTRAP_FRAMES_LV1);
				srcRect = (SDL_Rect){ 13 + smoothness * 64, 0, 39, 35 };
				if (smoothness == 0 || smoothness == 6 || smoothness == 7 || smoothness == 13 || smoothness == 14 || smoothness == 15) {
					map->ledges[row][col].isLethal = 0;
				}
				else {
					map->ledges[row][col].isLethal = 1;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->saw_trap[0], row, col, 0, 0);
				break;
			}
		}
	}
}

void drawLedges(GameState* gameState) {
	for (int i = 0; i < NUM_OF_LEDGES; i++) {
		SDL_Rect srcRect = { 217, 116, gameState->ledges[i].w, gameState->ledges[i].h };
		SDL_Rect ledgeRect = { gameState->scrollX + gameState->ledges[i].x, gameState->ledges[i].y , gameState->ledges[i].w, gameState->ledges[i].h };
		SDL_RenderCopyEx(gameState->renderer, gameState->platform[2], &srcRect, &ledgeRect, 0, NULL, 0);
	}
}

void drawTilesMap(GameState* gameState, Map map, SDL_Rect srcRect, SDL_Texture* texture, int row, int col, double angle, int flip) {
	SDL_Rect dstRect = { gameState->scrollX + map.ledges[row][col].x , map.ledges[row][col].y, map.ledges[row][col].w , map.ledges[row][col].h };
	SDL_RenderCopyEx(gameState->renderer, texture, &srcRect, &dstRect, angle, NULL, flip);
}

void drawBackground(GameState* gameState) {
	SDL_Renderer* renderer = gameState->renderer;
	SDL_RenderCopy(renderer, gameState->background[0].layer[0], NULL, NULL);
	for (int i = 1; i < 5; i++) {
		float camX = -gameState->scrollX;
		float dist = -camX / gameState->background[i].scrollSpeed;
		SDL_Rect dstRect_previous = { dist + gameState->background[i].previousX ,0,WIDTH_WINDOW,HEIGHT_WINDOW };
		SDL_Rect dstRect_default = { dist + gameState->background[i].defaultX ,0,WIDTH_WINDOW,HEIGHT_WINDOW };
		SDL_Rect dstRect_next = { dist + gameState->background[i].nextX ,0,WIDTH_WINDOW,HEIGHT_WINDOW };
		SDL_SetRenderTarget(renderer, gameState->background[i].layer[0]);
		SDL_RenderCopy(renderer, gameState->background[i].layer[1], NULL, &dstRect_default);

		SDL_RenderCopy(renderer, gameState->background[i].layer[0], NULL, &dstRect_previous);

		SDL_RenderCopy(renderer, gameState->background[i].layer[1], NULL, &dstRect_default);

		SDL_SetRenderTarget(renderer, gameState->background[i].layer[2]);
		SDL_RenderCopy(renderer, gameState->background[i].layer[1], NULL, &dstRect_default);

		SDL_RenderCopy(renderer, gameState->background[i].layer[2], NULL, &dstRect_next);
		float temp = camX - dist;
		if (temp > gameState->background[i].defaultX + 2*WIDTH_WINDOW) {
			gameState->background[i].defaultX += WIDTH_WINDOW;
			gameState->background[i].previousX = gameState->background[i].defaultX - WIDTH_WINDOW;
			gameState->background[i].nextX = gameState->background[i].defaultX + WIDTH_WINDOW;
		}
		else if (temp < gameState->background[i].defaultX - WIDTH_WINDOW) {
			gameState->background[i].defaultX -= WIDTH_WINDOW;
			gameState->background[i].previousX = gameState->background[i].defaultX - WIDTH_WINDOW;
			gameState->background[i].nextX = gameState->background[i].defaultX + WIDTH_WINDOW;
		}
	}
}

void drawMap_SingleplayerMode(GameState* gameState) {
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
		else if (i == 7) { //lv2
			additionalDis = WIDTH_WINDOW;
			loadMap(&map[map->counter++], lv2, gameState->ledges[i - 1].x + WIDTH_PLATFORM_3, 0);
		}
		else if (i == 8) { //lv3
			additionalDis = WIDTH_WINDOW;
			loadMap(&map[map->counter++], lv3, gameState->ledges[i - 1].x + WIDTH_PLATFORM_3, 0);
		}
		else if (i == 9) { //lv4
			additionalDis = WIDTH_WINDOW;
			loadMap(&map[map->counter++], lv4, gameState->ledges[i - 1].x + WIDTH_PLATFORM_3, 0);
		}
		gameState->ledges[i].x = gameState->ledges[i - 1].x + WIDTH_PLATFORM_3 + additionalDis;
		gameState->ledges[i].y = 800 - HEIGHT_PLATFORM_3;
		gameState->ledges[i].w = WIDTH_PLATFORM_3;
		gameState->ledges[i].h = HEIGHT_PLATFORM_3;
		gameState->ledges[i].isLethal = 0;
		gameState->ledges[i].isBlocked = 1;
	}
}