#ifndef GEM_CPP
#define GEM_CPP

#include "Gem.h"
#include <GL/glut.h>
#include <SDL/SDL_image.h>
#include "SDL/SDL_ttf.h"

bool Gem::drawIt()
{
	UpdateAnimation();
	glPushMatrix();
	glTranslatef((float)kamienie[i].pos.x - 7.5f, (float)kamienie[i].pos.y - 7.5f, 0.5f);
	glRotatef(
	    kamienie[i].accumulator,
	    kamienie[i].rotationVector.x,
	    kamienie[i].rotationVector.y,
	    kamienie[i].rotationVector.z
	);
	glTranslatef(-(float)kamienie[i].pos.x + 7.5f, -(float)kamienie[i].pos.y + 7.5f, -0.5f);
	//glTranslatef((float)kamienie[i].x, (float)kamienie[i].y, 0.0f);
	DrawQuadTexture(
	    (float)kamienie[i].pos.x - 7.5f, (float)kamienie[i].pos.y - 7.5f, 0.5f,
	    1.0f, 1.0f,
	    texturki[TEX_GEM]
	);
	glPopMatrix();
	return true;
}

#endif
