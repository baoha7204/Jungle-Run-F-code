#include "common_functions.h"
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

void do_render(GameState* gameState) {
	SDL_Renderer* renderer = gameState->renderer;
	SDL_RenderClear(renderer);
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
	gameState->scrollX = -gameState->player.x + 350;
	if (gameState->scrollX > 0) {
		gameState->scrollX = 0;
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
	// SDL_Delay(5);
	// Reset the color modulation to white
	SDL_SetTextureColorMod(texture, 255, 255, 255);
	SDL_RenderCopyEx(gameState->renderer, texture, NULL, &dstRect, 0, NULL, gameState->player.flip);
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