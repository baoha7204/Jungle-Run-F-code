#include "map.h"

void loadMap(Map* map, int arr[MAP_HEIGHT][MAP_WIDTH], float x, float y) {
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			map->pos[row][col] = arr[row][col];
			map->ledges[row][col].x = x + +col * WIDTH_PLATFORM_6;
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
			case 4: // fire trap
				smoothness = animation_smoothness(9, FIRETRAP_FRAMES);
				srcRect = (SDL_Rect){ 8 + smoothness * 32, 31, 16, 33 };
				if (smoothness == 4 || smoothness == 5) {
					map->ledges[row][col].isLethal = 1;
				}
				else {
					map->ledges[row][col].isLethal = 0;
				}
				drawTilesMap(gameState, *map, srcRect, gameState->fire_trap[0], row, col);
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
