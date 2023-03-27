#ifndef structs
#define structs
typedef struct {
	float x, y;
	float dx, dy;
	short lives;
	char* name;
	int onLedge; // 0 = false, 1 = true
	int flip, status; // 0 = idle, 1 = run, 2 = jump, 3 = mid air, 4 = falling, 5 = ledge grab
} Object;

typedef struct {
	float x, y, w, h;
} Ledge;

typedef struct {
	float x, y;// x, y position to start drawing map
	int counter; // number of maps created actually
	int pos[MAP_HEIGHT][MAP_WIDTH]; // tilesmap
	Ledge ledges[MAP_HEIGHT][MAP_WIDTH];
} Map;

typedef struct {
	// Scroll offset
	float scrollX;
	// Players
	Object player;
	// Status State
	int statusState;
	// Ledges
	Ledge ledges[NUM_OF_LEDGES];
	// Images
		// Character animation
	SDL_Texture* idle_anim[12];
	SDL_Texture* jump_anim[4];
	SDL_Texture* ledgeGrab_anim[6];
	SDL_Texture* run_anim[8];
	// Golem
	SDL_Texture* golem;
	// Platform
	SDL_Texture* platform[6];
	// Background
	SDL_Texture* background[5];
	// Label
	SDL_Texture* label;
	// Fonts
	TTF_Font* font;
	// Renderer
	SDL_Renderer* renderer;
	// Map
	Map map_1[100];
} GameState;
#endif // !stucts