
/*
 * GreenEngine.cpp
 * Copyright (C) 2015 Marcin Natanek <natanek.marcin@gmail.com>
 *
 * Distributed under terms of the GPL license.
 */

#include "GreenEngine.h"
#include "src/engine/Map.cpp"

GreenEngine::GreenEngine()
{
	OK = true;
	srand(time(NULL));

	// Init SDL.
	if (!InitSDL())
		OK = false;

	// Init OpenGL.
	if (!InitOpenGL())
		OK = false;

	if (TTF_Init() == -1)
		OK = false;

	// Title
	SDL_WM_SetCaption("<<GreenBall>>", "TEST");

	GreenEngine::LoadGraphics();

}

GreenEngine::~GreenEngine()
{
	SDL_Quit();
}

void GreenEngine::Run()
{
	// Main loop:
	unsigned int last_time = SDL_GetTicks();

	GreenEngine::ratio = 0.0f;

	double current_time = (double)last_time / 1000.0f;

	for (;;) {
		// Main stuff.
		printf("for(;;) - now events\n");

		if (!Events(*this))
			break;

		printf("for(;;) - now logic\n");

		//MapRead(Vector3D(5,17,0)).print_zawartosc();

		Logic(*this);

		printf("for(;;) - now Scene\n");
		Scene();

		// Calc time.
		unsigned int curr_time = SDL_GetTicks();

		ratio = (float)(curr_time - last_time) / 1000.0f;
		last_time = curr_time;
		current_time = (float)curr_time / 1000.0f;
	}
}

// Loop functions
bool GreenEngine::Events()
{
	SDL_Event ev;

	memset(&ev, 0, sizeof(ev));

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
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

	if (win && win_countdown < current_time) {
		win = false;
		if (engine.main_map->load_next_level()) {
			game_complete = true;
		}
	}

	return true;
}

void GreenEngine::Logic()
{
	Vector3D zmiana = none;

	// TODO this needs to be moved into player.cpp
	printf("player\n");
	if (!engine.main_player->animating) {
		printf("access\n");
		if (engine.main_map->
		    access(engine.main_player->pos + down).canFall()) {
			fail = true;
			engine.main_player->setAnimation(engine.
							 main_player->pos,
							 Vector3D
							 (engine.main_player->pos.
							  x,
							  engine.main_player->pos.
							  y, -21.0f),
							 ANIM_TIME_FALL);
		}
		if (!fail) {
			if (keys[SDLK_UP]) {
				zmiana = top;
				keys[SDLK_UP] = false;
			} else if (keys[SDLK_DOWN]) {
				zmiana = bot;
				keys[SDLK_DOWN] = false;
			} else if (keys[SDLK_LEFT]) {
				zmiana = left;
				keys[SDLK_LEFT] = false;
			} else if (keys[SDLK_RIGHT]) {
				zmiana = right;
				keys[SDLK_RIGHT] = false;
			}
		}
	}

	printf("other\n");
	if (zmiana != zero) {
		Vector3D nowaPozycja = engine.main_player->pos + zmiana;

		if (engine.main_map->
		    access(nowaPozycja).canEnter(*engine.main_map, zmiana)) {
			engine.main_map->
			    access(nowaPozycja).playerEnters(*engine.main_map,
							     zmiana);
			engine.main_player->setAnimation(engine.
							 main_player->pos,
							 nowaPozycja,
							 ANIM_PLAYER_TIME);
		}
	}
	if (engine.main_player->animating) {
		engine.main_player->animating =
		    engine.main_player->UpdateAnimation(ratio);
	}
}

