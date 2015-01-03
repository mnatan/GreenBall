#ifndef MAP
#define MAP

#include "MapChunk.cpp"
#include "Vector3D.cpp"
#include "../game_objects/game_obj.cpp"

class Map
{
	public:
	Map(int height, int width, int depth);

	MapChunk & map_access(Vector3D pos);
	bool map_remove(Vector3D where, game_obj what);
	bool map_insert(Vector3D where, game_obj what);
	bool load_next_level();
	bool load_map(const char * filename);

	private:
	MapChunk ***map;
	int map_width;
	int map_height;
	int map_layers = 2;
	int level;
};

#endif
