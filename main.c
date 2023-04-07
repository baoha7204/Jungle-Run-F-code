#include <stdio.h>
#include <stdlib.h>
#include "common_functions.h"

int main(int argc, char* argv[]) {
	GameState gameState;
	SDL_Window* window = NULL; // declare a window
	SDL_Renderer* renderer = NULL; // declare a renderer
	SDL_Init(SDL_INIT_EVERYTHING); // initialize SDL2
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
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
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
		gameState.dt = elapsedTime / 1000.0f;
	//	printf("Framerate: %.2f\n", framerate);
	}

	// Close and destroy the window
	clean(&gameState, window);
	return 0;
}