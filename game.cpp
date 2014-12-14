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

#include "define.h"

//#include "class/game_obj.cpp"
#include "class/game_obj.cpp"
#include "class/MapChunk.cpp"
#include "class/blinker.cpp"
#include "class/Player.cpp"
#include "class/Box.cpp"
#include "class/Door.cpp"
#include "class/Switch.cpp"
#include "class/Gem.cpp"

#include "class/EmptySpace.cpp"
#include "class/Floor.cpp"
#include "class/Wall.cpp"

#include "game.h"

//using namespace std;

// Globals.

SDL_Color redFont = {255, 0, 0, 0};
SDL_Color greenFont = {0, 255, 0, 0};
SDL_Color blueFont = {0, 0, 255, 0};
SDL_Surface *scoresurf;
TTF_Font *fontKomoda;

Player map_player;
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

	if (win && win_countdown < current_time)
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
	Vector3D zmiana = none;

	if (!map_player.animating)
	{
		if (MapRead(map_player.pos + down).canFall())
		{
			fail = true;
			map_player.setAnimation(
			    map_player.pos,
			    Vector3D(
			        map_player.pos.x,
			        map_player.pos.y,
			        -21.0f
			    ),
			    ANIM_TIME_FALL
			);
		}
		if (!fail)
		{
			if (keys[SDLK_UP])
			{
				zmiana = top;
				keys[SDLK_UP] = false;
			}
			else if (keys[SDLK_DOWN])
			{
				zmiana = bot;
				keys[SDLK_DOWN] = false;
			}
			else if (keys[SDLK_LEFT])
			{
				zmiana = left;
				keys[SDLK_LEFT] = false;
			}
			else if (keys[SDLK_RIGHT])
			{
				zmiana = right;
				keys[SDLK_RIGHT] = false;
			}
		}
	}

	if (zmiana != zero)
	{
		Vector3D nowaPozycja = map_player.pos + zmiana;

		if (MapRead(nowaPozycja).canEnter(zmiana))
		{
			MapRead(nowaPozycja).playerEnters(zmiana);
			map_player.setAnimation(map_player.pos,
			                        nowaPozycja,
			                        ANIM_PLAYER_TIME
			                       );
		}
	}
	if (map_player.animating)
	{
		map_player.animating = map_player.UpdateAnimation();
	}
}

