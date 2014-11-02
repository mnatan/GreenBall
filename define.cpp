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

#define MAP_NONE    -1
#define MAP_WALL    0
#define MAP_FLOOR   1
#define MAP_STONE   2
#define MAP_PLAYER  3
#define MAP_BACKG   4

#define ANIM_PLAYER_TIME    0.1f
#define ANIM_PLAYER_TIME_FALL 1.0f
#define WIN_WAIT_TIME 		5.0f
#define ANIM_BACKG_TIME     5.0f
#define ANIM_BACKG_SPEED    0.1f

struct gem;
struct player_st;

struct anim_move_pl;
struct anim_fall_pl;
