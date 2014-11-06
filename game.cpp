/*
 * First testing game by Marcin Natanek
 * created using Mini Game Template Gynvael Coldwind
 *
 * win32: g++ game.cpp -Wall -pedantic -lSDL -lopengl32 -lglu32 -lSDL_image
 * linux: g++ game.cpp -Wall -pedantic -lSDL -lGL -lGLU -lSDL_image
 */

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <GL/glut.h>

#include <SDL/SDL_image.h>
#include "SDL/SDL_ttf.h"

#ifdef _WIN32
#  include <windows.h>
#endif

// Remove SDL main hook on Windows
#ifdef _WIN32
#  undef main
#endif

#include "define.cpp"
#include "game.h"

//using namespace std;

// Globals.
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
float f_time;
float backdir = 1.0f;
float background_start_time;

unsigned int texturki[20];
int map_width;
int map_height;
int **map;
std::vector<gem> kamienie;
std::vector<blinker> blinkery;
std::vector<door> drzwi;
std::vector<switcher> guziki;
std::vector<box> pudelka;

SDL_Color redFont = {255, 0, 0, 0};
SDL_Color greenFont = {0, 255, 0, 0};
SDL_Color blueFont = {0, 0, 255, 0};
SDL_Surface *scoresurf;
TTF_Font *fontKomoda;

struct player_st
{
	float x, y, z;

	bool active;

	//do animacji
	float x_s, y_s, z_s; //start pos
	float x_e, y_e, z_e; //end pos
	float t_s, t_e; //start/end time
} map_player;
struct gem
{
	int x, y;

	//lolwut fabryka?
	static struct gem mk(int x, int y)
	{
		gem g = {x, y};
		return g;
	}
};
struct blinker
{
	int x, y, z;
	float time_dead;
	float time_alive;

	float next_switch;
	int state; // 1-jest, 2-zmiana, 3-nie ma, 4-zmiana
	float anim_start;
	float anim_end;

	float curr_d;

	//lolwut fabryka?
	static struct blinker mk(int x, int y, int z, float time_dead, float time_alive)
	{
		blinker g = {x, y, z, time_dead, time_alive};
		g.next_switch = f_time + time_alive;
		g.state = 1;
		g.curr_d = 1;
		return g;
	}
};
struct door
{
	int x, y, z;

	bool closed;

	static struct door mk(int x, int y, int z)
	{
		door g = {x, y, z};
		g.closed = true;
		return g;
	}
};
struct switcher
{
	int x, y, z;

	bool on;

	static struct switcher mk(int x, int y, int z)
	{
		switcher g = {x, y, z};
		g.on = false;
		return g;
	}
};
struct box
{
	int x, y, z;

	//lolwut fabryka?
	static struct box mk(int x, int y, int z)
	{
		box g = {x, y, z};
		return g;
	}
};

struct anim_fall_pl
{
	bool active;

	//intput
	float t_s, t_e; //start/end time
	float z_s;
	float z_e;

	//output
	float z; //w czasie t
} anim_player_fall;

// Functions

static bool Events()
{
	SDL_Event ev;
	memset(&ev, 0, sizeof(ev));

	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_KEYUP:
			keys[ev.key.keysym.sym] = false;
			break;

		case SDL_KEYDOWN:
			if (ev.key.keysym.sym == SDLK_ESCAPE)
				return false;
			keys[ev.key.keysym.sym] = true;
			break;

		case SDL_QUIT:
			return false;
		}
	}

	if (win && win_countdown < f_time)
	{
		win = false;
		if ( !LoadNextLevel() )
		{
			game_complete = true;
		}
	}

	return true;
}

