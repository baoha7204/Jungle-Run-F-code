typedef struct {
	float x, y;
	float dx, dy;
	short lives;
	char* name;
	int onLedge; // 0 = false, 1 = true
	int flip, status; // 0 = idle, 1 = run, 2 = jump, 3 = mid air, 4 = falling, 5 = ledge grab
} Object;

typedef struct {
	int x, y, w, h;
} Ledge;

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
	SDL_Texture* platform[5];
	// Background
	SDL_Texture* background[5];
	// Label
	SDL_Texture* label;
	// Fonts
	TTF_Font* font;
	// Renderer
	SDL_Renderer* renderer;
} GameState;