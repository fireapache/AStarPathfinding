#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <map>
#include "fixinclude.h"

// Texture IDs for different tile types
enum TextureID {
    TEX_FREE = 0,
    TEX_DIRT,
    TEX_WALL,
    TEX_START,
    TEX_END,
    TEX_PATH,
    TEX_CLOSED,
    TEX_OPENED0,
    TEX_OPENED1,
    TEX_OPENED2,
    TEX_OPENED3,
    TEX_OPENED4,
    TEX_OPENED5,
    TEX_OPENED6,
    TEX_OPENED7,
    TEX_COUNT
};

class Renderer
{
private:
    unsigned int textures[TEX_COUNT];
    int tileSize;
    int gridWidth;
    int gridHeight;
    int scale;

    bool loadTexture(const char* filename, unsigned int* textureID);

public:
    Renderer();
    ~Renderer();

    bool initialize();
    void cleanup();

    void setGridSize(int width, int height);
    void setScale(int scale);

    void renderGrid(MapNode** map, int mapWidth, int mapHeight, int viewportX, int viewportY, int viewportWidth, int viewportHeight);
    void renderTile(int x, int y, TextureID texID);

    // Helper to get texture ID from node state
    TextureID getTextureForNode(const MapNode& node, AncestorDirection ancestor = SELF);

    int getTileSize() const { return tileSize * scale; }
    int getScale() const { return scale; }
};

#endif // RENDERER_H