void GreenEngine::Scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -14.0f);
	glScalef(1.0f, -1.0f, 1.0f);

	// animacja tła - czasowo lewo prawo
	if (current_time - background_start_time > ANIM_BACKG_TIME) {
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

	DrawQuadTexture(Vector3D(0.0f, 0.0f, -20.1f),
			4.0f * skalar, 3.0f * skalar, TEX_BACKG);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-engine.main_player->pos.x, -engine.main_player->pos.y,
		     -0.0f);

	// Draw map
	// TODO FIXME opuszczamy globale
	//engine.main_map->draw_map();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// draw player
	DrawQuadTexture(engine.main_player->pos + Vector3D(0, 0, 0.5f),
			0.8f, 0.8f, TEX_PLAYER);

	if (win) {
		/*
		 *DrawQuadTexture(
		 *    (float)engine.main_player->pos.x - 7.5f, (float)engine.main_player->pos.y - 7.5f, 2.5f,
		 *    7.0f, 2.0f,
		 *    textures[TEX_LVLCMP]
		 *);
		 */
	}
	if (fail) {
		/*
		 *DrawQuadTexture(
		 *    (float)engine.main_player->pos.x - 7.5f, (float)engine.main_player->pos.y - 7.5f, 2.6f,
		 *    7.0f, 2.0f,
		 *    textures[TEX_FAIL]
		 *);
		 */
	}
	if (game_complete) {
		/*
		 *DrawQuadTexture(
		 *    (float)engine.main_player->pos.x - 7.5f, (float)engine.main_player->pos.y - 7.5f, 2.7f,
		 *    7.0f, 2.0f,
		 *    textures[TEX_WIN]
		 *);
		 */
	}

	glPopMatrix();

	//std::string scores = "Score: ";
	//std::stringstream ss;
	//ss << scores << score;
	std::string scores = "(";
	std::stringstream ss;
	ss << scores << engine.main_player->pos.x << "," << engine.main_player->
	    pos.y << "," << engine.main_player->pos.z << ")";
	std::string result = ss.str();
	scoresurf = TTF_RenderText_Solid(fontKomoda, result.c_str(), blueFont);
	textures[TEX_SCORE] = SurfaceToTexture(scoresurf, TEX_SCORE);
	/*
	 *DrawQuadRGBA(
	 *    7.0f, -6.0f, 2.1f,
	 *    3.5f, 1.5f,
	 *    78, 158, 116, 0.6
	 *);
	 *DrawQuadTexture(
	 *    7.0f, -6.0f, 2.2f,
	 *    3.0f, 1.0f,
	 *    textures[TEX_SCORE]
	 *);
	 */

	glPushMatrix();
	gluPerspective(0.0, (float)screen_width / (float)screen_height, 0.0,
		       1024.0);
	static float rotat;

	glTranslatef(7.0f, 5.0f, 3.0f);
	glRotatef(rotat += 50.0f * ratio, 0.5f, 1.0f, 0.6f);
	glTranslatef(-7.0f, -5.0f, -3.0f);
	DrawCubeTexture(Vector3D(7.0f, 5.0f, 3.0f), 0.8f, TEX_KUCYK);
	glPopMatrix();

	glDisable(GL_BLEND);

	SDL_GL_SwapBuffers();
}

// Init
static bool GreenEngine::InitSDL(bool fullscreen, int width, int height)
{
	int screen_bpp;
	int screen_flag = SDL_OPENGL;

	if (fullscreen)
		screen_flag |= SDL_FULLSCREEN;

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
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_putenv(const_cast < char *>("SDL_VIDEO_CENTERED=center"));

	surface = SDL_SetVideoMode(screen_width, screen_height,
				   screen_bpp, screen_flag);

	if (surface == NULL)
		goto err;

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
			    SDL_DEFAULT_REPEAT_INTERVAL);

	return true;

 err:
	SDL_Quit();
	return false;
}

static bool GreenEngine::InitOpenGL()
{
	float ratio;

	ratio = (float)screen_width / (float)screen_height;

	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, ratio, 1.0, 1024.0);
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

static bool GreenEngine::LoadGraphics()
{
	textures[TEX_WALL] = ImgToTexture("gfx/wall.png");
	textures[TEX_FLOOR] = ImgToTexture("gfx/floor.png");
	textures[TEX_STONE] = ImgToTexture("gfx/ball.png");
	textures[TEX_PLAYER] = ImgToTexture("gfx/player.png");
	textures[TEX_BACKG] = ImgToTexture("gfx/background.jpg");
	textures[TEX_GEM] = ImgToTexture("gfx/gem.png");
	//kombinowanie
	textures[TEX_SCORE] = ImgToTexture("gfx/gem.png");

	// z jakiegoś powodu musi być po score !! FIXME
	textures[TEX_WIN] = ImgToTexture("gfx/win.png");
	textures[TEX_FAIL] = ImgToTexture("gfx/fail.png");
	textures[TEX_LVLCMP] = ImgToTexture("gfx/lvlcmp.png");
	textures[TEX_DOOR] = ImgToTexture("gfx/door.png");
	textures[TEX_KUCYK] = ImgToTexture("gfx/kucyk.png");
	textures[TEX_SWITCH] = ImgToTexture("gfx/switch.png");
	textures[TEX_BOX] = ImgToTexture("gfx/box.png");

	fontKomoda = TTF_OpenFont("font/Komoda.ttf", 35);
}

