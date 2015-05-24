/*
 * GreenEngine.h
 * Copyright (C) 2015 Marcin Natanek <natanek.marcin@gmail.com>
 *
 * Distributed under terms of the GPL license.
 */

#ifndef GREENENGINE_H
# define GREENENGINE_H
# include "src/engine/Map.cpp"
# include "src/game_objects/player.cpp"

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <GL/glut.h>
#include <SDL/SDL_image.h>
#include "SDL/SDL_ttf.h"

class GreenEngine {
 public:
	GreenEngine();		// default constuctor

	Map main_map;
	Player main_player;

	static bool Events(Map & map);
	static void Logic(Map & map);
	static void Scene();

	static bool InitSDL(bool fullscreen = false, int width =
			    860, int height = 640);
	static bool InitOpenGL();

	unsigned int ImgToTexture(const char *filename);
	unsigned int SurfaceToTexture(SDL_Surface * img,
				      unsigned int texture_id);

	void DrawQuad(float x, float y, float z, float w, float h);
	void DrawQuadRGBA(float x, float y, float z, float w, float h, float r,
			  float g, float b, float a);
	void DrawQuadTexture(Vector3D vect, float w, float h,
			     unsigned int texture_id);

	void DrawCube(float x, float y, float z, float a);
	void DrawCubeTexture(Vector3D pos, float a, unsigned int texture_id);

 private:

};

#endif				/* !GREENENGINE_H */
