#ifndef structs
#define structs
typedef struct {
	float x, y;
	float dx, dy;
	short lives;
	char* name;
	int isTakenDamage;
	int onLedge; // 0 = false, 1 = true
	int flip, status; // 0 = idle, 1 = run, 2 = jump, 3 = mid air, 4 = falling
} Object;

typedef struct {
	float x, y, w, h;
	int isMoved; // 0 = false, 1 = true
	int isLethal; // 0 = false, 1 = true
	int isBlocked; // 0 = false, 1 = true
} Ledge;

typedef struct {
	float x, y;// x, y position to start drawing map
	int counter; // number of maps created actually
	int pos[MAP_HEIGHT][MAP_WIDTH]; // tilesmap
	Ledge ledges[MAP_HEIGHT][MAP_WIDTH];
} Map;

typedef struct {
	float defaultX, previousX, nextX;
	int scrollSpeed;	
	SDL_Texture* layer[3];
} Background; // use to make parallax effect

typedef struct {
	// Gamemode
	int mode;
	// Scroll offset
	float scrollX;
	// Players
	Object player;
	// Status State
	int statusState;
	// Ledges
	Ledge ledges[NUM_OF_LEDGES];
	// Images
		// Trap
			// Fire trap
	SDL_Texture* fire_trap[3];
			// Ceiling trap
	SDL_Texture* ceiling_trap[1];
			// Saw trap
	SDL_Texture* saw_trap[1];
		// Character animation
	SDL_Texture* idle_anim[12];
	SDL_Texture* jump_anim[4];
	SDL_Texture* run_anim[8];
	// Golem
	SDL_Texture* golem;
		// Platform: 
		// platform[5] is jungle tileset
	SDL_Texture* platform[6];
		// Background
	Background background[5];
	// Label
	SDL_Texture* label;
	// Fonts
	TTF_Font* font;
	// Renderer
	SDL_Renderer* renderer;
	// Map
	Map map[20];
} GameState;
#endif // !stucts