// Graphic helpers
unsigned int GreenEngine::ImgToTexture(const char *filename)
{
	// Load image.
	SDL_Surface *img = IMG_Load(filename);

	if (!img) {
		fprintf(stderr,
			"error: ImgToTexture could not load image \"%s\"\n",
			filename);
		return ~0;
	}
	// Image type? Only 24 and 32 supported, rest must be converted.
	unsigned int img_type = 0;

	if (img->format->BitsPerPixel == 32) {
		img_type = GL_RGBA;
	} else if (img->format->BitsPerPixel == 24) {
		img_type = GL_RGB;
	} else {

		// Convert to 32 bits.
		SDL_PixelFormat fmt = {
			NULL, 32, 4,
			0, 0, 0, 0,
			0, 8, 16, 24,
			0xff, 0xff00, 0xff0000, 0xff000000,
			0,
			0xff
		};

		SDL_Surface *nimg =
		    SDL_ConvertSurface(img, &fmt, SDL_SWSURFACE);
		SDL_FreeSurface(img);

		if (!nimg) {
			fprintf(stderr,
				"error: ImgToTexture could not convert image \"%s\" to 32-bit\n",
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

unsigned int GreenEngine::SurfaceToTexture(SDL_Surface * img,
					   unsigned int texture_id)
{
	// Image type? Only 24 and 32 supported, rest must be converted.
	unsigned int img_type = 0;

	if (img->format->BitsPerPixel == 32) {
		img_type = GL_RGBA;
	} else if (img->format->BitsPerPixel == 24) {
		img_type = GL_RGB;
	} else {

		// Convert to 32 bits.
		SDL_PixelFormat fmt = {
			NULL, 32, 4,
			0, 0, 0, 0,
			0, 8, 16, 24,
			0xff, 0xff00, 0xff0000, 0xff000000,
			0,
			0xff
		};

		SDL_Surface *nimg =
		    SDL_ConvertSurface(img, &fmt, SDL_SWSURFACE);
		SDL_FreeSurface(img);

		if (!nimg) {
			fprintf(stderr,
				"error: ImgToTexture could not convert surface to 32-bit\n");
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

void GreenEngine::DrawCube(float x, float y, float z, float a)
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

void GreenEngine::DrawCubeTexture(Vector3D pos, float a,
				  unsigned int texture_id)
{
	// Enable texturing if needed.
	bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);

	if (!texturing_enabled)
		glEnable(GL_TEXTURE_2D);

	// Bind texture and draw.
	glBindTexture(GL_TEXTURE_2D, textures[texture_id]);
	DrawCube(pos.x, pos.y, pos.z, a);

	// Disable if was disabled.
	if (!texturing_enabled)
		glDisable(GL_TEXTURE_2D);
}

void GreenEngine::DrawQuad(float x, float y, float z, float w, float h)
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

void GreenEngine::DrawQuadRGBA(float x, float y, float z, float w, float h,
			       float r, float g, float b, float a)
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

void GreenEngine::DrawQuadTexture(Vector3D vect, float w, float h,
				  unsigned int texture_id)
{
	// Enable texturing if needed.
	bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);

	if (!texturing_enabled)
		glEnable(GL_TEXTURE_2D);

	// Bind texture and draw.
	glBindTexture(GL_TEXTURE_2D, textures[texture_id]);
	DrawQuad(vect.x, vect.y, vect.z, w, h);

	// Disable if was disabled.
	if (!texturing_enabled)
		glDisable(GL_TEXTURE_2D);
}
