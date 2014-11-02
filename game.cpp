//
// First testing game by Marcin Natanek
// created using Mini Game Template Gynvael Coldwind
//
// win32: g++ game.cpp -Wall -pedantic -lSDL -lopengl32 -lglu32 -lSDL_image
// linux: g++ game.cpp -Wall -pedantic -lSDL -lGL -lGLU -lSDL_image
//

#include <vector>
#include <sstream>
#include <iostream>

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
int score = 0;

static int screen_width;
static int screen_height;
bool keys[SDLK_LAST];

float ratio;
float f_time;
float backdir = 1.0f;
float background_start_time;

unsigned int texturki[10];
int map_width;
int map_height;
int **map;
std::vector<gem> kamienie;

SDL_Color redFont = {255, 0, 0, 0};
SDL_Color greenFont = {0, 255, 0, 0};
SDL_Color blueFont = {0, 0, 255, 0};
SDL_Surface *scoresurf;
TTF_Font *fontKomoda;

struct player_st
{
	int x, y;
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

struct anim_move_pl
{
	bool active;

	//intput
	float x_s, y_s; //start pos
	float x_e, y_e; //end pos
	float t_s, t_e; //start/end time

	//output
	float x, y; //w czasie t
} anim_player_move;
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

	return true;
}

static void Logic()
{
	int p_dx = 0;
	int p_dy = 0;

	if (!anim_player_move.active)
	{
		if (map[map_player.x][map_player.y] == MAP_NONE)
		{
			if (!fail)
			{
				fail = true;

				anim_player_fall.active = true;
				anim_player_fall.t_s = f_time;
				anim_player_fall.t_e = f_time + ANIM_PLAYER_TIME_FALL;
				anim_player_fall.z_s = 0;
				anim_player_fall.z_e = -21.0f;
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
			anim_player_move.active = true;
			anim_player_move.t_s = f_time;
			anim_player_move.t_e = f_time + ANIM_PLAYER_TIME;
			anim_player_move.x_s = (float)map_player.x;
			anim_player_move.y_s = (float)map_player.y;
			anim_player_move.x_e = (float)p_nx;
			anim_player_move.y_e = (float)p_ny;

			map_player.x = p_nx;
			map_player.y = p_ny;

			std::vector<gem>::iterator gem = getGemByXY( map_player.x, map_player.y);
			if ( gem != kamienie.end())
			{
				kamienie.erase(gem);
				score += 1;
				if (kamienie.empty())
					win = true;
			}
		}
	}
	if (anim_player_move.active)
	{
		anim_player_move.active = AnimUpdate(&anim_player_move);
	}
	if (anim_player_fall.active)
	{
		anim_player_fall.active = AnimUpdate2(&anim_player_fall);
	}
}

static void Scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef( 0.0, 0.0, -17.0f);
	glScalef(1.0f, -1.0f, 1.0f);

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
	if (anim_player_move.active)
		glTranslatef(
		    -(float)anim_player_move.x + 7.5f,
		    -(float)anim_player_move.y + 7.5f,
		    -0.0f);
	else
		glTranslatef(
		    -(float)map_player.x + 7.5f,
		    -(float)map_player.y + 7.5f,
		    -0.0f);

	// Draw map
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			if (map[i][j] != MAP_NONE)
			{
				if (map[i][j] == MAP_WALL)
				{
					DrawCubeTexture(
					    (float)i - 7.5f, (float)j - 7.5f, 0.5f,
					    1.0f,
					    texturki[TEX_WALL]
					);
					DrawCubeTexture(
					    (float)i - 7.5f, (float)j - 7.5f, -0.5f,
					    1.0f,
					    texturki[TEX_FLOOR]
					);
				}
				else if (map[i][j] == MAP_FLOOR)
				{
					DrawCubeTexture(
					    (float)i - 7.5f, (float)j - 7.5f, -0.5f,
					    1.0f,
					    texturki[TEX_FLOOR]
					);
				}
				else
					DrawQuadTexture(
					    (float)i - 7.5f, (float)j - 7.5f, 0.0f,
					    1.0f, 1.0f,
					    texturki[map[i][j]]
					);
			}
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	size_t s = kamienie.size();
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
	if (anim_player_move.active)
		DrawQuadTexture(
		    (float)anim_player_move.x - 7.5f, (float)anim_player_move.y - 7.5f, 0.5f,
		    1.0f, 1.0f,
		    texturki[TEX_PLAYER]
		);
	else if (fail)
		DrawQuadTexture(
		    (float)map_player.x - 7.5f, (float)map_player.y - 7.5f, anim_player_fall.z,
		    1.0f, 1.0f,
		    texturki[TEX_PLAYER]
		);
	else
		DrawQuadTexture(
		    (float)map_player.x - 7.5f, (float)map_player.y - 7.5f, 0.5f,
		    1.0f, 1.0f,
		    texturki[TEX_PLAYER]
		);

	if (win)
	{
		DrawQuadTexture(
		    0.0f, 0.0f, 1.5f,
		    7.0f, 2.0f,
		    texturki[TEX_WIN]
		);
	}
	if (fail)
	{
		DrawQuadTexture(
		    0.0f, 0.0f, 1.5f,
		    7.0f, 2.0f,
		    texturki[TEX_FAIL]
		);
	}

	glPopMatrix();

	std::string scores = "Score: ";
	std::stringstream ss;
	ss << scores << score;
	std::string result = ss.str();
	scoresurf = TTF_RenderText_Solid( fontKomoda, result.c_str(), blueFont );
	texturki[TEX_SCORE] = SurfaceToTexture(scoresurf, TEX_SCORE);
	DrawQuadRGBA(
	    10.0f, -7.5f, 1.1f,
	    3.5f, 1.5f,
	    78, 158, 116, 0.6
	);
	DrawQuadTexture(
	    10.0f, -7.5f, 1.2f,
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

	fontKomoda = TTF_OpenFont( "font/Komoda.ttf" , 35 );

	// Main loop:
	unsigned int last_time = SDL_GetTicks();
	ratio = 0.0f;

	f_time = (float)last_time / 1000.0f;

	//ImgToTexture( "background.jpg" );
	LoadMap("maps/map1.txt");

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
	std::cout << "width: " << map_width << std::endl;
	std::cout << "height: " << map_height << std::endl;
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
			default:
				fprintf(stderr, "error: unexpected char \"%c\" in %d %d: \"%s\"\n", line[j], i, j, filename);
			}
		}
	}
	fprintf(stdout, "info: loaded map \"%s\"\n", filename);
	return true;
}
static bool AnimUpdate(anim_move_pl *a)
{
	if (!a->active)
		return false;

	if (f_time >= a->t_e)
		return false;

	float d = (f_time  - a->t_s) / (a->t_e - a->t_s);
	float dx = a->x_e - a->x_s;
	float dy = a->y_e - a->y_s;

	a->x = a->x_s + d * dx;
	a->y = a->y_s + d * dy;

	return true;
}
static bool AnimUpdate2(anim_fall_pl *a)
{
	if (!a->active)
		return false;

	if (f_time >= a->t_e)
		return false;

	float d = (f_time - a->t_s) / (a->t_e - a->t_s);
	float dz = a->z_e - a->z_s;

	a->z = a->z_s + d * dz;

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
