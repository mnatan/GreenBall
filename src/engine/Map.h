#ifndef MAP
#   define MAP

#   include "Vector3D.cpp"
#   include "src/game_objects/game_obj.cpp"
#   include "src/engine/MapChunk.cpp"
class MapChunk;
class game_obj;

class Map {
 public:
	Map();

	MapChunk & access(Vector3D pos);
	bool map_remove(Vector3D where, game_obj what);
	bool map_insert(Vector3D where, game_obj what);
	bool load_next_level();
	bool load_map(const char *filename);
	bool draw_map();

 private:
	 MapChunk *** map;
	int map_width;
	int map_height;
	int map_layers = 2;
	int level;
};

#endif