static void Scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef( 0.0, 0.0, -14.0f);
	glScalef(1.0f, -1.0f, 1.0f);


	// animacja tła - czasowo lewo prawo
	if (current_time - background_start_time > ANIM_BACKG_TIME)
	{
		if (backdir == 1.0f)
			backdir = -1.0f;
		else
			backdir = 1.0f;
		background_start_time = current_time;
	}
	glPushMatrix();
	static float trans;
	glTranslatef(trans += ANIM_BACKG_SPEED * ratio * backdir, 0.0f, 0.0f);
	float skalar = 16.5f;
	DrawQuadTexture(
	    Vector3D(0.0f, 0.0f, -20.1f),
	    4.0f * skalar, 3.0f * skalar,
	    TEX_BACKG
	);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(
	    -map_player.pos.x,
	    -map_player.pos.y,
	    -0.0f);

	// Draw map
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			for (int k = 0; k < map_layers; k++)
			{
				MapRead(Vector3D(i, j, k)).drawIt(); // lol so short
			}
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//size_t s = blinkery.size();
	//for (size_t i = 0 ; i < s; i++)
	//{
	//blinkery[i].UpdateAnimation();
	//DrawCubeTexture(
	//blinkery[i].pos - Vector3D(7.5f, 7.5f, 0.5f),
	//blinkery[i].scale,
	//texturki[TEX_FLOOR]
	//);
	//}
	//s = drzwi.size();
	//for (size_t i = 0 ; i < s; i++)
	//{
	//if (drzwi[i].animating)
	//{
	//drzwi[i].UpdateAnimation();
	//}
	/*
	 *DrawCubeTexture(
	 *    (float)drzwi[i].pos.x - 7.5f, (float)drzwi[i].pos.y - 7.5f, (float)drzwi[i].pos.z + 0.5f,
	 *    0.95f,
	 *    texturki[drzwi[i].TEX]
	 *);
	 */
	//}
	//s = guziki.size();
	//for (size_t i = 0 ; i < s; i++)
	//{
	/*
	 *std::vector<Box>::iterator box = getBoxByVector(guziki[i].pos);
	 *if ( box != pudelka.end())
	 *{
	 *    if (!guziki[i].animating)
	 *    {
	 *        //guziki[i].active = true;
	 *        guziki[i].ActivateTarget();
	 *        std::cout << "aktywuje" << std::endl;
	 *    }
	 *}
	 *else
	 *{
	 *    if (guziki[i].animating)
	 *    {
	 *        //guziki[i].active = false;
	 *        guziki[i].DeactivateTarget();
	 *        std::cout << "deaktywuje" << std::endl;
	 *    }
	 *}
	 */
	//DrawQuadTexture(
	//(float)guziki[i].pos.x - 7.5f, (float)guziki[i].pos.y - 7.5f, (float)guziki[i].pos.z + 0.1f,
	//1.0f, 1.0f,
	//texturki[guziki[i].TEX]
	//);
	//}
	//s = pudelka.size();
	//for (size_t i = 0 ; i < s; i++)
	//{
	//if (pudelka[i].animating)
	//{
	//pudelka[i].animating = pudelka[i].UpdateAnimation();
	//}
	//pudelka[i].checkFloor();
	/*
	 *DrawCubeTexture(
	 *    (float)pudelka[i].pos.x - 7.5f, (float)pudelka[i].pos.y - 7.5f, (float)pudelka[i].pos.z + 0.5f,
	 *    1.0f,
	 *    texturki[TEX_BOX]
	 *);
	 */
	//}

	//s = kamienie.size();
	//for (size_t i = 0 ; i < s; i++)
	//{
	//kamienie[i].UpdateAnimation();
	//glPushMatrix();
	//glTranslatef((float)kamienie[i].pos.x - 7.5f, (float)kamienie[i].pos.y - 7.5f, 0.5f);
	//glRotatef(
	//kamienie[i].accumulator,
	//kamienie[i].rotationVector.x,
	//kamienie[i].rotationVector.y,
	//kamienie[i].rotationVector.z
	//);
	//glTranslatef(-(float)kamienie[i].pos.x + 7.5f, -(float)kamienie[i].pos.y + 7.5f, -0.5f);
	////glTranslatef((float)kamienie[i].x, (float)kamienie[i].y, 0.0f);
	//DrawQuadTexture(
	//(float)kamienie[i].pos.x - 7.5f, (float)kamienie[i].pos.y - 7.5f, 0.5f,
	//1.0f, 1.0f,
	//texturki[TEX_GEM]
	//);
	//glPopMatrix();
	//}

	// draw player
	DrawQuadTexture(
	    map_player.pos + Vector3D(0, 0, 0.5f),
	    0.8f, 0.8f,
	    TEX_PLAYER
	);

	if (win)
	{
		/*
		 *DrawQuadTexture(
		 *    (float)map_player.pos.x - 7.5f, (float)map_player.pos.y - 7.5f, 2.5f,
		 *    7.0f, 2.0f,
		 *    texturki[TEX_LVLCMP]
		 *);
		 */
	}
	if (fail)
	{
		/*
		 *DrawQuadTexture(
		 *    (float)map_player.pos.x - 7.5f, (float)map_player.pos.y - 7.5f, 2.6f,
		 *    7.0f, 2.0f,
		 *    texturki[TEX_FAIL]
		 *);
		 */
	}
	if (game_complete)
	{
		/*
		 *DrawQuadTexture(
		 *    (float)map_player.pos.x - 7.5f, (float)map_player.pos.y - 7.5f, 2.7f,
		 *    7.0f, 2.0f,
		 *    texturki[TEX_WIN]
		 *);
		 */
	}

	glPopMatrix();

	//std::string scores = "Score: ";
	//std::stringstream ss;
	//ss << scores << score;
	std::string scores = "(";
	std::stringstream ss;
	ss << scores << map_player.pos.x << "," << map_player.pos.y << "," << map_player.pos.z << ")";
	std::string result = ss.str();
	scoresurf = TTF_RenderText_Solid( fontKomoda, result.c_str(), blueFont );
	texturki[TEX_SCORE] = SurfaceToTexture(scoresurf, TEX_SCORE);
	/*
	 *DrawQuadRGBA(
	 *    7.0f, -6.0f, 2.1f,
	 *    3.5f, 1.5f,
	 *    78, 158, 116, 0.6
	 *);
	 *DrawQuadTexture(
	 *    7.0f, -6.0f, 2.2f,
	 *    3.0f, 1.0f,
	 *    texturki[TEX_SCORE]
	 *);
	 */

	glPushMatrix();
	gluPerspective( 0.0, (float)screen_width / (float)screen_height, 0.0, 1024.0 );
	static float rotat;
	glTranslatef(7.0f, 5.0f, 3.0f);
	glRotatef(rotat += 50.0f * ratio, 0.5f, 1.0f, 0.6f);
	glTranslatef(-7.0f, -5.0f, -3.0f);
	DrawCubeTexture(
	    Vector3D(7.0f, 5.0f, 3.0f),
	    0.8f,
	    TEX_KUCYK
	);
	glPopMatrix();

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

	current_time = (float)last_time / 1000.0f;

	//ImgToTexture( "background.jpg" );
	//LoadMap("maps/map1.txt");
	LoadNextLevel();

	for (;;)
	{
		// Main stuff.
		if (!Events())
			break;

		//MapRead(Vector3D(5,17,0)).print_zawartosc();

		Logic();
		Scene();

		// Calc time.
		unsigned int curr_time = SDL_GetTicks();
		ratio = (float)(curr_time - last_time) / 1000.0f;
		last_time = curr_time;
		current_time = (float)curr_time / 1000.0f;
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

unsigned int ImgToTexture(const char * filename)
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
unsigned int SurfaceToTexture(SDL_Surface * img, unsigned int texture_id)
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
void DrawCubeTexture(Vector3D pos, float a, unsigned int texture_id)
{
	// Enable texturing if needed.
	bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
	if (!texturing_enabled)
		glEnable(GL_TEXTURE_2D);

	// Bind texture and draw.
	glBindTexture(GL_TEXTURE_2D, texturki[texture_id]);
	DrawCube(pos.x, pos.y, pos.z, a);

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
void DrawQuadTexture(Vector3D vect, float w, float h, unsigned int texture_id)
{
	// Enable texturing if needed.
	bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
	if (!texturing_enabled)
		glEnable(GL_TEXTURE_2D);

	// Bind texture and draw.
	glBindTexture(GL_TEXTURE_2D, texturki[texture_id]);
	DrawQuad(vect.x, vect.y, vect.z, w, h);

	// Disable if was disabled.
	if (!texturing_enabled)
		glDisable(GL_TEXTURE_2D);
}

bool LoadNextLevel()
{
	level++;
	char filename[256] = {0};
	snprintf(filename, sizeof(filename) - 1, "maps/map%u.txt", level);
	return LoadMap(filename);
}
bool LoadMap(const char * filename)
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

	map = new MapChunk **[map_height];              // Inicjalizacja trójwymiarowej tablicy
	for (int i = 0; i < map_height ; i++)
		map[i] = new MapChunk*[map_width];
	for (int i = 0; i < map_height ; i++)
		for (int j = 0; j < map_width ; j++)
			map[i][j] = new MapChunk[map_layers];    // end

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
				map[j][i][0].zawartosc.push_back( new EmptySpace() );
				break;
			case 'p':
				map_player.pos = Vector3D(j, i, 1);
				map[j][i][0].zawartosc.push_back( new Floor(Vector3D(j, i, 0)) );
				break;
			case '#':
				map[j][i][0].zawartosc.push_back( new Floor(Vector3D(j, i, 0)) );
				map[j][i][1].zawartosc.push_back( new Wall(Vector3D(j, i, 0)) );
				break;
			case 'g':
				map[j][i][0].zawartosc.push_back( new Floor(Vector3D(j, i, 0)) );
				map[j][i][1].zawartosc.push_back(
				    new Gem( // TODO jebnąć to w Gem();
				        Vector3D(j, i, 0),
				        Vector3D(0, 1.0, 0),
				        (rand() % 10) + 50,
				        (rand() % 360)
				    )
				);
				break;
			case '.':
				map[j][i][0].zawartosc.push_back( new Floor(Vector3D(j, i, 0)) );
				break;
			case 'b':
				map[j][i][0].zawartosc.push_back( new Blinker(Vector3D(j, i, 0),
				                                  (((float)(rand() % 200 )) / 100) + 1,
				                                  (((float)(rand() % 200 )) / 100) + 1
				                                             )
				                                );
				break;
			case 'd':
				map[j][i][0].zawartosc.push_back( new Floor(Vector3D(j, i, 0)) );
				map[j][i][1].zawartosc.push_back( new Door(
				                                      Vector3D(j, i, 0)
				                                  )
				                                );
				break;
			case 'x':
				map[j][i][0].zawartosc.push_back( new Floor(Vector3D(j, i, 0)) );
				map[j][i][1].zawartosc.push_back(
				    new Box(
				        Vector3D(j, i, 1)
				    )
				);
				break;
			case 's':
				map[j][i][0].zawartosc.push_back( new Floor(Vector3D(j, i, 0)) );
				map[j][i][1].zawartosc.push_back(
				    new Switch(
				        Vector3D(j, i, 0),
				        Responser()
				    )
				);
				break;
			default:
				fprintf(stderr, "error: unexpected char \"%c\" in %d %d: \"%s\"\n", line[j], i, j, filename);
			}
		}
	}
	Vector3D trig;
	Vector3D resp;
	Vector3D poprawka(-1, -2, 0);


	/*poprawkaTODO FIXME Powrót to buga z drzwiami dol prawo
	 *size_t s = drzwi.size();
	 *for (size_t i = 0 ; i < s; i++)
	 *{
	 *    drzwi[i] = Door(drzwi[i].pos);
	 *}
	 */

	/*      TODO FIXME Przypisywanie guzikom drzwi.
	 *    while (fgets(line, sizeof(line), f) != NULL)
	 *    {
	 *        std::string line_ = line;
	 *        std::stringstream ss;
	 *        ss << line_;
	 *        ss >> trig.y;
	 *        ss >> trig.x;
	 *        ss >> resp.y;
	 *        ss >> resp.x;
	 *        trig = trig + poprawka;
	 *        resp = resp + poprawka;
	 *        std::cout << trig << "->" << resp << std::endl;
	 *        std::vector<Switch>::iterator sw = getSwitchByVector(trig);
	 *        if ( sw != guziki.end() )
	 *        {
	 *            std::vector<Door>::iterator doo = getDoorByVector(resp);
	 *            if ( doo != drzwi.end() )
	 *            {
	 *                sw->target = &*doo;
	 *            }
	 *            else
	 *            {
	 *                std::cout << "Niepoprawne koordynaty responsera!" << std::endl;
	 *            }
	 *
	 *        }
	 *        else
	 *        {
	 *            std::cout << "Niepoprawne koordynaty triggera!" << std::endl;
	 *        }
	 *    }
	 */

	fprintf(stdout, "info: loaded map \"%s\"\n", filename);
	return true;
}

