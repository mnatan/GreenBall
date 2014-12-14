#ifndef GEM_CPP
#define GEM_CPP

#include "Gem.h"
#include <GL/glut.h>
#include <SDL/SDL_image.h>
#include "SDL/SDL_ttf.h"

bool Gem::drawIt()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // Bugi z przezroczystosicia - Element przezroczysty trzeba rysować na końcu. Może wrzucać to na stosik?
	glEnable(GL_BLEND);

	UpdateAnimation();
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z + 0.5f);
	glRotatef(
	    accumulator,
	    rotationVector.x,
	    rotationVector.y,
	    rotationVector.z
	);
	glTranslatef(-pos.x, -pos.y, -pos.z - 0.5f);
	DrawQuadTexture(
	    pos + Vector3D(0, 0, 0.5),
	    1.0f, 1.0f,
	    TEX
	);
	glPopMatrix();

	glDisable(GL_BLEND);
	return true;
}

void Gem::playerEnters(Vector3D &zmiana)
{
	auto iter = std::find(
	                MapRead(pos).zawartosc.begin(),
	                MapRead(pos).zawartosc.end(),
	                (Box*)this);
	if (iter != MapRead(pos).zawartosc.end())
	{
		MapRead(pos).zawartosc.erase(iter);
	}

	// TODO GAME ENGINE - increment score
}

#endif