static void Logic()
{
	int p_dx = 0;
	int p_dy = 0;

	if (!map_player.active)
	{
		if (map[(int)map_player.x][(int)map_player.y] == MAP_NONE)
		{
			if (!fail)
			{
				fail = true;

				map_player.active = true;
				map_player.t_s = f_time;
				map_player.t_e = f_time + ANIM_PLAYER_TIME_FALL;
				map_player.x_s = map_player.x;
				map_player.x_e = map_player.x;
				map_player.y_s = map_player.y;
				map_player.y_e = map_player.y;
				map_player.z_s = 0;
				map_player.z_e = -21.0f;
			}
		}
		if (!fail)
		{
			if (keys[SDLK_UP])
			{
				p_dy = -1;
				keys[SDLK_UP] = false;
			}
			else if (keys[SDLK_DOWN])
			{
				p_dy = 1;
				keys[SDLK_DOWN] = false;
			}
			else if (keys[SDLK_LEFT])
			{
				p_dx = -1;
				keys[SDLK_LEFT] = false;
			}
			else if (keys[SDLK_RIGHT])
			{
				p_dx = 1;
				keys[SDLK_RIGHT] = false;
			}
		}
	}

	if (p_dx || p_dy)
	{

		int p_nx = map_player.x + p_dx;
		int p_ny = map_player.y + p_dy;

		bool moveok = true;

		if ( map[p_nx][p_ny] == MAP_WALL)
		{
			moveok = false;
		}

		if (moveok)
		{
			map_player.active = true;
			map_player.t_s = f_time;
			map_player.t_e = f_time + ANIM_PLAYER_TIME;
			map_player.x_s = (float)map_player.x;
			map_player.y_s = (float)map_player.y;
			map_player.x_e = (float)p_nx;
			map_player.y_e = (float)p_ny;
			map_player.z_e = map_player.z;
			map_player.z_s = map_player.z;

			map_player.x = p_nx;
			map_player.y = p_ny;

			std::vector<gem>::iterator gem = getGemByXY( map_player.x, map_player.y);
			if ( gem != kamienie.end())
			{
				kamienie.erase(gem);
				score += 1;
				if (kamienie.empty())
				{
					win = true;
					win_countdown = f_time + WIN_WAIT_TIME;
				}
			}
		}
	}
	if (map_player.active)
	{
		map_player.active = Move<player_st>(&map_player);
	}
}

