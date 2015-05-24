/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Marcin Natanek, 2015
 */


#include "define.h"
#include "game.h"

#include "src/engine/Vector3D.cpp"

#include "src/game_objects/animated.cpp"
#include "src/game_objects/blinker.cpp"
#include "src/game_objects/Box.cpp"
#include "src/game_objects/Door.cpp"
#include "src/game_objects/EmptySpace.cpp"
#include "src/game_objects/Floor.cpp"
#include "src/game_objects/game_obj.cpp"
#include "src/game_objects/Gem.cpp"
#include "src/game_objects/moved.cpp"
#include "src/game_objects/player.cpp"
#include "src/game_objects/Responder.cpp"
#include "src/game_objects/rotated.cpp"
#include "src/game_objects/scaled.cpp"
#include "src/game_objects/Switch.cpp"
#include "src/game_objects/trigger.cpp"
#include "src/game_objects/Wall.cpp"

#include "src/engine/MapChunk.cpp"

// Globals.


SDL_Color redFont = {255, 0, 0, 0};
SDL_Color greenFont = {0, 255, 0, 0};
SDL_Color blueFont = {0, 0, 255, 0};
SDL_Surface *scoresurf;
TTF_Font *fontKomoda;

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

int main(int argc, char **argv, char **envp)
{
	// Unused.
	UNUSED(argc);
	UNUSED(argv);
	UNUSED(envp);

    GreenEngine main_engine;

    main_engine.Run();

	// Done.
	SDL_Quit();
	return 0;
}
