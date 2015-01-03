#ifndef MAP_CPP
#define MAP_CPP

#include "Map.h"

Map::Map(int height, int width, int depth) :
map_width(width), map_height(height), map_layers(depth), level(1)
{
	this->load_map("maps/map1.txt");
}

MapChunk & Map::map_access(Vector3D pos)
{
	return map[(int)pos.x][(int)pos.y][(int)pos.z];
}

bool Map::load_next_level()
{
	level++;
	char filename[256] = {0};
	snprintf(filename, sizeof(filename) - 1, "maps/map%u.txt", level);
	return load_map(filename);
}

bool Map::load_map(const char * filename)
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
				//TODO FIXME player nieglobalny
				//map_player.pos = Vector3D(j, i, 1);
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

	fprintf(stdout, "info: loaded map \"%s\"\n", filename);
	return true;
}

bool Map::draw_map()
{
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
}

#endif
