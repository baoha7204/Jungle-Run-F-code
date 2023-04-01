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
				drawTilesMap(gameState, *map, srcRect, gameState->platform[5], row, col);
				break;
			case 2:
				srcRect = (SDL_Rect){ 463, 144, WIDTH_PLATFORM_6, HEIGHT_PLATFORM_6 };
				drawTilesMap(gameState, *map, srcRect, gameState->platform[5], row, col);
				break;
			case 3:
				srcRect = (SDL_Rect){ 320, 144, 113, 49 };
				drawTilesMap(gameState, *map, srcRect, gameState->platform[5], row, col);
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
				drawTilesMap(gameState, *map, srcRect, gameState->fire_trap[0], row, col);
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
				drawTilesMap(gameState, *map, srcRect, gameState->fire_trap[1], row, col);
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
				drawTilesMap(gameState, *map, srcRect, gameState->fire_trap[2], row, col);
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

void drawTilesMap(GameState* gameState, Map map, SDL_Rect srcRect, SDL_Texture* texture, int row, int col) {
	SDL_Rect dstRect = { gameState->scrollX + map.ledges[row][col].x , map.ledges[row][col].y, map.ledges[row][col].w , map.ledges[row][col].h };
	SDL_RenderCopy(gameState->renderer, texture, &srcRect, &dstRect);
}

void drawBackground(GameState* gameState) {
	SDL_Renderer* renderer = gameState->renderer;
	SDL_RenderCopy(renderer, gameState->background[0].layer[0], NULL, NULL);
	for (int i = 1; i < 5; i++) {
		float camX = -gameState->scrollX;
		float dist = camX / gameState->background[i].scrollSpeed;
		float temp = camX - dist;
		gameState->background[i].defaultX = dist;
		if (temp > dist + WIDTH_WINDOW) {
			gameState->background[i].defaultX -= WIDTH_WINDOW;
		}
		else if (temp < dist - WIDTH_WINDOW) {
			gameState->background[i].defaultX += WIDTH_WINDOW;
		}
		gameState->background[i].previousX = gameState->background[i].defaultX - WIDTH_WINDOW;
		gameState->background[i].nextX = gameState->background[i].defaultX + WIDTH_WINDOW;
		SDL_Rect dstRect_previous = { gameState->background[i].previousX ,0,WIDTH_WINDOW,HEIGHT_WINDOW };
		SDL_Rect dstRect_default = { gameState->background[i].defaultX ,0,WIDTH_WINDOW,HEIGHT_WINDOW };
		SDL_Rect dstRect_next = { gameState->background[i].nextX ,0,WIDTH_WINDOW,HEIGHT_WINDOW };

		SDL_SetRenderTarget(renderer, gameState->background[i].layer[0]);	
		SDL_RenderCopy(renderer, gameState->background[i].layer[1], NULL, &dstRect_default);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, gameState->background[i].layer[0], NULL, &dstRect_previous);

		SDL_RenderCopy(renderer, gameState->background[i].layer[1], NULL, &dstRect_default);

		SDL_SetRenderTarget(renderer, gameState->background[i].layer[2]);
		SDL_RenderCopy(renderer, gameState->background[i].layer[1], NULL, &dstRect_default);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, gameState->background[i].layer[2], NULL, &dstRect_next);
	}
}