static void Scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef( 0.0, 0.0, -14.0f);
	glScalef(1.0f, -1.0f, 1.0f);

	glPushMatrix();
	gluPerspective( 0.0, (float)screen_width / (float)screen_height, 0.0, 1024.0 );
	static float rotat;
	glTranslatef(9.5f, 7.0f, 0.0f);
	glRotatef(rotat += 50.0f * ratio, 0.5f, 1.0f, 0.3f);
	glTranslatef(-9.5f, -7.0f, 0.0f);
	DrawCubeTexture(
		9.5f, 7.0f, 0.0f,
		0.9f,
		texturki[TEX_KUCYK]
	);
	glPopMatrix();

	// animacja tła - czasowo lewo prawo
	if (f_time - background_start_time > ANIM_BACKG_TIME)
	{
		if (backdir == 1.0f)
			backdir = -1.0f;
		else
			backdir = 1.0f;
		background_start_time = f_time;
	}
	glPushMatrix();
	static float trans;
	glTranslatef(trans += ANIM_BACKG_SPEED * ratio * backdir, 0.0f, 0.0f);
	float skalar = 16.5f;
	DrawQuadTexture(
	    0.0f, 0.0f, -20.1f,
	    4.0f * skalar, 3.0f * skalar,
	    texturki[TEX_BACKG] );
	glPopMatrix();

	glPushMatrix();
	glTranslatef(
	    -map_player.x + 7.5f,
	    -map_player.y + 7.5f,
	    -0.0f);

	// Draw map
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			//if (
			//map[i][j] != MAP_NONE &&
			//map[i][j] != MAP_BLINKER
			//)
			switch (map[i][j])
			{
			case MAP_WALL:
				for(int k = 0; k<2; k++){
				DrawCubeTexture(
				    (float)i - 7.5f, (float)j - 7.5f, 0.5f+(float)k,
				    1.0f,
				    texturki[TEX_WALL]
				);
				}
				DrawCubeTexture(
				    (float)i - 7.5f, (float)j - 7.5f, -0.5f,
				    1.0f,
				    texturki[TEX_FLOOR]
				);
				break;
			case MAP_FLOOR:
				DrawCubeTexture(
				    (float)i - 7.5f, (float)j - 7.5f, -0.5f,
				    1.0f,
				    texturki[TEX_FLOOR]
				);
				break;
			case MAP_DOOR:
				break;
			case MAP_SWITCH:
				break;
			case MAP_BOX:
				break;
			case MAP_BLINKER:
				break;
			case MAP_NONE:
				break;
			default:
				DrawQuadTexture(
				    (float)i - 7.5f, (float)j - 7.5f, 0.0f,
				    1.0f, 1.0f,
				    texturki[map[i][j]]
				);
				break;
			}
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	size_t s = blinkery.size();
	for (size_t i = 0 ; i < s; i++)
	{
		UpdateBlinker(i);
		DrawCubeTexture(
		    (float)blinkery[i].x - 7.5f, (float)blinkery[i].y - 7.5f, (float)blinkery[i].z - 0.5f,
		    blinkery[i].curr_d,
		    texturki[TEX_FLOOR]
		);
	}
	s = drzwi.size();
	for (size_t i = 0 ; i < s; i++)
	{
		DrawCubeTexture(
		    (float)drzwi[i].x - 7.5f, (float)drzwi[i].y - 7.5f, (float)drzwi[i].z + 0.5f,
		    1.0f,
		    texturki[TEX_DOOR]
		);
	}
	s = guziki.size();
	for (size_t i = 0 ; i < s; i++)
	{
		DrawQuadTexture(
		    (float)guziki[i].x - 7.5f, (float)guziki[i].y - 7.5f, (float)guziki[i].z + 0.1f,
		    1.0f, 1.0f,
		    texturki[TEX_SWITCH]
		);
	}
	s = pudelka.size();
	for (size_t i = 0 ; i < s; i++)
	{
		DrawCubeTexture(
		    (float)pudelka[i].x - 7.5f, (float)pudelka[i].y - 7.5f, (float)pudelka[i].z + 0.5f,
		    1.0f,
		    texturki[TEX_BOX]
		);
	}

	s = kamienie.size();
	for (size_t i = 0 ; i < s; i++)
	{
		glPushMatrix();
		static float rot;
		glTranslatef((float)kamienie[i].x - 7.5f, (float)kamienie[i].y - 7.5f, 0.5f);
		glRotatef(rot += 10.0f * ratio, 0.0f, 1.0f, 0.0f);
		glTranslatef(-(float)kamienie[i].x + 7.5f, -(float)kamienie[i].y + 7.5f, -0.5f);
		//glTranslatef((float)kamienie[i].x, (float)kamienie[i].y, 0.0f);
		DrawQuadTexture(
		    (float)kamienie[i].x - 7.5f, (float)kamienie[i].y - 7.5f, 0.5f,
		    1.0f, 1.0f,
		    texturki[TEX_GEM]
		);
		glPopMatrix();
	}

	// draw player
	DrawQuadTexture(
	    map_player.x - 7.5f, map_player.y - 7.5f, map_player.z,
	    0.8f, 0.8f,
	    texturki[TEX_PLAYER]
	);

	if (win)
	{
		DrawQuadTexture(
		    (float)map_player.x - 7.5f, (float)map_player.y - 7.5f, 1.5f,
		    7.0f, 2.0f,
		    texturki[TEX_LVLCMP]
		);
	}
	if (fail)
	{
		DrawQuadTexture(
		    (float)map_player.x - 7.5f, (float)map_player.y - 7.5f, 1.6f,
		    7.0f, 2.0f,
		    texturki[TEX_FAIL]
		);
	}
	if (game_complete)
	{
		DrawQuadTexture(
		    (float)map_player.x - 7.5f, (float)map_player.y - 7.5f, 1.7f,
		    7.0f, 2.0f,
		    texturki[TEX_WIN]
		);
	}

	glPopMatrix();

	std::string scores = "Score: ";
	std::stringstream ss;
	ss << scores << score;
	//std::string scores = "(";
	//std::stringstream ss;
	//ss << scores << map_player.x << "," << map_player.y << "," << map_player.z << ")";
	std::string result = ss.str();
	scoresurf = TTF_RenderText_Solid( fontKomoda, result.c_str(), blueFont );
	texturki[TEX_SCORE] = SurfaceToTexture(scoresurf, TEX_SCORE);
	DrawQuadRGBA(
	    7.0f, -6.0f, 2.1f,
	    3.5f, 1.5f,
	    78, 158, 116, 0.6
	);
	DrawQuadTexture(
	    7.0f, -6.0f, 2.2f,
	    3.0f, 1.0f,
	    texturki[TEX_SCORE]
	);

	glDisable(GL_BLEND);

	SDL_GL_SwapBuffers();
}

