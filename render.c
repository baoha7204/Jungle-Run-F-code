#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "map.h"

void do_render(GameState* gameState) {
	SDL_Renderer* renderer = gameState->renderer;
	SDL_RenderClear(renderer);
	if (gameState->statusState == STATUS_STATE_GAMEOVER) {
		// draw background
		drawBackground(gameState);
		Over(gameState);
	}
	else if (gameState->statusState == STATUS_STATE_GAME) {
		// draw background
		drawBackground(gameState);
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
		// draw giga golem
		render_giga_golem(gameState, gameState->golem);
		// Present the actual renderer
		SDL_RenderPresent(renderer);
	}
}

void render_character_animation(GameState* gameState, SDL_Texture* texture[], const int width, const int height, int frame) {
	if (gameState->player.isTakenDamage) { 
		onTrapHit(gameState, texture[frame], gameState->scrollX + gameState->player.x, gameState->player.y);
	}
	else {
		SDL_Rect dstRect = { gameState->scrollX + gameState->player.x, gameState->player.y, width * 2, height * 2 };
		SDL_RenderCopyEx(gameState->renderer, texture[frame], NULL, &dstRect, 0, NULL, gameState->player.flip);
	}
}

void render_giga_golem(GameState* gameState, SDL_Texture* texture[]) {
	int smoothness = animation_smoothness(10, GIGA_GOLEM_FRAMES);
	SDL_Rect srcRect = { 24 + smoothness * 100, 624, 50, 50 };
	Golem boss = gameState->gigaGolem;
	SDL_Rect dstRect = { gameState->scrollX + boss.pos.x, boss.pos.y, WIDTH_GIGA_GOLEM, HEIGHT_GIGA_GOLEM };
	SDL_RenderCopyEx(gameState->renderer, texture[0], &srcRect, &dstRect, 0, NULL, 0);
}