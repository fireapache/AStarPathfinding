#ifndef APPLICATION_H
#define APPLICATION_H

#include "astar.h"
#include "fixinclude.h"
#include "renderer.h"
#include <list>
#include <utility>

class Application
{
private:
    AStar astar;
    MapNode** map;
    Renderer* renderer;

    std::list<std::pair<UINT, UINT> > path;
    std::list<std::list<NodeState> > steps;
    std::list<std::list<NodeState> >::iterator stepsIt;

    ToSetState toSetState;

    UINT mapWidth;
    UINT mapHeight;
    UINT itemSize;
    UINT itemScale;

    bool pathOnScreen;
    bool mapBuilt;

    AStarHeuristics currentHeuristic;

    void freeNodes();
    void destroyMap();
    void drawPath();
    void obtainPath(bool saveChanges = false);
    void reDrawStartEndNodes();
    void updateNodeTexture(UINT x, UINT y, TextureID texID);

public:
    Application(Renderer* renderer);
    ~Application();

    void buildMap(UINT width, UINT height, UINT scale);
    void showPath();
    void showNextStep();
    void setHeuristics(AStarHeuristics heuristic);
    void setToSetState(ToSetState state);

    // Mouse interaction
    void handleMouseClick(int screenX, int screenY, bool isDragging, int dragStartX, int dragStartY);

    // Getters
    MapNode** getMap() const { return map; }
    UINT getMapWidth() const { return mapWidth; }
    UINT getMapHeight() const { return mapHeight; }
    UINT getScale() const { return itemScale; }
    bool isMapBuilt() const { return mapBuilt; }
    AStarHeuristics getHeuristics() const { return currentHeuristic; }
    ToSetState getToSetState() const { return toSetState; }
};

#endif // APPLICATION_H
