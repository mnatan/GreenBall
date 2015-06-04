
/*
 * GreenEngine.h
 * Copyright (C) 2015 Marcin Natanek <natanek.marcin@gmail.com>
 *
 * Distributed under terms of the GPL license.
 */

#ifndef GREENENGINE_H
# define GREENENGINE_H
class Map;
class Player;

# include <vector>
# include <sstream>
# include <iostream>
# include <stdlib.h>
# include <time.h>

# include <GL/glut.h>
# include <SDL/SDL_image.h>
# include "SDL/SDL_ttf.h"

# include "src/engine/Vector3D.cpp"

// old defines
# define UNUSED(a) ((void)(a))

# define FULSCREEN   false
# define WIDTH       860
# define HEIGHT      640

# define TEX_WALL    0
# define TEX_FLOOR   1
# define TEX_STONE   2
# define TEX_PLAYER  3
# define TEX_BACKG   4
# define TEX_GEM     5
# define TEX_SCORE   6
# define TEX_WIN     7
# define TEX_FAIL    8
# define TEX_LVLCMP  9
# define TEX_DOOR    10
# define TEX_KUCYK   11
# define TEX_SWITCH  12
# define TEX_BOX     13

# define MAP_NONE    -1
# define MAP_WALL    0
# define MAP_FLOOR   1
# define MAP_STONE   2
# define MAP_PLAYER  3
# define MAP_BLINKER 4
# define MAP_DOOR    5
# define MAP_SWITCH  6
# define MAP_BOX     7

# define ANIM_PLAYER_TIME    0.1f
# define ANIM_TIME_FALL         1.0f
# define BLINK_FADE_TIME     0.5f
# define WIN_WAIT_TIME       2.0f
# define ANIM_BACKG_TIME     5.0f
# define ANIM_BACKG_SPEED    0.1f
# define ANIM_DOOR_CLOSE    0.5f

// end old defines

class GreenEngine
{
    public:

        static bool win;
        static bool fail;
        static bool game_complete;
        static int level;
        static int score;

        static double win_countdown;

        static int screen_width;
        static int screen_height;
        static bool keys[SDLK_LAST];
        static unsigned int textures[20];

        static double current_time;
        static double ratio;

        static double backdir;
        static double background_start_time;


        static bool OK;

        GreenEngine();     // default constuctor
        ~GreenEngine();     // default destructor

        static bool InitSDL(bool fullscreen = false, int width = 860, int height = 640);
        static bool InitOpenGL();

        bool Events();
        void Logic();
        void Scene();

        void Run(); // start main loop

        static bool LoadGraphics();
        void SetTitle(const char *x);

        bool load_next_level();

        static unsigned int ImgToTexture(const char* filename);
        static unsigned int SurfaceToTexture(SDL_Surface* img, unsigned int texture_id);

        static void DrawQuad(float x, float y, float z, float w, float h);
        static void DrawQuadRGBA(float x, float y, float z, float w, float h,
                                 float r, float g, float b, float a);
        static void DrawQuadTexture(Vector3D vect, float w, float h,
                                    unsigned int texture_id);
        static void DrawCube(float x, float y, float z, float a);
        static void DrawCubeTexture(Vector3D pos, float a,
                                    unsigned int texture_id);

        // Globals TODO
        //static const SDL_Color redFont(255, 0, 0, 0);
        //static const SDL_Color greenFont(0, 255, 0, 0);
        //static const SDL_Color blueFont(0, 0, 255, 0);
        //static const SDL_Surface* scoresurf;
        //static const TTF_Font* fontKomoda;
    private:
        Map* main_map;
        Player* main_player;
};

#endif              /* !GREENENGINE_H */
