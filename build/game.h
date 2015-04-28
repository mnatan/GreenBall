// Functions.

#include <vector>
#include "class/engine/Vector3D.cpp"

static bool Events(Map & map);
static void Logic(Map & map);
static void Scene();

static bool InitSDL(bool fullscreen = false, int width = 860, int height = 640);
static bool InitOpenGL();

unsigned int ImgToTexture(const char *filename);
unsigned int SurfaceToTexture(SDL_Surface *img, unsigned int texture_id);

//bool LoadNextLevel();
//bool LoadMap(const char *filename);

//MapChunk &MapRead(Vector3D point);

void DrawQuad(float x, float y, float z, float w, float h);
void DrawQuadRGBA(float x, float y, float z, float w, float h, float r, float g, float b, float a);
void DrawQuadTexture(Vector3D vect, float w, float h, unsigned int texture_id);

void DrawCube(float x, float y, float z, float a);
void DrawCubeTexture(Vector3D pos, float a, unsigned int texture_id);

//template <class T>
//std::vector<T>::iterator getByVector(Vector3D vect);

/*
 *std::vector<rotated>::iterator getGemByVector(Vector3D v);
 *std::vector<Box>::iterator getBoxByVector(Vector3D v);
 *std::vector<Door>::iterator getDoorByVector(Vector3D v);
 *std::vector<Switch>::iterator getSwitchByVector(Vector3D v);
 */