int main(int argc, char **argv, char **envp)
{
	// Unused.
	UNUSED(argc);
	UNUSED(argv);
	UNUSED(envp);

	srand(time(NULL));

	// Init SDL.
	if (!InitSDL(FULSCREEN, WIDTH, HEIGHT))
		return 1;

	// Init OpenGL.
	if (!InitOpenGL())
		return 2;

	if ( TTF_Init() == -1 )
		return 1;

	// Title
	SDL_WM_SetCaption("Pierwsza gra w OpenGL", "My game");

	// Loading Textures
	texturki[TEX_WALL] = ImgToTexture("gfx/wall.png");
	texturki[TEX_FLOOR] = ImgToTexture("gfx/floor.png");
	texturki[TEX_STONE] = ImgToTexture("gfx/ball.png");
	texturki[TEX_PLAYER] = ImgToTexture("gfx/player.png");
	texturki[TEX_BACKG] = ImgToTexture("gfx/background.jpg");
	texturki[TEX_GEM] = ImgToTexture("gfx/gem.png");
	//kombinowanie
	texturki[TEX_SCORE] = ImgToTexture("gfx/gem.png");

	// z jakiegoś powodu musi być po score !! FIXME
	texturki[TEX_WIN] = ImgToTexture("gfx/win.png");
	texturki[TEX_FAIL] = ImgToTexture("gfx/fail.png");
	texturki[TEX_LVLCMP] = ImgToTexture("gfx/lvlcmp.png");
	texturki[TEX_DOOR] = ImgToTexture("gfx/door.png");
	texturki[TEX_KUCYK] = ImgToTexture("gfx/kucyk.png");
	texturki[TEX_SWITCH] = ImgToTexture("gfx/switch.png");
	texturki[TEX_BOX] = ImgToTexture("gfx/box.png");

	fontKomoda = TTF_OpenFont( "font/Komoda.ttf" , 35 );

	// Main loop:
	unsigned int last_time = SDL_GetTicks();
	ratio = 0.0f;

	f_time = (float)last_time / 1000.0f;

	//ImgToTexture( "background.jpg" );
	//LoadMap("maps/map1.txt");
	LoadNextLevel();

	for (;;)
	{

		// Main stuff.
		if (!Events())
			break;

		Logic();
		Scene();

		// Calc time.
		unsigned int curr_time = SDL_GetTicks();
		ratio = (float)(curr_time - last_time) / 1000.0f;
		last_time = curr_time;
		f_time = (float)curr_time / 1000.0f;
	}

	// Done.
	SDL_Quit();
	return 0;
}



