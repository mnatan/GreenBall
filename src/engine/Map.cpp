#ifndef MAP_CPP
#   define MAP_CPP

#   include "Map.h"
#   include "src/game_objects/Floor.cpp"
#   include "src/game_objects/Wall.cpp"
#   include "src/game_objects/Gem.cpp"
#   include "src/game_objects/Door.cpp"
#   include "src/game_objects/Switch.cpp"
#   include "src/game_objects/player.cpp"
#   include "src/game_objects/blinker.cpp"
#   include "src/game_objects/Box.cpp"

 Map::Map():map_width(0), map_height(0), map_layers(0), level(1)
{
	printf("loading map:\n");
	this->load_map("maps/map1.txt");
	printf("map loaded.\n");
}

MapChunk & Map::access(Vector3D pos)
{
	return map[(int)pos.x][(int)pos.y][(int)pos.z];
}

bool Map::load_next_level()
{
	level++;
	char filename[256] = { 0 };
	snprintf(filename, sizeof(filename) - 1, "maps/map%u.txt", level);
	// should free last map's memory here TODO
	return load_map(filename);
}

bool Map::load_map(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "error: could not open map file: \"%s\"\n",
			filename);
		return false;
	}
	fscanf(f, "%d", &map_height);
	fscanf(f, "%d", &map_width);
	map_layers = 3;
	char line[128];		// Max map width
	fgets(line, sizeof(line), f);
	printf("map_height:%d\n", map_height);
	printf("map_width:%d\n", map_width);
	printf("map_layers:%d\n", map_layers);

	map = new MapChunk **[map_height];	// Allocate 3D array
	for (int i = 0; i < map_height; i++) {
		map[i] = new MapChunk *[map_width];
		for (int j = 0; j < map_width; j++)
			map[i][j] = new MapChunk[map_layers];
	}
	printf("Map allocated\n");

	for (int i = 0; i < map_width; i++) {
		fgets(line, sizeof(line), f);
		if (feof(f)) {
			fprintf(stderr,
				"error: unexpected eof in: \"%s\" - probbably wrong map in file\n",
				filename);
			fclose(f);
			return false;
		}
		for (int j = 0; j < map_height; j++) {
			switch (line[j]) {
			case ' ':
				map[j][i][0].zawartosc.push_back(EmptySpace());
				break;
			case 'p':
				//TODO FIXME player nieglobalny
				//map_player.pos = Vector3D(j, i, 1);
				map[j][i][0].zawartosc.
				    push_back(Floor(Vector3D(j, i, 0)));
				break;
			case '#':
				map[j][i][0].zawartosc.
				    push_back(Floor(Vector3D(j, i, 0)));
				map[j][i][1].zawartosc.
				    push_back(Wall(Vector3D(j, i, 0)));
				break;
			case 'g':
				map[j][i][0].zawartosc.
				    push_back(Floor(Vector3D(j, i, 0)));
				map[j][i][1].zawartosc.push_back(Gem(	// TODO jebnąć to w Gem();
									    Vector3D
									    (j,
									     i,
									     0),
									    Vector3D
									    (0,
									     1.0,
									     0),
									    (rand
									     ()
									     %
									     10)
									    +
									    50,
									    (rand
									     ()
									     %
									     360)
								 )
				    );
				break;
			case '.':
				map[j][i][0].zawartosc.
				    push_back(Floor(Vector3D(j, i, 0)));
				break;
			case 'b':
				map[j][i][0].zawartosc.
				    push_back(Blinker
					      (Vector3D(j, i, 0),
					       (((float)(rand() % 200)) / 100) +
					       1,
					       (((float)(rand() % 200)) / 100) +
					       1)
				    );
				break;
			case 'd':
				map[j][i][0].zawartosc.
				    push_back(Floor(Vector3D(j, i, 0)));
				map[j][i][1].zawartosc.
				    push_back(Door(Vector3D(j, i, 0)
					      )
				    );
				break;
			case 'x':
				map[j][i][0].zawartosc.
				    push_back(Floor(Vector3D(j, i, 0)));
				map[j][i][1].zawartosc.
				    push_back(Box(Vector3D(j, i, 1)
					      )
				    );
				break;
			case 's':
				map[j][i][0].zawartosc.
				    push_back(Floor(Vector3D(j, i, 0)));
				map[j][i][1].zawartosc.
				    push_back(Switch
					      (Vector3D(j, i, 0), Responder()
					      )
				    );
				break;
			default:
				fprintf(stderr,
					"error: unexpected char \"%c\" in %d %d: \"%s\"\n",
					line[j], i, j, filename);
				return false;
			}
		}
	}
	//Vector3D trig;
	//Vector3D resp;
	Vector3D poprawka(-1, -2, 0);	//wtf?

	fprintf(stdout, "info: loaded map \"%s\"\n", filename);
	return true;
}

bool Map::draw_map()
{
	for (int i = 0; i < map_height; i++) {
		for (int j = 0; j < map_width; j++) {
			for (int k = 0; k < map_layers; k++) {
				this->access(Vector3D(i, j, k)).drawIt();	// lol so short
			}
		}
	}
	return true;
}

#endif
