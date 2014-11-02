// Functions.

#include <vector>

static bool Events();
static void Logic();
static void Scene();

static bool InitSDL(bool fullscreen = false, int width = 860, int height = 640);
static bool InitOpenGL();

unsigned int ImgToTexture(const char *filename);
unsigned int SurfaceToTexture(SDL_Surface *img, unsigned int texture_id);

void DrawQuad(float x, float y, float z, float w, float h);
void DrawQuadRGBA(float x, float y, float z, float w, float h, float r, float g, float b, float a);
void DrawQuadTexture(float x, float y, float z, float w, float h, unsigned int texture_id);

void DrawCube(float x, float y, float z, float a);
void DrawCubeTexture(float x, float y, float z, float a, unsigned int texture_id);

bool LoadNextLevel();
bool LoadMap(const char *filename);
static bool AnimUpdate(anim_move_pl *a);
static bool AnimUpdate2(anim_fall_pl *a);
std::vector<gem>::iterator getGemByXY(int x, int y);
