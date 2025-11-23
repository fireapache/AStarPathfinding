#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Define GL_CLAMP_TO_EDGE if not already defined
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

Renderer::Renderer() : tileSize(8), gridWidth(0), gridHeight(0), scale(1)
{
    for (int i = 0; i < TEX_COUNT; i++)
        textures[i] = 0;
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::loadTexture(const char* filename, unsigned int* textureID)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return false;
    }

    glGenTextures(1, textureID);
    glBindTexture(GL_TEXTURE_2D, *textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    std::cout << "Loaded texture: " << filename << " (" << width << "x" << height << ")" << std::endl;

    return true;
}

bool Renderer::initialize()
{
    // Load all textures
    const char* textureFiles[TEX_COUNT] = {
        "images/free.png",      // TEX_FREE
        "images/dirt.png",      // TEX_DIRT
        "images/wall.png",      // TEX_WALL
        "images/start.png",     // TEX_START
        "images/end.png",       // TEX_END
        "images/path.png",      // TEX_PATH
        "images/closed.png",    // TEX_CLOSED
        "images/opened0.png",   // TEX_OPENED0
        "images/opened1.png",   // TEX_OPENED1
        "images/opened2.png",   // TEX_OPENED2
        "images/opened3.png",   // TEX_OPENED3
        "images/opened4.png",   // TEX_OPENED4
        "images/opened5.png",   // TEX_OPENED5
        "images/opened6.png",   // TEX_OPENED6
        "images/opened7.png"    // TEX_OPENED7
    };

    for (int i = 0; i < TEX_COUNT; i++)
    {
        if (!loadTexture(textureFiles[i], &textures[i]))
        {
            std::cerr << "Failed to load texture " << i << std::endl;
            return false;
        }
    }

    return true;
}

void Renderer::cleanup()
{
    glDeleteTextures(TEX_COUNT, textures);
}

void Renderer::setGridSize(int width, int height)
{
    gridWidth = width;
    gridHeight = height;
}

void Renderer::setScale(int s)
{
    scale = s;
}

TextureID Renderer::getTextureForNode(const MapNode& node, AncestorDirection ancestor)
{
    // Map node state to texture ID
    switch (node.type)
    {
        case WALLNODE:
            return TEX_WALL;
        case WAYNODE:
            if (node.expandCost == 2)
                return TEX_DIRT;
            return TEX_FREE;
        case OPENEDNODE:
            // Use ancestor direction to determine which opened texture
            switch (ancestor)
            {
                case UPLEFT: return TEX_OPENED0;
                case UP: return TEX_OPENED1;
                case UPRIGHT: return TEX_OPENED2;
                case LEFT: return TEX_OPENED3;
                case RIGHT: return TEX_OPENED4;
                case DOWNLEFT: return TEX_OPENED5;
                case DOWN: return TEX_OPENED6;
                case DOWNRIGHT: return TEX_OPENED7;
                default: return TEX_OPENED0;
            }
        case VISITEDNODE:
            return TEX_CLOSED;
        default:
            return TEX_FREE;
    }
}

void Renderer::renderTile(int x, int y, TextureID texID)
{
    if (texID < 0 || texID >= TEX_COUNT)
        return;

    glBindTexture(GL_TEXTURE_2D, textures[texID]);

    float tileW = (float)(tileSize * scale);
    float tileH = (float)(tileSize * scale);
    float posX = (float)(x * tileSize * scale);
    float posY = (float)(y * tileSize * scale);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(posX, posY);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(posX + tileW, posY);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(posX + tileW, posY + tileH);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(posX, posY + tileH);
    glEnd();
}

void Renderer::renderGrid(MapNode** map, int mapWidth, int mapHeight, int viewportX, int viewportY, int viewportWidth, int viewportHeight)
{
    if (!map)
        return;

    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewportWidth, viewportHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear background
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render each tile
    for (int i = 0; i < mapWidth; i++)
    {
        for (int j = 0; j < mapHeight; j++)
        {
            TextureID texID = (TextureID)map[i][j].textureIndex;
            renderTile(i, j, texID);
        }
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}
