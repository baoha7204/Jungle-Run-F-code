#include "collision_handling.h"

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