static bool InitSDL(bool fullscreen, int width, int height)
{
	int screen_bpp;
	int screen_flag = SDL_OPENGL;
	if (fullscreen) screen_flag |= SDL_FULLSCREEN;

	screen_width = width;
	screen_height = height;

	const SDL_VideoInfo *info;
	SDL_Surface *surface;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;

	info = SDL_GetVideoInfo();
	if (info == NULL)
		goto err;

	screen_bpp = info->vfmt->BitsPerPixel;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_putenv(const_cast<char *>("SDL_VIDEO_CENTERED=center"));

	surface = SDL_SetVideoMode(
	              screen_width, screen_height,
	              screen_bpp, screen_flag
	          );

	if (surface == NULL)
		goto err;

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	return true;

	err:
	SDL_Quit();
	return false;
}
static bool InitOpenGL()
{
	float ratio;
	ratio = (float)screen_width / (float)screen_height;

	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity( );
	gluPerspective( 60.0, ratio, 1.0, 1024.0 );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glDisable(GL_DEPTH_TEST); // uncomment this if going 2D
	glEnable(GL_DEPTH_TEST);

	//  A pointer to a font style..
	//  Fonts supported by GLUT are: GLUT_BITMAP_8_BY_13,
	//  GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10,
	//  GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10,
	//  GLUT_BITMAP_HELVETICA_12, and GLUT_BITMAP_HELVETICA_18.
	//font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	return true;
}

unsigned int ImgToTexture(const char *filename)
{
	// Load image.
	SDL_Surface *img = IMG_Load(filename);
	if (!img)
	{
		fprintf(stderr, "error: ImgToTexture could not load image \"%s\"\n",
		        filename);
		return ~0;
	}

	// Image type? Only 24 and 32 supported, rest must be converted.
	unsigned int img_type = 0;

	if (img->format->BitsPerPixel == 32)
	{
		img_type = GL_RGBA;
	}
	else if (img->format->BitsPerPixel == 24)
	{
		img_type = GL_RGB;
	}
	else
	{

		// Convert to 32 bits.
		SDL_PixelFormat fmt =
		{
			NULL, 32, 4,
			0, 0, 0, 0,
			0, 8, 16, 24,
			0xff, 0xff00, 0xff0000, 0xff000000,
			0,
			0xff
		};

		SDL_Surface *nimg = SDL_ConvertSurface(img, &fmt, SDL_SWSURFACE);
		SDL_FreeSurface(img);

		if (!nimg)
		{
			fprintf(stderr, "error: ImgToTexture could not convert image \"%s\" to 32-bit\n",
			        filename);
			return ~0;
		}

		// Done converting.
		img = nimg;
		img_type = GL_RGBA;
	}

	// Create texture.
	unsigned int texture_id = ~0;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// You might want to play with these parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Gen texture.
	glTexImage2D(GL_TEXTURE_2D, 0, img_type, img->w, img->h,
	             0, img_type, GL_UNSIGNED_BYTE, img->pixels);

	printf("info: ImgToTexture loaded texture \"%s\" as %u\n",
	       filename, texture_id);

	// Done
	return texture_id;
}
unsigned int SurfaceToTexture(SDL_Surface *img, unsigned int texture_id)
{
	// Image type? Only 24 and 32 supported, rest must be converted.
	unsigned int img_type = 0;

	if (img->format->BitsPerPixel == 32)
	{
		img_type = GL_RGBA;
	}
	else if (img->format->BitsPerPixel == 24)
	{
		img_type = GL_RGB;
	}
	else
	{

		// Convert to 32 bits.
		SDL_PixelFormat fmt =
		{
			NULL, 32, 4,
			0, 0, 0, 0,
			0, 8, 16, 24,
			0xff, 0xff00, 0xff0000, 0xff000000,
			0,
			0xff
		};

		SDL_Surface *nimg = SDL_ConvertSurface(img, &fmt, SDL_SWSURFACE);
		SDL_FreeSurface(img);

		if (!nimg)
		{
			fprintf(stderr, "error: ImgToTexture could not convert surface to 32-bit\n");
			return ~0;
		}

		// Done converting.
		img = nimg;
		img_type = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, texture_id + 1);

	// You might want to play with these parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Gen texture.
	glTexImage2D(GL_TEXTURE_2D, 0, img_type, img->w, img->h,
	             0, img_type, GL_UNSIGNED_BYTE, img->pixels);

	//printf("info: SurfaceToTexture loaded texture as %u\n", texture_id+1);

	// Done
	return texture_id + 1;
}

