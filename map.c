#include "map.h"

void loadMap(GameState* gameState, int arr[MAP_HEIGHT][MAP_WIDTH], float x, float y) {
	Map* map = &gameState->map;
	map->x = x;
	map->y = y;
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			map->pos[row][col] = arr[row][col];
			map->ledges[row][col].x = map->x + col * WIDTH_PLATFORM_6;
			map->ledges[row][col].y = map->y + row * HEIGHT_PLATFORM_6;
			map->ledges[row][col].w = WIDTH_PLATFORM_6;
			map->ledges[row][col].h = HEIGHT_PLATFORM_6;
		}
	}
}

void drawMap(GameState* gameState) {
	Map map = gameState->map;
	SDL_Rect dstRect;
	int type = 0;
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			type = map.pos[row][col];
			switch (type) {
			case 1:
				dstRect = (SDL_Rect) { gameState->scrollX + map.ledges[row][col].x , map.ledges[row][col].y, map.ledges[row][col].w , map.ledges[row][col].h };
				SDL_RenderCopy(gameState->renderer, gameState->platform[5], NULL, &dstRect);
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