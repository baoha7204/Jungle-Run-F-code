#include <stdio.h>
#include <stdlib.h>
#include "SDL_1.h"

int main(int argc, char* argv[]) {
	GameState gameState;
	SDL_Window* window = NULL; // declare a window
	SDL_Renderer* renderer = NULL; // declare a renderer
	SDL_Init(SDL_INIT_VIDEO); // initialize SDL2
	// Create an application window with following settings
	window = SDL_CreateWindow("Test",
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
		done = processEvents(window, &gameState);

		collision_detect(&gameState);

		add_physics(&gameState);

		do_render(&gameState);

		//	SDL_Delay(10);
	}

	// Close and destroy the window
	SDL_DestroyTexture(gameState.idle_anim);
	SDL_DestroyTexture(gameState.run_anim);
	SDL_DestroyTexture(gameState.jump_anim);
	SDL_DestroyTexture(gameState.ledgeGrab_anim);
	SDL_DestroyTexture(gameState.platform);
	SDL_DestroyTexture(gameState.background);
	SDL_DestroyTexture(gameState.golem);
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