#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>

int main(int argc, char** argv)

{

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("window1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 1280, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	SDL_Surface* surface = IMG_Load("C:\\Users\\Admin\\source\\repos\\Jungle Run\\Resource\\Some Bullshit Platform\\Platform 1.png");

	SDL_Rect box = { 0, 0, 500, 500 };

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &box);
	for (int x = 0; x <= 500; x++) {
		for (int y = 0; y <= 500; y++) {
			if ((x - 250)*(x - 250) + (y - 250)*(y - 250) > 250 * 250) {
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}

	SDL_RenderPresent(renderer);

	SDL_Delay(12000);

	SDL_FreeSurface(surface);

	SDL_DestroyTexture(texture);

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

}
// I use the above code to achieve a circular image display, still using the rect function.If you want to try, just change the image path(IMG_ Load(" ")).