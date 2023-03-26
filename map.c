#include "map.h"

void loadMap(GameState* gameState, int arr[MAP_HEIGHT][MAP_WIDTH], float x) {
	Map* map = &gameState->map;
	map->x = x;
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			map->pos[row][col] = arr[row][col];
		}
	}
}

void drawMap(GameState* gameState) {
	Map* map = &gameState->map;
	SDL_Rect dstRect;
	int type = 0;
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			type = map->pos[row][col];
			switch (type) {
			case 1:
				dstRect = (SDL_Rect) { gameState->scrollX + map->x + col*WIDTH_PLATFORM_6, row*HEIGHT_PLATFORM_6, WIDTH_PLATFORM_6, HEIGHT_PLATFORM_6 };
				SDL_RenderCopy(gameState->renderer, gameState->platform[5], NULL, &dstRect);
				break;
			}
		}
	}
}