/* Pozostałości po starych strukturach danych
 *std::vector<rotated>::iterator getGemByVector(Vector3D v)
 *{
 *    std::vector<rotated>::iterator s = kamienie.end();
 *    for ( std::vector<rotated>::iterator i = kamienie.begin() ; i < s; i++)
 *    {
 *        if ( i->pos == v )
 *            return i;
 *    }
 *    return s;
 *}
 *std::vector<Box>::iterator getBoxByVector(Vector3D v)
 *{
 *    std::vector<Box>::iterator s = pudelka.end();
 *    for ( std::vector<Box>::iterator i = pudelka.begin() ; i < s; i++)
 *    {
 *        if ( i->pos == v)
 *            return i;
 *    }
 *    return s;
 *}
 *std::vector<Door>::iterator getDoorByVector(Vector3D v)
 *{
 *    std::vector<Door>::iterator s = drzwi.end();
 *    for ( std::vector<Door>::iterator i = drzwi.begin() ; i < s; i++)
 *    {
 *        if ( i->pos == v)
 *            return i;
 *    }
 *    return s;
 *}
 *std::vector<Switch>::iterator getSwitchByVector(Vector3D v)
 *{
 *    std::vector<Switch>::iterator s = guziki.end();
 *    for ( std::vector<Switch>::iterator i = guziki.begin() ; i < s; i++)
 *    {
 *        if ( i->pos == v)
 *            return i;
 *    }
 *    return s;
 *}
 */

MapChunk & MapRead(Vector3D point)
{
	return map[(int)point.x][(int)point.y][(int)point.z];
}