void DrawCube(float x, float y, float z, float a)
{
	a /= 2.0f;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + a, y + -a, z + a);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + -a, y + -a, z + a);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + -a, y + a, z + a);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + a, y + a, z + -a);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + a, y + -a, z + -a);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + -a, y + -a, z + -a);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + -a, y + a, z + -a);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + a, y + -a, z + a);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + a, y + -a, z + -a);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + a, y + a, z + -a);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + a, y + -a, z + a);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + a, y + -a, z + -a);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + -a, y + -a, z + -a);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + -a, y + -a, z + a);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + -a, y + a, z + a);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + -a, y + -a, z + a);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + -a, y + -a, z + -a);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + -a, y + a, z + -a);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + a, y + a, z + -a);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + -a, y + a, z + -a);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + -a, y + a, z + a);
	glEnd();
}
void DrawCubeTexture(float x, float y, float z, float a, unsigned int texture_id)
{
	// Enable texturing if needed.
	bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
	if (!texturing_enabled)
		glEnable(GL_TEXTURE_2D);

	// Bind texture and draw.
	glBindTexture(GL_TEXTURE_2D, texture_id);
	DrawCube(x, y, z, a);

	// Disable if was disabled.
	if (!texturing_enabled)
		glDisable(GL_TEXTURE_2D);
}

void DrawQuad(float x, float y, float z, float w, float h)
{
	w /= 2.0f;
	h /= 2.0f;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x - w, y - h, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + w, y - h, z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + w, y + h, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x - w, y + h, z);
	glEnd();
}
void DrawQuadRGBA(float x, float y, float z, float w, float h, float r, float g, float b, float a)
{
	// Get old color.
	float current_color[4];
	glGetFloatv(GL_CURRENT_COLOR, current_color);

	// Set new color and draw quad.
	glColor4f(r, g, b, a);
	DrawQuad(x, y, z, w, h);

	// Set old color.
	glColor4fv(current_color);
}
void DrawQuadTexture(float x, float y, float z, float w, float h, unsigned int texture_id)
{
	// Enable texturing if needed.
	bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
	if (!texturing_enabled)
		glEnable(GL_TEXTURE_2D);

	// Bind texture and draw.
	glBindTexture(GL_TEXTURE_2D, texture_id);
	DrawQuad(x, y, z, w, h);

	// Disable if was disabled.
	if (!texturing_enabled)
		glDisable(GL_TEXTURE_2D);
}

bool LoadNextLevel()
{
	level++;
	char filename[256] = {0};
	snprintf(filename, sizeof(filename) - 1, "maps/map%u.txt", level);
	blinkery.clear();
	drzwi.clear();
	guziki.clear();
	pudelka.clear();
	return LoadMap(filename);
}
bool LoadMap(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if (!f)
	{
		fprintf(stderr, "error: could not open file: \"%s\"\n", filename);
		return false;
	}
	fscanf(f, "%d", &map_height);
	fscanf(f, "%d", &map_width);
	char line[64];
	fgets(line, sizeof(line), f);

	map = new int* [map_height];
	for (int i = 0; i < map_height ; i++)
		map[i] = new int[map_width];

	for (int i = 0; i < map_width; i++)
	{
		for (int j = 0; j < map_height; j++)
		{
			map[i][j] = -1;
		}
	}
	for (int i = 0; i < map_width; i++)
	{
		fgets(line, sizeof(line), f);
		if (feof(f))
		{
			fprintf(stderr, "error: unexpected eof in: \"%s\"\n", filename);
			fclose(f);
			return false;
		}
		for (int j = 0; j < map_height; j++)
		{
			switch ( line[j] )
			{
			case ' ':
				map[j][i] = MAP_NONE;
				break;
			case 'p':
				map_player.x = j;
				map_player.y = i;
				map_player.z = 0.5;
				map[j][i] = MAP_FLOOR;
				break;
			case '#':
				map[j][i] = MAP_WALL;
				break;
			case 'g':
				map[j][i] = MAP_FLOOR;
				kamienie.push_back( gem::mk(j, i) );
				break;
			case '.':
				map[j][i] = MAP_FLOOR;
				break;
			case 'o':
				map[j][i] = MAP_STONE;
				break;
			case 'b':
				map[j][i] = MAP_BLINKER;
				blinkery.push_back(blinker::mk(j, i, 0,
				                               (float)((rand() % 2 ) + 1),
				                               (float)((rand() % 2 ) + 1)
				                              ));
				break;
			case 'd':
				map[j][i] = MAP_FLOOR;
				drzwi.push_back(door::mk(j, i, 0));
				break;
			case 'x':
				map[j][i] = MAP_FLOOR;
				pudelka.push_back(box::mk(j, i, 0));
				break;
			case 's':
				map[j][i] = MAP_FLOOR;
				guziki.push_back(switcher::mk(j, i, 0));
				break;
			default:
				fprintf(stderr, "error: unexpected char \"%c\" in %d %d: \"%s\"\n", line[j], i, j, filename);
			}
		}
	}
	fprintf(stdout, "info: loaded map \"%s\"\n", filename);
	return true;
}

