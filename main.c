#include <stdio.h>
#include <stdlib.h>
#include "SDL_1.h"

int main(int argc, char* argv[]) {
	GameState gameState;
	SDL_Window* window = NULL; // declare a window
	SDL_Renderer* renderer = NULL; // declare a renderer
	SDL_Init(SDL_INIT_VIDEO); // initialize SDL2
	// FPS
	Uint32 lastFrameTime = 0;
	float framerate = 0.0f;
	// Create an application window with following settings
	window = SDL_CreateWindow("Jungle Run",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH_WINDOW,
		HEIGHT_WINDOW,
		0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// Load game
	gameState.renderer = renderer;
	TTF_Init();
	load_game(&gameState);
	// Enter program loop (see SDL_PollEvent)
	int done = 0;
	while (!done) {
		Uint32 currentFrameTime = SDL_GetTicks64();
		Uint32 elapsedTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		done = processEvents(window, &gameState);

		// detect ground floor (platform[2])
		collision_detect_floor(&gameState);

		// detect map
		for (int i = 0; i < gameState.map->counter; i++) {
			collision_detect_map(&gameState, &gameState.map[i]);
		}

		add_physics(&gameState);

		do_render(&gameState);

		// Calculate and print framerate
		framerate = 1000.0f / elapsedTime;
		printf("Framerate: %.2f\n", framerate);
	}

	// Close and destroy the window
	SDL_DestroyTexture(gameState.idle_anim);
	SDL_DestroyTexture(gameState.run_anim);
	SDL_DestroyTexture(gameState.jump_anim);
	SDL_DestroyTexture(gameState.platform);
	SDL_DestroyTexture(gameState.background);
	SDL_DestroyTexture(gameState.golem);
	SDL_DestroyTexture(gameState.fire_trap);
	if (gameState.label != NULL) {
		SDL_DestroyTexture(gameState.label);
	}
	TTF_CloseFont(gameState.font);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();

	// Clean up
	SDL_Quit();

	return 0;
}