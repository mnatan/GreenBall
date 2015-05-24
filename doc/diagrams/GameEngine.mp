input metauml;

beginfig(1);
iClass.foreColor := (.5, 1, .5);

Class.Engine("GameEngine")
		("map : Map",
		"score : int",
		"current_level : int")
		("-Logic() : bool",
		"-Events(): bool",
		"-Scene(): bool");
Engine.info.foreColor := (.9,.7,.7);
Class.Map("Map")
		("map: **MapChunk",
		"map_width: int",
		"map_height: int",
		"map_layers: int")
		("access(Vector3D pos) : &MapChunk ",
		"map_remove(Vector3D where, game_obj what) : bool",
		"map_insert(Vector3D where, game_obj what) : bool",
		"load_level(int num) : bool",
		"load_map(const char * filename) : bool",
		"draw_map() : bool");
Map.info.foreColor := (1,1,.5);
Class.MapChunk("MapChunk")
		("zawartosc : vector<game_obj>")
		( "canFall() : bool ",
		"drawIt() : bool ",
		"canEnter(Map & map, Vector3D &zmiana) : bool ",
		"playerEnters(Map & map, Vector3D &zmiana) : bool ",
		"print_zawartosc() : void ");
MapChunk.info.foreColor := (1,1,.5);

leftToRight(30)(Map,MapChunk);
Group.maps(Map,MapChunk);
topToBottom(30)(Engine,maps);

drawObjects(Engine,maps);

clink(associationUni)(Map,Engine);
clink(associationUni)(MapChunk,Map);

Class.Vector("Vector3D")
		("+x: float",
		"+y: float",
		"+z: float")
		("+basic aritmetrics(+,-,*,/,==,!=)");

topToBottom(30)(maps, Vector);

drawObjects(Vector);
clink(associationUni)(Vector,Map);
clink(associationUni)(Vector,MapChunk);

endfig;
				   end;
