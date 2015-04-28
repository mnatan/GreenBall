#ifndef DEFINITIONS
#define DEFINITIONS

#include <SDL/SDL_image.h>
#include <vector>

#define UNUSED(a) ((void)(a))

#define FULSCREEN   false
#define WIDTH       860
#define HEIGHT      640

#define TEX_WALL    0
#define TEX_FLOOR   1
#define TEX_STONE   2
#define TEX_PLAYER  3
#define TEX_BACKG   4
#define TEX_GEM     5
#define TEX_SCORE   6
#define TEX_WIN     7
#define TEX_FAIL    8
#define TEX_LVLCMP  9
#define TEX_DOOR    10
#define TEX_KUCYK   11
#define TEX_SWITCH  12
#define TEX_BOX     13

#define MAP_NONE    -1
#define MAP_WALL    0
#define MAP_FLOOR   1
#define MAP_STONE   2
#define MAP_PLAYER  3
#define MAP_BLINKER 4
#define MAP_DOOR    5
#define MAP_SWITCH  6
#define MAP_BOX     7

#define ANIM_PLAYER_TIME    0.1f
#define ANIM_TIME_FALL 		1.0f
#define BLINK_FADE_TIME     0.5f
#define WIN_WAIT_TIME       2.0f
#define ANIM_BACKG_TIME     5.0f
#define ANIM_BACKG_SPEED    0.1f
#define ANIM_DOOR_CLOSE 	0.5f

class MapChunk;
class Map;
//class Player;
//class Vector3D;

//class game_obj;
//class animated;
//class scaled;
//class rotated;
//class moved;

//class Gem;
//class Box;
//class Blinker;
//class Switch;
//class Door;
//class Wall;
//class Floor;
//class EmptySpace;

//struct player_st;
//struct gem;
//struct blinker;
//struct door;
//struct switcher;
//struct box;

//struct anim_move_pl;
//struct anim_fall_pl;

// Globals

bool win = false;
bool fail = false;
bool game_complete = false;
int score = 0;

float win_countdown;
unsigned int level = 0;

static int screen_width;
static int screen_height;
bool keys[SDLK_LAST];

float ratio;
float current_time;
float backdir = 1.0f;
float background_start_time;

unsigned int texturki[20];
//std::vector<rotated> kamienie;
//std::vector<Blinker> blinkery;
//std::vector<Door> drzwi;
//std::vector<Switch> guziki;
//std::vector<Box> pudelka;

#endif