template <class T>
	static bool Move(T *a)
{
	if (f_time >= a->t_e)
	{
		a->x = a->x_e;
		a->y = a->y_e;
		a->z = a->z_e;
		return false;
	}

	float d = (f_time  - a->t_s) / (a->t_e - a->t_s);
	float dx = a->x_e - a->x_s;
	float dy = a->y_e - a->y_s;
	float dz = a->z_e - a->z_s;

	a->x = a->x_s + d * dx;
	a->y = a->y_s + d * dy;
	a->z = a->z_s + d * dz;

	return true;
}
static bool UpdateBlinker(size_t id)
{
	if (f_time > blinkery[id].next_switch)
	{
		if (blinkery[id].state == 1)
		{
			blinkery[id].state = 2;
			blinkery[id].next_switch =  f_time + BLINK_FADE_TIME + blinkery[id].time_alive;
			blinkery[id].anim_end =     f_time + BLINK_FADE_TIME;
			blinkery[id].anim_start =   f_time;
		}
		else if (blinkery[id].state == 3)
		{
			blinkery[id].state = 4;
			blinkery[id].next_switch =  f_time + BLINK_FADE_TIME + blinkery[id].time_alive;
			blinkery[id].anim_end =     f_time + BLINK_FADE_TIME;
			blinkery[id].anim_start =   f_time;
			map[blinkery[id].x][blinkery[id].y] = MAP_BLINKER;
		}
	}
	else if (blinkery[id].state == 2)
	{
		if (blinkery[id].anim_end < f_time)
		{
			blinkery[id].state = 3;
			blinkery[id].curr_d = 0;
			map[blinkery[id].x][blinkery[id].y] = MAP_NONE;
		}
		else
		{
			float delta = (f_time - blinkery[id].anim_start) / (blinkery[id].anim_end - blinkery[id].anim_start);
			blinkery[id].curr_d = 1 - delta;
		}
	}
	else if (blinkery[id].state == 4)
	{
		if (blinkery[id].anim_end < f_time)
		{
			blinkery[id].state = 1;
			blinkery[id].curr_d = 1;
		}
		else
		{
			float delta = (f_time - blinkery[id].anim_start) / (blinkery[id].anim_end - blinkery[id].anim_start);
			blinkery[id].curr_d = delta;
		}
	}
	return true;
}
std::vector<gem>::iterator getGemByXY(int x, int y)
{
	std::vector<gem>::iterator s = kamienie.end();
	for ( std::vector<gem>::iterator i = kamienie.begin() ; i < s; i++)
	{
		if ( i->x == x &&
		        i->y == y)
			return i;
	}
	return s;
}
